#!/usr/bin/env perl

=head1 NAME

EMBL::Sphere - 

=head1 SYNOPSIS


=head1 DESCRIPTION



=head1 BUGS

None known.

=head1 REVISION

$Id: Prediction.pm,v 1.33 2005/02/28 01:34:35 uid1343 Exp $

=head1 APPENDIX

Details on functions implemented here are described below.
Private internal functions are generally preceded with an _

=cut

################################################################################

package EMBL::Sphere;

use lib "..";
use base qw(EMBL::Point);


use overload (
    '-' => 'difference',
    );



################################################################################
=head2 new

 Title   : new
 Usage   : 
 Function: 
 Returns : 
 Args    :

=cut

sub new {
    my ($class, @args) = @_;
    my $self = {};
    bless $self, $class;

    $self->{centre} = $self->{center} = new EMBL::Point(@args);
    $self->{radius} = $args[3] || 0;

    return $self;

} # new


################################################################################
=head2 difference

 Title   : difference
 Usage   : 
 Function: 
 Returns : 
 Args    : 

=cut

sub difference {
    my ($self, $obj) = @_;

    return $self->{radius} + $obj->{radius} - ($self->{centre} - $obj->{centre});

} # add_template


# Similar, but requires no sqrt calculation (which could be costly)
sub overlaps {
    my ($self, $obj, $thresh) = @_;
    $thresh ||= 0;
    my $sqdist = 
            sq($self->centre->{x} - $obj->centre->{x}) + 
            sq($self->centre->{y} - $obj->centre->{y}) +
            sq($self->centre->{z} - $obj->centre->{z});
    my $radii = $self->{radius} + $obj->{radius};
    return $sqdist + $thresh <= $radii * $radii;
}


###############################################################################

1;

__END__
