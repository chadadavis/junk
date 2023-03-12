#!/usr/bin/env perl
use v5.14;
use Time::HiRes qw(usleep);
use POSIX qw(strftime);
$| = 1;
# Give each producer a bias
my $base = 10 * int rand 100;
while (1) {
    # Generate some random samples representing request processing time (200ms-1200ms, avg 700ms)
    my $request_time = $base + int rand 1000;
#     my $timestamp = strftime "%F %T", localtime();
    # Use seconds, to avoid date parsing
    my $timestamp = time();
    # Simulate that this is not aggregated, each record is the sum of 1 requests
    my $count = 1;
    say join "\t", $timestamp, $request_time, $count, $$, '...';
    # Simulate some kind of request rate (sleep between 0 ms - 1000 ms)
    usleep 1000 * int rand 1000;
}
