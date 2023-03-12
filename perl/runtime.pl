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

NUM:
while (my $l = <STDIN>) {
    chomp $l;
    if ($l <= 1) {
        ##### $l
        say "Not prime";
        next NUM;
    }
    if ($l == 2) {
        ##### $l
        say "Prime";
        next NUM;
    }
    if (0 == $l % 2) {
        ##### $l
        say "Not prime";
        next NUM;
    }
    my $i = 3;
  CHECK:
    for (; $i <= int( sqrt($l) ); $i+=2) {
        if (0 == $l % $i) {
            ##### $l
            say "Not prime";
            next NUM;
        }
    }
    ##### $l
    say "Prime";

}
