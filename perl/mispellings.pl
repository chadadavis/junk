#!/usr/bin/env perl

=head1 SYNOPSIS

Given a word and a number of allowed misspellings, determine all possible misspelled words.

A misspelling is any one of:
* Inserting a character that does not belong
* Removing a character that does belong
* Replacing a correct character with an incorrect character

Assume the alphabet is ASCII and case-insensitive.

E.g. mispellings of "hat" include:
* cat
* bat
* at
* hats
But not "flat" as that would be two mispellings in "hat" (replacing the "h" with "l" and inserting an extra "f")


=head1 DESCRIPTION

If it's N chars, then it's the number of permutations of N+2 things (+2 since
there might be a leading insertion or a trailing insertion. For every other
position, choose either to keep the char $c, or choose to modify it by
selecting from the set [a..z,''] minus the set $c (i.e. all posible chars,
plus empty char, but not counting the original char (because you need to know
if it's counted as a change or not).

Followup: do the same for arbitrary values of N

Followup: how do you know that when N % 2 == 0 that you haven't reproduced the original:

E.g. N=2
"paris" => "pbris" => "paris" (2 operations, but re-created the original)

=cut

use v5.14;
use strict;
use warnings;
no warnings 'experimental::autoderef';
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

my $word = lc <>;
chomp $word;
my $n = <>;
chomp $n;

my @p = misspell($word);
### @p;

sub misspell {
    my ($word, $n) = @_;

    state $alphabet = [ undef, 'a' .. 'z' ];

    my @letters = split '', $word;
    my @possible;
    for my $l (@letters) {
        push @possible, $alphabet;
        push @possible, [ grep { $_ && $l ne $_ } @$alphabet ];
    }
    push @possible, $alphabet; # For potential trailing insertion
    ### @possible

}
