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

while (my $s = <STDIN>) {
    chomp $s;
##### $s
    my $even = '';
    my $odd  = '';
    for (my $i = 0; $i < length($s); $i++) {
        if (0 == $i % 2) {
            $even .= substr($s, $i, 1);
            ##### $even
        }
        else {
            $odd .= substr($s, $i, 1);
            ##### $odd
        }
    }
    say "$even $odd";
}
