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

my %pageview;
my %booking;
my %session;

sub log_pageview {
    my ($session_id, $page_id) = @_;
    $pageview{$page_id}++;
    $session{$session_id}{$page_id}++;
}

sub log_booking {
    my ($session_id) = @_;
    # Not counting duplicate pageviews per session toward booking
    for my $page_id ( keys %{ $session{$session_id} } ) {
        $booking{$page_id}++;
    }
}

sub get_conversion {
    my ($page_id) = @_;
    return 0 unless $pageview{$page_id};
    return $booking{$page_id} / $pageview{$page_id};
}
