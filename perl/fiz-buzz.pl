#!/usr/bin/env perl
use Modern::Perl;
my $limit = shift || 100;
for (my $i = 1; $i <= $limit; $i++) {
    printf "%3d ", $i;
    if ($i % 3 == 0) { print "Fizz" }
    if ($i % 5 == 0) { print "Buzz" }
    print "\n";
}
