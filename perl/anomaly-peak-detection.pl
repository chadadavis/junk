#!/usr/bin/env perl

=head1 SYNOPSIS



=head1 DESCRIPTION



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
use Devel::Comments '#####';
use Test::More qw(no_plan);
use Getopt::Long;
use Term::ReadKey;

# Generate enough data to plot on screen size, but leave a header
my ($wchar, $hchar) = GetTerminalSize();
my @data = map { $wchar + rand($wchar) - $wchar / 2 } 1 .. ($hchar - 5);

# Keep a rolling window of recent data
my $last_n = 10;
my @last;
# Detect a change more than N stddevs from the mean
my $thresh = 1.5;

# Keep track of where we've had peaks (to plot them)
my %peak;

my $avg = 1;
my $variance = 1;
my $min = $data[0];
my $max = $data[0];
for (my $i = 0; $i < @data; $i++) {
    $min = $data[$i] if $data[$i] < $min;
    $max = $data[$i] if $data[$i] > $max;
    # Scale to current range
    my $v = 0 + ($data[$i] - $min) * $wchar / ( ($max - $min) || 1 );

    push @last, $data[$i];
    splice @last, 0, 1 if @last > $last_n;

    # Update stats over rolling window
    my $avg_prev = $avg;
    $avg = $avg + ($data[$i] - $last[0]) / $last_n;
    $variance +=  ($data[$i] - $last[0]) * ($data[$i]-$avg+$last[0]-$avg_prev) / ($last_n - 1);
    my $stddev = sqrt($variance);
    my $z = ($data[$i] - $avg) / $stddev;

    if ( $z > $thresh || $z < - $thresh) {
        $peak{$i} = $z;
        say "<-- Alarm. z: " . $peak{$i} if $peak{$i};
    }
    say "x" x $v;
}

