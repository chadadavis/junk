#!/usr/bin/env perl

=head1 SYNOPSIS

Simple search through descriptions of cities by keywords that they might contain.

=head1 DESCRIPTION



=cut

use v5.14;
use strict;
use warnings;
use autodie;
use Log::Any qw($log);
use Carp qw(cluck confess);
$SIG{__DIE__} ||= \&confess;
use IO::All;
use Data::Dump qw(dump);
use Devel::Comments '#####';
use Test::More qw(no_plan);
use Getopt::Long;

my $dests = [
    {
        name => "Istanbul, Turkey",
        description =>
            "The city of Istanbul offers a trip unlike any other, encompassing
            Asian and European cultures to create a heady mix of art, music
            and food.",
    },
    {
        name => "Dubai, UAE",
        description =>
            "Cosmopolitan and architecturally stunning Dubai is a truly global
            city.",
    },
    {
        name => "Stockholm, Sweden",
        description =>
            "Modern Stockholm is uniquely made up of 30% waterways and another
            30% of parks and green spaces.",
   },
    {
        name => "Zurich, Switzerland",
        description =>
            "As the financial capital of Switzerland and headquarters of
            several large companies including FIFA, there’s a good chance that
            if you’re in Zurich you’re here on business.",
    },
    {
        name => "Tokyo, Japan",
        description =>
            "The capital of Japan, Tokyo is a vast, teeming metropolis nestled
            between mountains and Tokyo Bay.",
   },
];

build_inverted_index($dests);
build_trie($dests);

for my $method ( (\&search_inverted, \&search_regex, \&search_trie) ) {
    is_deeply $method->('of'), [0,2,3,4];
    is_deeply $method->('modern'), [2];
    is_deeply $method->('momo'), [];
    is_deeply $method->('capital'), [3,4];

}

exit;

################################################################################

sub _canonicalize {
    my ($str) = @_;
    $str =~ s/[^a-zA-Z ]//g;
    $str =~ s/\s+/ /g;
    $str = lc $str;
    return $str;
}

# 6 mins (on paper)
sub search_regex {
    my ($term) = @_;
    $term = _canonicalize($term);
    my @matches;
    for (my $i = 0; $i < @$dests; $i++) {
        my $d = $dests->[$i]{description};
        $d = _canonicalize($d);
        my ($match) = $d =~ /($term)/;
        push(@matches, $i) if $match;
    }
    return \@matches;
}

# 10 mins (on paper)
{
    my $_index;
    sub build_inverted_index {
        my ($docs) = @_;
        for (my $doc_id = 0; $doc_id < @$docs; $doc_id++) {
            my @tokens = split ' ', $docs->[$doc_id]{description};
            for my $token (@tokens) {
                $token = _canonicalize($token);
                $_index->{$token}{$doc_id}++
            }
        }
        return $_index;
    }

    sub search_inverted {
        my ($term) = @_;
        $term = _canonicalize($term);
        # sort by doc_id, for determinism
        return [ sort keys( %{ $_index->{$term} || {} } )  ];
    }
}

# 23 mins (on paper) + 30 mins in debugger
{
    my $trie;
    sub build_trie {
        my ($dests) = @_;
        $trie //= { children => {}, docs => [] };
        for (my $i = 0; $i < @$dests; $i++) {
            my $desc = _canonicalize($dests->[$i]{description});
            my @tokens = split ' ', $desc;
            for my $t (@tokens) {
                # Insert from the root
                _insert($trie, $t, $i);
            }
        }
        return $trie;
    }

    sub _insert {
        my ($parent, $token, $i, $level) = @_;
        $level //= 1;
        my $label = substr($token, 0, 1);
        my $child = $parent->{children}{$label} ||= { children => {}, level => $level };
        $child->{docs}{$i}++;
        my $rest = substr($token, 1);
        return unless length($rest) > 0;
        # Insert remaining string starting from this child
        _insert($child, $rest, $i, $level + 1);
        return;
    }

    sub search_trie {
        my ($keyword, $node) = @_;
        $node //= $trie;
        $keyword = _canonicalize($keyword);
        my $label = substr($keyword, 0, 1);
        my $child = $node->{children}{$label};
        return [] unless $child;
        if (length($keyword) > 1) {
            return search_trie(substr($keyword, 1), $child);
        }
        else {
            my $docs = $node->{docs};
            return [ sort {
                # returns document matches sorted by token frequency
                # $docs->{$b} <=> $docs->{$a}
                # or just by document id:
                $a cmp $b
            } keys %$docs ];
        }
    }
}

