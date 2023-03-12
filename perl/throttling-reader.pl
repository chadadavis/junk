#!/usr/bin/env perl
use Modern::Perl;
use Data::Dump qw(dump);
use List::AllUtils;



while (<>) {
    my ($ip, $ua, $time_stamp, @other) = split ' ';
    my $epoch = $time_stamp; # convert as needed

    my $should_block = should_block($ip, $ua, $epoch);
    # Send to LRU cache accessible by webservers

}

sub should_block {
    state %requests;
    state $max_count = 500;
    state $max_age = 300;

    # Test
    my $ip = '1.1.1.1';
    my $epoch = '1406373365';

    $requests{$ip} //= [];
    # Expire older requsts
    my $now = time;
    my $i = 0;
    for (; $i < @{$requests{$ip}} && $requests{$ip}[$i] < $now - $max_age; $i++) {
    }
    if ($i > 0) {
        say "Purge: $ip, 0, $i-1";
        splice($requests{$ip}, 0, $i-1);
    }
    push $requests{$ip}, $epoch;
    return @{$requests{$ip}} > $max_count ? 503 : 0;
}

sub expire_ips {

}
