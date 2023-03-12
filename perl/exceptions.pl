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

my $s = <STDIN>;
chomp $s;

eval {
    use warnings FATAL => "all";
    die unless $s == $s;
    1;
} or do {
    say "Bad String";
    exit;
};

say $s;
