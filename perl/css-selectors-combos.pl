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

sub combos {
    my $target = pop;
    my @answers;
    for (my $i=0; $i<2**@_; $i++) {
        my @s = reverse map { $i & 2**$_ ? $_[-($_+1)] : () } 0..$#_;
        my $s = join(' ', @s, $target);
        push @answers, $s;
    }
    return @answers;
}
my @words = qw(body div p img);
say $_ for combos(@words);
