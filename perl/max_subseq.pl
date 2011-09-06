#!/usr/bin/env perl
use strict;
use warnings;

sub max_subseq2 {
    my @a = @_;
    my ($maxv, $maxi, $maxj) = (0) x 3;
    my ($currv, $curri, $currj) = (0) x 3;

    for ( ; $currj < @a; $currj++) {
        $currv += $a[$currj];
        if ($currv > $maxv) {
            ($maxv, $maxi, $maxj) = ($currv, $curri, $currj);
        } elsif ($currv < 0) {
            $curri = $currj + 1;
            $currv = 0;
        }
    }
    return ($maxv, $maxi, $maxj);
}


################################################################################

1;
__END__
