#!/usr/bin/env perl

=head1 SYNOPSIS

Given a list of hotels (with prices) and a budget, write a function that returns all pairs of hotels with the sum of their prices equal to the budget.

=head1 DESCRIPTION

This question can be generalised as:
Given an array of numbers, find all pairs whose sum is equal to X.

Possible follow-ups:
- Modify the algorithm to include close matches with a predefined margin of error.
- Instead of looking for pairs, add a new parameter that defines the number of hotels in each returned set.

From jure.merhar

=cut

use v5.14;
use strict;
use warnings;
no warnings 'experimental::autoderef';
use autodie;
use open qw(:utf8 :std);
use Log::Any qw($log);
use Carp qw(cluck confess);
$SIG{__DIE__} ||= \&confess;
use IO::All;
use Data::Dump qw(dump);
# use Devel::Comments;
use Test::More qw(no_plan);
use Getopt::Long;

my @hotels = map { { id => $_, price => 25 + int rand 100 } } 1..5;
# Cheapest first, but it's exhaustive anyway ...
# If it were gready, would be likely to find 1 solution sooner
my $hotels = [ sort { $a->{price} <=> $b->{price} } @hotels ];
my $nights = 2 + int rand 5;
my $nightly_avg = 50 + int rand 50;
my $budget = $nights * $nightly_avg;

my @combos = budget($nights, $budget, [], $hotels);
# Sort by getting as close to budget without exceeding
@combos = sort { $a->{left} <=> $b->{left} } @combos;

for my $solution ( @combos ) {
    dump $solution;
}
say "budget\t$budget";
say "nights\t$nights";
exit;

sub budget {
    #### @_
    my ($nights, $left, $selected, $avail) = @_;
    if ($nights == 0 && $left >= 0) {
        my $total;
        $total += $_->{price} for @$selected;
        return {
            total  => $total,
            hotels => $selected,
            left   => $left,
        };
    }
    return if $left < 0 || ! @$avail;

    my @avail_copy = @$avail;
    my $candidate = shift @avail_copy;
    my $price = $candidate->{price};

    my @solutions;
    # Traverse power set via binary decision at each step
    for my $choose (0..1) {
        my @sol = budget($nights - $choose, $left - $choose * $price, [ $choose ? $candidate : (), @$selected ], \@avail_copy );
        push @solutions, @sol;
    }
    return @solutions;
}

