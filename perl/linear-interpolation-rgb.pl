#!/usr/bin/env perl
use v5.14;
use Devel::Comments;
use List::MoreUtils qw(minmax);
use Test::More qw(no_plan);

my @hotels = map +{ name => $_, available => int(rand 10) + int(rand 100) }, 'a'..'z';

my ($min,$max) = minmax map { $_->{available} } @hotels;
for (@hotels) {
    my $extent = ($_->{available} - $min) / ($max - $min);
    $_->{color} = [ int 255 * (1 - $extent), int 255 * $extent, 0 ];
}

# Sort to check. Red will be monotonically decreasing, green monotonically increasing
@hotels = sort { $a->{available} <=> $b->{available} } @hotels;
### @hotels
### $min
### $max

ok is_sorted();
ok ! is_sorted(3,2,1,1);
ok   is_sorted(1,1,2,3);
# Sorted by AV
ok is_sorted(map { $_->{available} } @hotels);
# Green increasing
ok is_sorted(map { $_->{color}[1] } @hotels);
# Red decreasing
ok is_sorted(reverse map { $_->{color}[0] } @hotels);

sub is_sorted {
    my $prev;
    for (@_) {
        $prev && $prev > $_ && return 0;
        $prev = $_;
    }
    return 1;
}
