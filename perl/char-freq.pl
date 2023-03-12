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

my $char_stream = "ycarzkiucatuifeadogfxvcatdwzq";
my $words = ["car", "dog", "cat", "hat"];

sub count_words {
    my ($words, $stream) = @_;
    my $exp = join '|', @$words;
    my $counts;
    while ($stream =~ /($exp)/g) {
        say("Found:$1");
        $counts->{$1}++;
    }
    return $counts;
}


use Devel::Comments '####';
#### <here>
#### counts: count_words($words, $char_stream)
