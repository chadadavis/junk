#!/usr/bin/env perl

=head1 SYNOPSIS



=head1 DESCRIPTION

write a function that gets a filename/file descriptor for a dictionary file
and a list of chars and returns the biggest word that contains all the chars
in the list. example: '/usr/share/dict/english.dict' ['a','b','c'] =>
abacadabra


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

my @chars = split /\s+/, scalar <>;
### @chars
my $max_str = '';
my $max_n = 0;
outer:
while (my $l = <>) {
    chomp $l;
    next if $max_n && $max_n > length $l;
    my %current;
    $current{$_} = 1 for split '', $l;
    for my $c (@chars) {
        unless ($current{$c}) {
            next outer;
        }
    }
    $max_n = length $l;
    $max_str = $l;
    ### $max_str
}
say "$max_n\t$max_str";
