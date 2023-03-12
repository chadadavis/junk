#!/usr/bin/env perl

=head1 SYNOPSIS



=head1 DESCRIPTION

We log for each pageview the response time (milliseconds), like:

956 /index.html?sid=3478432adf62
1562 /searchresults.html?city=27788
156 /faq.html
1667 /index.html?aid=167783
5663 /searchresults.html?country=nl
...

Given this input, return top 10 slowest pages.

* Note that /index.html?sid=2472757 and /index.html?sid=1647868 are the same page
* Use the average time for each page
* Print the pages in order, slowest first

See also:
Top k biggest numbers from a file
http://www.glassdoor.com/Interview/Booking-com-Interview-RVW2626143.htm

Followup:
Since the average is subject to extreme outliers (e.g. temporary outage), use the median


=cut

use v5.14;
use strict;
use warnings;
use autodie;
use open qw(:utf8 :std);

use Log::Any qw($log);
use Carp qw(cluck);
$SIG{__DIE__} ||= \&confess;

use IO::All;
use Data::Dump;
use Devel::Comments;
use Test::More qw(no_plan);
use Getopt::Long;

my %sums;
my %counts;

while (my $line = <>) {
    chomp;
    my ($time, $url) = split /\s+/, $line;
    my ($page) = split /\?/, $url;
    $sums{$page} += $time;
    $counts{$page} ++;
}

