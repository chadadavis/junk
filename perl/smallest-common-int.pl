#!/usr/bin/env perl

=head1 SYNOPSIS

* Get the smallest common integer from two lists
** Followup: smallest common integer from N lists
** Followup: M smallest common integers from N lists


=head1 DESCRIPTION



=cut

use v5.14;
use strict;
use warnings;
no warnings 'experimental::autoderef';
use autodie;
use open qw(:utf8 :std);
use Log::Any qw($log);
use Carp qw(cluck);
use IO::All;
use Data::Dump qw(dump);
use Devel::Comments;
use Test::More qw(no_plan);
use Getopt::Long;


my @lists;
my $n_lists;
my %uniq_all;
for (<>) {
    chomp;
    my @vals = split /\s+/;
    ### @vals
    my %uniq_this = map { $_ => 1 } @vals;
    $uniq_all{$_}++ for keys %uniq_this;
    $n_lists++
}

my @common = grep { $n_lists == $uniq_all{$_} } keys %uniq_all;
my @mins;
my $k = $n_lists;
for (@common) {
    _insertion_sort(\@mins, $_);
    splice @mins, $k;
}

sub _insertion_sort {
    my ($a, $val) = @_;
    for (my $i = 0; $i <= @$a; $i++) {
        if ($i == @$a || $val < $a->[$i]) {
            splice $a, $i, 0, $val;
            last;
        }
    }
    return $val;
}

say join(' ', @mins);
