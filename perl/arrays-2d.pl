#!/usr/bin/env perl

use strict;
use warnings;
use autodie;
use feature 'say';
use open qw(:utf8 :std);
use Carp qw(cluck confess);
$SIG{__DIE__} ||= \&confess;
use List::Util qw(min max sum sum0);
use JSON::PP;
use Getopt::Long;
use Devel::Comments '#####';

my @a;
while (my $l = <STDIN>) {
    chomp $l;
    push @a, [ split ' ', $l ];
}

# Because it might be only negatives
my $max = -inf;

for (my $i = 1; $i < @a - 1; $i++) {
    ##### $i
    my @r = @{$a[$i]};
    for (my $j = 1; $j < @r -1; $j++) {
        ##### $j
        # One hourglass:
        my $sum = 0;
        # Relative coords around center
        for ( [0,0], [-1,-1], [-1,0], [-1,1], [1,-1], [1,0], [1,1] ) {
            my ($x,$y) = @$_;
            $sum += $a[$i+$x][$j+$y];
        }
        ##### $sum
        $max = $sum if $sum > $max;
    }
}

say $max;
