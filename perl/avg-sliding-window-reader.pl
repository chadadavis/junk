#!/usr/bin/env perl
use v5.14;
$| = 1;
# A window of values over time
my @window;
# Limit the window to a certain time interval (s)
my $window_width = 50;
my $window_start;
my $window_end;
# Count the running average (to later aggregate)
my $count = 0;
my $sum = 0;
my $avg;
# use Devel::Comments;
my %ids;
while (<>) {
    chomp;
    # Whether we're reading directly from a producer or an aggregator, use the sum,count format.
    my ($ts, $val, $rec_count, $id, @rest) = split /\s+/;
    $window_start ||= $ts;
    $window_end = $ts;
    # Do truncation first, in case stream delays
    my $n_truncated = 0;
    while ( @window > $window_width ) {
        my $t   = shift @window or last;
        $sum   -= $t->[1];
        $count -= $t->[2];
        $window_start = @window ? $window[0]->[0] : $window_end;
        $n_truncated++;
    }
    my $width = sprintf "%3d", $window_end - $window_start + 1;
    ### $n_truncated
    ### $window_start
    ### $window_end
    ### $width

    # Record sum and the number of values it represents
    push @window, [ $ts, $val, $rec_count];

    $sum   += $val;
    $count += sprintf "%10d", $rec_count;
    $ids{$id} += $rec_count;
    $avg    = sprintf "%5d", $sum / $count;
    ### $sum
    ### $count
    ### $avg

    # Without sleep(), this Will be biased toward the faster producer (due to double counting)
    sleep 1;
    say join("\t", $window_end, $sum, $count, $$, "avg:$avg", "width:$width", "start:$window_start", "end:$window_end", "lastid:$id",  %ids, );

}
