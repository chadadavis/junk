#!/usr/bin/env perl

=head1 NAME

EMBL::Point - 

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



package EMBL::Point;

use overload (
#     '-' => 'difference',
    '-' => 'difference2',
    '""' => 'stringify',
    );

use PDL;
use PDL::Math;
use PDL::Matrix;

use lib "..";
# use EMBL::Table;


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

    $self->{p} = pdl @args;

    return $self;

} # new



sub sq {
    my ($x) = @_;
    return $x*$x;
}

sub stringify {
    my ($self) = @_;
    return $self->{p};
}

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

    return sqrt(sq($self->{x} - $obj->{x}) + 
                sq($self->{y} - $obj->{y}) + 
                sq($self->{z} - $obj->{z}));

} 

sub difference2 {
    my ($a, $b) = @_;
    # square root of: sum of: differences squared
    return sqrt sumover(($a - $b) ** 2);
}


sub transform2 {

}

#sub transform {
#    my ($raw) = @_;
#
#    if (ref($raw)) {
#        # Assume it's a ref to a 2D array
#    } else {
#        # Assume it's a string to be parsed into a 2D array
#        $raw = table($raw);
#    }
#
#    my $z = mpdl zeroes(4,4);
#    # Append 1 at cell 3,3 for affine transform
#    $z->slice('3,3') += 1;
#
#    # Add the transformation on top
#    my $matrix_transform = mpdl $raw;
#    $z->slice('0:2,0:3') += $matrix_transform;
#
#    # TODO if going to be doing multiple transforms, save this pdl in $self
#    # Append 1 for affine computation
#    my $pt_vector = mpdl ($self->x, $self->y, $self->z, 1);
#    # Transpose row to a column vector
#    $pt_vector = transpose($pt_vector);
#
#    # Finally, transform vect using matrix
#    my $new = $z x $pt_vector;
##     ($self->{x}, $self->{y}, $self->{z}) = 
#
#}



###############################################################################

1;

__END__
