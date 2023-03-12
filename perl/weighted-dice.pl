#!/usr/bin/env perl

=head1 SYNOPSIS



=head1 DESCRIPTION



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

# Test::Most vs Test::More

use Test::Most qw(defer_plan);

sub roll ($) {
    my ($config) = @_;
    my $rand = rand();
    my $thresh = 0;
    for my $key (sort keys $config) {
        $thresh += $config->{$key};
        return $key if $rand < $thresh;
    }
}

my %counts;
my $config = { 1 => .1, 2 => .5, 3 => .1, 4 => .1, 5 => .1, 6 => .1 };
for (1..1000) {
    my $res = roll($config);
    $counts{$res}++;
}
say join ",", %counts;
