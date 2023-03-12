#!/usr/bin/env perl

=head1 SYNOPSIS

Given some scores, rank hotels for the search results page. If a hotel is
'preferred' it should appear before hotels that are not
'preferred'. Otherwise, the ranking should depend on the hotel score.

=head1 DESCRIPTION

From jure.merhar

E.g. imagine we simply negate the hotel rank to identify preferred hotels:

Preferred hotels get a negative sign:
[ 1, -2, -3, 4, -5, 6, -7, -8, 9 ]

Final ranking would be:
[ -2, -3, -5, -7, -8, 1, 4, 6, 9 ]

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
use Devel::Comments;
use Test::More qw(no_plan);
use Getopt::Long;

my @input = ( 2, 5, -2, 1, -3, -1, 3 );
my @expect = (-2, -3, -1, 2, 5, 1, 3);
my @got;

@got = sub1(@input);
is_deeply \@got, \@expect;

@got = sub2(@input);
is_deeply \@got, \@expect;

sub sub1 {
    my @in = @_;
    my @indexes = sort {
        ($in[$a] >= 0 ? 1 : -1) <=> ($in[$b] >=0 ? 1: -1)
        || $a <=> $b
    } 0..$#in;
    my @out = @in[@indexes];
    return @out;
}

sub sub2 {
    my @in = @_;
    my @neg;
    my @pos;
    for (@in) {
        if ($_ < 0) {
            push @neg, $_;
        }
        else {
            push @pos, $_;
        }
    }
    return @neg, @pos;
}
