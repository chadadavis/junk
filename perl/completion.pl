#!/usr/bin/env perl

=head1 NAME

B<sbgobj> - Interface to serialized objects 

=head1 SYNOPSIS

sbgobj myfile.stor <object-method> <parameters> ...

=head1 DESCRIPTION

Calls the given method on the object stored in myfile.stor

If no method is given, the object is printed.


Uses bash autocomplete:
 
 complete -o default -C 'sbgobj -options' sbgobj


=head1 OPTIONS

=head2 -k Keep temporary files

For debugging

=head2 -l Log level

One of:

 TRACE DEBUG INFO WARN ERROR FATAL

=head2 -h Help

=head1 SEE ALSO

L<SBG::Role::Storable>

=cut

################################################################################

# NB requires bash cmd: complete  -o default -C scriptname scriptname
# Synopsis:
use SBG::U::Complete qw/complete_methods/;
use Getopt::Complete ( 
    'keep!'      => undef,
    'help!'      => undef,
    'log=s'      => [ qw/TRACE DEBUG INFO WARN ERROR FATAL/ ],
    # For everything else that's not a file, try method name completion
    '<>'         => \&complete_methods,
    );

print "help\n" if $ARGS{help};
my @bare_args = @{$ARGS{'<>'}};
# my @oparams = map { -s $_ ? load($_) : $_ } @params;

print "params:@params:\n";

# print "ARGS:", join(',',%ARGS), "\n";

print "Done\n";


__END__


