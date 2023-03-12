#!/usr/bin/env perl
use v5.14;
use List::Util qw(sum);
use Data::Dump qw(dump);

sub roll ($) {
    my ($config) = @_;
    # Sum of possible values, for later normalization
    my $sum = sum(values %$config);
    my $rand = rand();
    my $thresh = 0;
    # Sort to have the keys in consisten order per sub call
    for my $key (sort keys $config) {
        $thresh += $config->{$key} / $sum;
        return $key if $rand < $thresh;
    }
}

my %counts;
my @capacity = ( (100) x 4, (500) x 3, (1000) x 1 );
my $config;
my $i = 1;
while (my ($k, $v) = each @capacity) {
    $config->{$i++} = $v;
}
for (1..2900) {
    my $res = roll($config);
    $counts{$res}++;
}
for my $k (sort keys %counts) {
    say "$k => $counts{$k}";
}
