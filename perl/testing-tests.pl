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


my $n_max = 200;
# my $n_max = 200;
# my $n_max = 10;
my $t = 5;

say $t;
for my $i (1..$t) {
    my $n = 2 + int rand($n_max-1);
    my $k = 2 + int rand($n-1);
    say "$n $k";
    my $cancelled = $i % 2;
    my $late   = $cancelled ? $n - $k + 1 : $n - $k;
    my $timely = $cancelled ? $k - 1      : $k;
    my @a = (-1);
    push @a, (0) x ($timely - 1);
    push @a, (1) x $late;
    say join ' ', @a;
}
