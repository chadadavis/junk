#!/usr/bin/env perl

=head1 SYNOPSIS

Activate the most recent index


=head1 DESCRIPTION

A cluster of search engine machines regularly receives an updated index file. When they have a new index, they should start using it.

Sometimes this process fails, however. It's not guaranteed that all nodes have the same indexes. The cluster only works correctly if everyone is using the same index version.

Write a function that will put all of the machines on the most recent index that is available to all nodes.

API:

@host_names = get_hosts() # an array of strings

@indexes = get_indexes($one_hostname) # an array of strings

$is_success = activate_index($one_hostname, $one_index_name) # boolean

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
use Devel::Comments qw(#####);
use Test::More qw(no_plan);
use Getopt::Long;

my @hostnames = map { 'host_' . sprintf('%02d', $_) } 1..10;

my @indexes = map { 20150101 + $_ } 0 .. 10;
##### @indexes

my %available = map {
    my $host = $_;
    my %rand = map { $indexes[ rand 10 ] => 1 } 0 .. 20;
    $host => [ keys %rand ];
} @hostnames;

##### %available;

my %found;
for my $host ( @hostnames ) {
    my $indexes = $available{$host};
    $found{$_}++ for @$indexes;
}
##### %found

my @shared = grep { $found{$_} == @hostnames } keys %found;
##### @shared

my $max = shift @shared;
for (@shared) {
    $max = $_ if $_ > $max;
}

##### $max
