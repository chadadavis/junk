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
use Devel::Comments '####';
use Test::More qw(no_plan);
use Getopt::Long;

sub rand_bin_tree {
    my ($count, $range) = @_;
    $count //= 10;
    $range //= 100;
    my $root = { v => int rand $range };
    my $cur;
    # insert one new node
    while (--$count > 0) {
        $cur = $root;
        #### $count
        #### $root
        while ($cur) {
            #### $cur
            my $child = rand() < 0.5 ? 'l' : 'r';
            if ( $cur->{$child} ) {
                $cur = $cur->{$child};
            }
            else {
                $cur->{$child} = { v => int rand $range };
                last;
            }
        }
    }
    return $root;
}

sub find_in_tree {
    my ($cur, $target) = @_;
    my @q;
    push @q, $cur;
    while ( $cur = shift @q ) {
        return 1 if $cur->{v} == $target;
        push @q, $cur->{l} if $cur->{l};
        push @q, $cur->{r} if $cur->{r};
    }
    return;
}

my $t = rand_bin_tree(10, 10);
#### $t

my $target = int rand 10;
my $res = find_in_tree($t, $target);
#### $target
#### $res
