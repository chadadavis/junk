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

my %h;
for (1..$n) {
    my $l = <STDIN>;
    chomp $l;
    my ($name, $num) = split ' ', $l;
    $h{$name} = $num;
}

##### %h;

while (my $l = <STDIN>) {
    chomp $l;
    my $v = $h{$l};
    if (! $v) {
        say "Not found";
        next;
    }
    say "$l=$h{$l}";
}


