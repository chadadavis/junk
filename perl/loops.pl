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

my $n = <STDIN>;
chomp $n;
##### $n;
for (my $i = 1; $i <= 10; $i++) {
    say "$n x $i = " . ($n * $i);
}
