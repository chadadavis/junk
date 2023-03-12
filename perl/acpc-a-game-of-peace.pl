#!/usr/bin/env perl

=head1 SYNOPSIS

ACPC Problem A

=head1 DESCRIPTION



=cut

use v5.14;
use strict;
use warnings;
use autodie;
use open qw(:utf8 :std);

use Log::Any qw($log);
use Carp qw(cluck);
$SIG{__DIE__} ||= \&confess;

use IO::All;
use Data::Dump;
use Devel::Comments;
use Test::More qw(no_plan);
use Getopt::Long;

$| = 1;

my @cases;
my $n = <>;
my $case_n = 1;
while (<>) {
    chomp;
    my ($x, $n, $y, $m) = split /\s+/;
    my ($a,$b) = pour($x,0,$n);
    if ($a > $b) { $a += $y; }
    else { $b += $y; }
    ($a, $b) = pour($a,$b,$m-$n);
    say "Case $n: ", gcd($a,$b);
}

sub pour {
    my ($a, $b, $n) = @_;
    return ($a,$b) if $n == 0;
    if ($a < $b) { $a += $b; }
    else { $b += $a; }
    return pour($a,$b,$n-1);
}

sub gcd {
    my ($x, $y) = @_;
    ($x, $y) = sort { $a <=> $b } ($x, $y);
    return $x if $y % $x == 0;
    return gcd($x, $y % $x);
}

sub divisors {
    my ($a) = @_;
    my @divisors;
    for (my $i = 1; $i <= $a; $i++) {
        push @divisors, $i if $a % $i == 0;
    }
    return @divisors;
}

