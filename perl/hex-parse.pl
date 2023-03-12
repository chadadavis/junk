#!/usr/bin/env perl
use Modern::Perl;
use Test::More;

my @vals = (0..9,'a'..'f');
my %vals = ( map { $vals[$_] => $_ } 0..$#vals );

my $in = shift;
# Or a rand hex number of n digits (n/2 bytes)
$in //= join('', map { sprintf "%x", int rand 16 } 0..8);

my $total = 0;
my @split = split //, $in;
for (my $i = 0; $i < @split; $i++) {
    $total += (16**$i) * $vals{$split[$#split-$i]};
}
is sprintf("%x", $total), $in, "0x$in == $total";
