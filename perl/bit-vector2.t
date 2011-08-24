#!/usr/bin/env perl

use Test::More 'no_plan';
use Bit::Vector::Overload;
Bit::Vector->Configuration("in=enum,out=bin");

# Finally, the keyword "^enum" causes scalar input to be considered as being a list ("enumeration") of indices and ranges of (contiguous) indices, i.e., "$vector |= '2,3,5,7-13,17-23';" will cause bits #2, #3, #5, #7 through #13 and #17 through #23 to be set.


################################################################################

# Like ceil() but finds the next power of 2 rather than just the next integer
sub ceilpower2 {
    my $x = shift;
    return 1 unless $x > 0;
    # Number is already a power of 2?
    # Example: 8=>1000, 7=>0111, AND operator sets every bit to 0
    return $x unless $x & ($x-1);
    # Otherwise do ceil(log base2)
    my $r = 1 + int(log($x) / log(2));
    return 2 ** $r;
}


sub bitvec_subset {
    my ($bitvec) = @_;
    # Make sure to use 'b' rather than 'B' here, we want to index from the left
    my @bits = split(//, unpack("b*", $bitvec));
    my @enabled = grep { $bits[$_] } (0..$#bits);
    return @enabled;
}


#NB 
# Least significant bit on the right
# To get the better scoring templates to be tried together first, sort desc
my @sorted_sedges = qw/great good better ok worse worst/;

my $nedges = scalar @sorted_sedges;

# vec() requires number of bits to be a power of 2
my $vecsize = ceilpower2 $nedges;
# Next smallest power of 2 that's greater than 6 is: 8
is($vecsize, 8, 'ceilpower2');

# Bit Vector
my $bitvec;
# Set all bits to enabled/on
vec($bitvec, 0, $vecsize) = 2 ** $nedges - 1;

# The following efficiently counts the number of set bits in a bit vector:
#                    $setbits = unpack("%32b*", $selectmask);
diag "array: ", unpack("B*", $bitvec), "\n";
my @names = @sorted_sedges[bitvec_subset($bitvec)];
diag "names : @names\n";

use Bit::Vector::Overload;

# NB this does not work (
# $bitvec = $bitvec - 1;
# print "array: $bitvec\n";

