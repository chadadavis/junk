#!/usr/bin/env perl

=head1 NAME

SBG::HashFields - Create hash-based accessors

=head1 SYNOPSIS

 package MyPackage;
 use SBG::HashFields;

 hashfield 'mystuff';

 my $obj = new MyPackage;
 $obj->mystuff('key', 'value');
 print "The value is:", $obj->mystuff('key');


=head1 DESCRIPTION


=head1 SEE ALSO

L<Moose::Role>

=cut

################################################################################

package SBG::HashFields;

use base qw(Exporter);
our @EXPORT = qw(hashfield);


################################################################################
=head2 hashfield

 Function:
 Example :
 Returns : 
 Args    :

Creates a method to access a dynamically-created hash in your class. The hash is
one-dimensional.

E.g. in the body of your class:
Allows you to then:
my $obj = new MyClass(); 
$obj->color("favourite", "blue"); k
print $obj->color("favourite"), "\n";

See also B<field> from L<Spiffy> and B<has> from L<Moose>

If $names is also given, it creates an access for the HashRef itself

=cut
sub hashfield {
    my ($name, $names) = @_;

    my $code = <<END;
    sub {
        my (\$self,\$key,\$val) = \@_;
        \$self->{"$name"} ||= {};
        \$self->{"$name"}{\$key} = \$val if defined \$val;
        \$self->{"$name"}{\$key};
    }
END
    # Fully qualified name of method to be created
    my ($pkg) = caller();
    my $full = "${pkg}::${name}";
    *$full = eval $code;

    return unless $names;

    # Also create an accessor for the HashRef itself?
    $full = "${pkg}::${names}";
    $code = "sub { (shift)->{\"$name\"} || {} }";
    *$full = eval $code;
} 



################################################################################
1;

