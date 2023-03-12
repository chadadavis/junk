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


my $t = <STDIN>;
chomp $t;

for my $i (1..$t) {
    my $c = <STDIN>;
    chomp $c;
    my ($e, $a) = split ' ', $c;
    ##### $e
    ##### $a
    my $t = <STDIN>;
    chomp $t;
    my @t = split ' ', $t;
    ##### @t
    my $n_timely = grep { $_ <= 0 } @t;
    ##### $n_timely
    if ($n_timely < $a) {
        say "YES";
    }
    else {
        say "NO";
    }
}
