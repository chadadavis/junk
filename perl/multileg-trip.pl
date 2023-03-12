#!/usr/bin/env perl

=head1 SYNOPSIS

Determine if a user has a booked a multi-legged trip.

=head1 DESCRIPTION

What if the records are not sorted?

What if the legs are in e.g. east Amsterdam and then west Amsterdam?

What if two bookings are in parallel, same hotel, same dates? What if not entirely contained?

What if there's a one-day gap between bookings? Still multi-leg?

If you have some info that the trip is multi-leg, what can you do with that?

Assuming you've done this for all users, can you use that to make a better website for new users?

Given a list of cities and hotels and availability, suggest multi-leg trips?

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
use List::Util qw(max);
use Devel::Comments qw(#####);
use Test::More qw(no_plan);
use Getopt::Long;

# Input
my @bookings = (
    [ 20150202, 20150205 ],
    [ 20150205, 20150210 ],
    [ 20150612, 20150615 ],
    [ 20150818, 20150822 ],
    [ 20150822, 20150828 ],
);

# Expected output
my $d;
$d = [
    [
    [ 20150202, 20150205 ],
    [ 20150205, 20150210 ],
    ],
    [
    [ 20150612, 20150615 ],
    ],
    [
    [ 20150818, 20150822 ],
    [ 20150822, 20150828 ],
    ],
];
is_deeply trip_legs(\@bookings), $d;
is_deeply trip_legs_unsorted(\@bookings), $d;

push @bookings, [ 20150819, 20150820 ];

$d = [
    [
    [ 20150202, 20150205 ],
    [ 20150205, 20150210 ],
    ],
    [
    [ 20150612, 20150615 ],
    ],
    [
    [ 20150818, 20150822 ],
    [ 20150819, 20150820 ],
    [ 20150822, 20150828 ],
    ],
];
is_deeply trip_legs(\@bookings), $d;
is_deeply trip_legs_unsorted(\@bookings), $d;

push @bookings, [ 20150616, 20150617 ];

$d = [
    [
    [ 20150202, 20150205 ],
    [ 20150205, 20150210 ],
    ],
    [
    [ 20150612, 20150615 ],
    ],
    [
    [ 20150616, 20150617 ],
    ],
    [
    [ 20150818, 20150822 ],
    [ 20150819, 20150820 ],
    [ 20150822, 20150828 ],
    ],
];

is_deeply trip_legs(\@bookings), $d;
is_deeply trip_legs_unsorted(\@bookings), $d;

push @bookings, [ 20150618, 20150620 ];

is_deeply trip_legs(\@bookings, 1), [
    [
    [ 20150202, 20150205 ],
    [ 20150205, 20150210 ],
    ],
    [
    [ 20150612, 20150615 ],
    [ 20150616, 20150617 ],
    [ 20150618, 20150620 ]
    ],
    [
    [ 20150818, 20150822 ],
    [ 20150819, 20150820 ],
    [ 20150822, 20150828 ],
    ],
];


exit;

sub trip_legs {
    my ($d, $thresh) = @_;
    $thresh //= 0;
    @_ = @$d;
    # Sort by first coord
    @_ = sort { $a->[0] cmp $b->[0] } @_;
    my @groups;
    my $curr = shift @_;
    my @group = ($curr);
    my ($c0, $c1) = @$curr;
    while (my $t = shift @_) {
        my ($t0, $t1) = @$t;
        if ($t0 > $c1 + $thresh) {
            # Close prev group
            push @groups, [ @group ];
            ($c0,$c1) = @$t;
            @group = ($t);
        }
        else {
            # Append to current group
            push @group, $t;
            $c1 = $t1 > $c1 ? $t1 : $c1;
        }
    }
    push @groups, [ @group ] if @group;
    return \@groups;
}

sub trip_legs_unsorted {
    my ($d) = @_;
    @_ = @$d;
    my %all;
    my %start;
    my %end;
    while (my $t = shift @_) {
        my $self = { range => $t };
        $all{ join '-', @$t } = $self;
        my ($t0, $t1) = @$t;
        if ( $end{$t0} ) {
            $end{$t0}{next} = $self;
        }
    }

    return [];

}
