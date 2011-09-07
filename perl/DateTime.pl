#!/usr/bin/env perl
use Modern::Perl;
use DateTime;

# Default time zone is 'floating' time zone of the iCal standard

my $start = time;
my $start_date = DateTime->from_epoch(epoch=>$start);

use Time::HiRes;
# Floating second sleep
my $sleep = 1 + rand 2;
say "Sleeping $sleep";
sleep $sleep;

my $stop_date = DateTime->now;

# ISO8601 looks like $dt->ymd('-') . 'T' . $dt->hms(':')
say "Started  $start_date";
say "Finished $stop_date";

say 'Standard dash format ' . $stop_date->ymd;
say 'Standard EUR  format ' . $stop_date->dmy('.');

my $duration = $stop_date - $start_date;
say 'Duration ' . $duration->hms; # Nope ...
