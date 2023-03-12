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
# use Devel::Comments '#####';


my $n = <STDIN>;
chomp $n;

my $l = <STDIN>;
chomp $l;
my @a = split ' ', $l;

my $numberOfSwaps = 0;
for (my $i = 0; $i < $n; $i++) {
#     // Track number of elements swapped during a single array traversal
        

    for (my $j = 0; $j <$n - 1; $j++) {
#         // Swap adjacent elements if they are in decreasing order
            if ($a[$j] > $a[$j + 1]) {
                ($a[$j], $a[$j + 1]) = ($a[$j+1], $a[$j]);
                $numberOfSwaps++;
            }
    }

#     // If no elements were swapped during a traversal, array is sorted
    if ($numberOfSwaps == 0) {
        last;
    }
}

say "Array is sorted in $numberOfSwaps swaps.";
say "First Element: $a[0]";
say "Last Element: $a[-1]";
