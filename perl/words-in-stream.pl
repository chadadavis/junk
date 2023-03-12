#!/usr/bin/env perl

=head1 SYNOPSIS



=head1 DESCRIPTION

Count the occurences of a keyword in a stream of characters.

CharacterStream : Allows access to a steam of characters one at a time
+ next_char() # Returns next char in stream or \0 at the end of the stream

# Example:
stream = new MockCharacterStream("zcar zkiu catuife adogfxv cat dwzq")
String[] keywords = ["cat", "hat", "dog", "ate"]

result = count_keywords(stream, keywords)
# expected result: {
 cat: 2
 dog: 1
}

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
use Devel::Comments qw(####);
use Test::More qw(no_plan);
use Getopt::Long;

my %tests = (
    # More than a sigle target
    "Multiiple" => {
        in  => [ qw(agh4catdcatg cat) ],
        out => { cat => 2 },
    },
    # Check simple off-by-1 errors
    "Edges" => {
        in  => [ qw(alphakbetalahbeta alpha beta) ],
        out => { alpha => 1, beta => 2 },
    },
    # Overlapping of different targets
    "Combined" => {
        in  => [ qw(asgalphagalphat alpha aga) ],
        out => { alpha => 2, aga => 1 },
    },
    # This one tests correct resetting on single char
    "Invalid overlap" => {
        in  => [ qw(alpalphabeta alpha) ],
        out => { alpha => 1 },
    },
    # This one requires ability to have multiple simultaneous matches per target
    "Valid overlap" => {
        in  => [ qw(alphalpha alpha) ],
        out => { alpha => 2 },
    },
);

for my $test (sort keys %tests) {
    my $in  = $tests{$test}{in};
    my $out = $tests{$test}{out};
    is_deeply(stream_find(@$in), $out, $test);
}

sub stream_find {
    my ($content, @targets) = @_;

    my %matches = map { $_ => 0 } @targets;

    # Each target word is a current position (default 0) plus an array of chars
    @targets = map { [ 0, split '', $_ ] } @targets;

    # For each char in the stream
    for (my $i = 0; $i < length($content); $i++) {
        my $c = substr($content, $i, 1);

        #### @targets
        #### $i
        #### $c

        # Which matches should be advanced:
        for my $t ( @targets ) {
            # Reset all counters that are not still matching
            if ($c ne $t->[ $t->[0] + 1 ] ) {
                $t->[0] = 0;
            }
            # Allow to match from a new start / contiue matching
            if ($c eq $t->[ $t->[0] + 1 ] ) {
                $t->[0]++;
                if ( $t->[0] ==  $#$t ) {
                    $matches{ join '', @$t[ 1..$#$t ] }++;
                    $t->[0] = 0;
                    #### %matches
                }
            }
        }
    }
    return \%matches;
}
