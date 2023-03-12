#!/usr/bin/env perl

=head1 SYNOPSIS

Given a list of resources and which processes (might) have them (currently) locked, detect deadlock.

=head1 DESCRIPTION

Deadlock happens when a system cannot proceed due to the fact that some
processes are waiting for resources that are locked by other processes, and
vice versa. E.g. if process 135 has resource A locked and is waiting for
resource B, but process 672 has resource B locked and is now waiting for
resource A, then the system is in deadlock, since both processes will wait
indefinitely.

Implement a function is_deadlock() that will return true when this is the case.

Assume you have input in the form of a list of locks, one per line. So, the case above would have the following input:

1 A 135
0 B 135
1 B 672
0 A 672

The first column is whether the lock is already attained. If it's 1 (true),
then the lock is already granted for the resource listed in the second
column. The third column is the ID of the process holding / requesting the
lock. If the lock is not yet granted (first field is 0 / false), then it's
still pending / not yet granted.

Locks are exclusive (i.e. only one process can hold a lock for a single
resource). However any number of processes might be waiting for a lock on
certain resource.

TODO: 

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

my @list = <>;
chomp @list;

# Represent as a directed graph

# Test case: if 366 locks both A and B, and 677 is waiting on both A and B,
# that's not deadlock (testing directedness)

# Follow up: identify the content of the deadlock, i.e. not just boolean, but the dependencies

# Test case: disconnected graph

# Test case: detect multiple deadlocks

# Test case: multiple deadlocks in a single component?

# Test case: when a process is waiting for multiple resources, and/or has multiple locks

my %dag;
for my $e (@list) {
    next if $e =~ /^\s*$/;
    my ($direction, $resource, $process) = split ' ', $e;
    if ($direction) {
        $dag{$resource}{$process} = 1;
    }
    else {
        $dag{$process}{$resource} = 1;
    }
}
### %dag

my @deadlocks = is_deadlock(%dag);
### @deadlocks

sub is_deadlock {
    my %dag = @_;
    my @nodes = keys %dag;
    my @deadlocks;

    # For each connected component of the (potentially disconnected) graph
    my %processed;
  COMPONENT:
    while (my $n = shift @nodes) {
        next if $processed{$n};
        $processed{$n} = 1;
        ### %processed

        # For each node along a path, track the edges, BFS
        my %edges;
        my @todo = ($n);
      PATH:
        while (my $t = shift @todo) {
            $processed{$t} = 1;
            if ($edges{$t}) {
                push @deadlocks, { %edges };
                ### deadlock: %edges;
                # This skips the possibility of one process being in two deadlocks
                # But maybe that's only theoretically possible (?)
                next COMPONENT;
            }
            push @todo, keys %{ $dag{$t} };
            $edges{$t}{$_} = 1 for keys %{ $dag{$t} };
            ### %edges
            ### @todo
            sleep 1;
        }
    }
    return @deadlocks;
}




