#!/usr/bin/env perl

=head1 SYNOPSIS

https://docs.google.com/a/booking.com/document/d/1ASGoMIZAmYLXdo9mgtD2n4zMKLf58w1U_P_ZUeuMZQU

=head1 DESCRIPTION



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
use Devel::Comments;
use Test::More qw(no_plan);
use Getopt::Long;

my @cases = (
    {
        expect => [ qw(c) ],
        states => {
            a => [ qw(a b c d e f g) ],
            b => [ qw(a b c d e f g) ],
            c => [ qw(b c d) ],
            d => [ qw(a b c d e f g) ],
            e => [ qw(a b c d e f g) ],
            f => [ qw(a b c d e f g) ],
            g => [ qw(a b c d e f g) ],
        },
    },
    {
        expect => [ qw(d e) ],
        states => {
            a => [ qw(a b c) ],
            b => [ qw(a b c) ],
            c => [ qw(a b c) ],
            d => [ qw(a b c d e) ],
            e => [ qw(a b c d e) ],
        },
    },
    # Split-brain case, i.e. it's draw/tie (return all of them?)
    {
        expect => [ qw(a b c d) ],
        states => {
            a => [ qw(a b) ],
            b => [ qw(a b) ],
            c => [ qw(c d) ],
            d => [ qw(c d) ],
        },
    },
);

my $states;
for my $test (@cases) {
    # Set global state for test case
    $states = $test->{states};
    is_deeply broken_nodes(keys %$states), $test->{expect};
}

# API
sub list_members {
    my ($source) = @_;
    return $states->{$source};
}

sub broken_nodes {
    my (@nodes) = @_;
    my %sets;
    for my $n (@nodes) {
        # Encocde cluster state
        my $members = list_members($n);
        my $key = join('|', @$members);
        $sets{$key}{$n} = 1;
    }
    #### %sets
    # Majority vote wins
    my @by_agreement = sort {
        scalar(keys($sets{$b})) <=> scalar(keys($sets{$a}))
    } keys %sets;
    #### @by_agreement;
    my $winner = shift @by_agreement;
    my %others;
    for my $problematic_set (@by_agreement) {
        $others{$_} = 1 for keys $sets{$problematic_set}
    }
    #### %others
    return [ sort keys %others ]
}
