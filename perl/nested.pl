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


my $la = <STDIN>;
chomp $la;
my ($da,$ma,$ya) = split ' ', $la;

my $le = <STDIN>;
chomp $le;
my ($de,$me,$ye) = split ' ', $le;

my $fine = 0;
if ($ya > $ye) {
    $fine = 10000;
}
elsif ($ya == $ye && $ma > $me) {
    $fine = 500 * ($ma - $me);
}
elsif ($ya == $ye && $ma == $me && $da > $de) {
    $fine = 15 * ($da - $de);
}
else {
}

say $fine;
