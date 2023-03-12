#!/usr/bin/env perl

=head1 SYNOPSIS

Record transitions through the site.

=head1 DESCRIPTION

Approaches:

* Sequence of page types, then mulitiple alignment on them

* Markov chains, liklihood of transitions between page types (depending page type segments, and user segments)

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
use Devel::Comments qw(####);
use Test::More qw(no_plan);
use Getopt::Long;


my %pages = (
    'lp' => [ qw(x y z) ],
    'x' => [ qw(y z a) ],
    'a' => [ qw(x bp) ],
    'z' => [ qw(y bp) ],
);

# Returns ArrayRef of targets of links
sub get_links {
    my ($source) = @_;
    return $pages{$source} || [];
}

sub ways_to_get_there {
    my ($start, $end) = @_;
    my @q = get_links($start);
    while (my $next = shift @q) {
        
    }
}
