#!/usr/bin/env perl

=head1 SUMMARY

Given multisets, find the intersection. In a multiset, one element can be
present multiple times. Output should also be a multiset.

Glasdoor discussion
http://u.booking.com/st

=cut

use Modern::Perl;
use Data::Dump qw(dump);
use Devel::Comments;
use List::Util qw(min);

# Accepts a list of lists
sub intersect {
    # All symbols ever seen, across all sets
    my %total;
    # For each list, count symbols seen
    my @counts;
    for (my $i = 0; $i < @_; $i++) {
        my $sublist = $_[$i];
        for my $elem (@$sublist) {
            $counts[$i] //= {};
            $counts[$i]{$elem}++;
            # Update total, determines what keys to check later
            $total{$elem}++;
        }
    }

    my @output;
    # For keys that were seen:
    for my $k (keys %total) {
        # Skip unless count was defined in each sublist / multiset
        my @values = map { $counts[$_]{$k} } 0..$#counts;
        next unless @counts == grep { $_ } @values;
        # Number of elems in result is the min of elems in each multiset input
        my $min = min @values;
        push @output, ($k) x $min;
    }
    return @output;
}

if ( __FILE__ eq $0 ) {
    # Random input
    my @lists = map {
        [ map { int rand 5 } 0..1+int rand 10 ]
    } 0..1+int rand 5;
    dump @lists;

    my @res = intersect @lists;
    dump @res;
}
