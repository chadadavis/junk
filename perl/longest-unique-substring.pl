#!/usr/bin/env perl

=head1 SYNOPSIS



=head1 DESCRIPTION

From nemanja.djuric

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
# use Devel::Comments;
use Test::More qw(no_plan);
use Getopt::Long;

my $in = shift || 'abcdecdefgrtg';
my $expect = 'cdefgrt';
is longest1($in), $expect;
is longest1('xx'), 'x';
is longest1('abcdbxxyz123'), 'xyz123';
is longest1('abcxxxabcde'), 'xabcde';
is longest1('abc'), 'abc';
is longest1('x'), 'x';
# is longest2($in), $expect;

# Sliding window grows / shrinks
sub longest1 {
    my ($str) = @_;
    #### $str
    return unless length $str;
    my @str =  split '', $str;
    my $start_max = 0;
    my $end_max = 0;
    # Current trial window (half-open interval)
    my $start = 0;
    my $end = 0;
    my %count;
    for (my $i = 0; $i < @str; $i++) {

        #### current: "[$start-$end) @{[ substr($str, $start, $end-$start) ]}"
        #### max: "[$start_max-$end_max) @{[ substr($str, $start_max, $end_max-$start_max) ]}"
        #### char: $str[$i]

        if ($end-$start > $end_max-$start_max) {
            #### Exceeded: "[ $start_max - $end_max )"
            $start_max = $start;
            $end_max = $end;
        }
        #### $start_max
        #### $end_max

        if ($count{ $str[$i] }++ ) {
            while ( $start < $end && $count{ $str[$i] } > 1 ) {
                #### pop: $str[$start]
                $count{ $str[$start++] }--;
                #### current: "[$start-$end) @{[ substr($str, $start, $end-$start) ]}"
            }
        }
        $end++;
    }
    if ($end-$start > $end_max-$start_max) {
        #### Exceeded: "[ $start_max - $end_max )"
        $start_max = $start;
        $end_max = $end;
    }

    return substr($str, $start_max, $end_max-$start_max);
}

