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

my $b = sprintf "%b", $n;

my $max = 0;
my $current = 0;
for (my $i = 0; $i < length($b); $i++) {
    my $c = substr($b, $i, 1);
    if ($c eq '1') {
        $current++;
        $max = $current if $current > $max;
    }
    else {
        $current = 0;
    }
}
say $max;

