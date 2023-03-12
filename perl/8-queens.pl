#!/usr/bin/env perl

use v5.14;
use Data::Dump qw(dump);
use Devel::Comments;

my $calls = 0;
my $n_queens = 0;
sub place {
    my ($queens, $rows, $cols) = @_;
    ### $calls
    say join ' , ', map { "@$_" } @$queens;
    exit if @$queens == 8;
    $calls++;
    my @r = @$rows;
    my $r = shift @r;
    col:
    for (my $i = 0; $i < @$cols; $i++) {
        my $c = $cols->[$i];
        my $clashed = 0;
        for my $q (@$queens) {
            if ( abs($q->[0] - $r) == abs($q->[1] - $c) ) {
                say "$r,$c clashes with $q->[0], $q->[1]";
                $clashed = 1;
                next col;
            }
        }
        if (! $clashed) {
            my @c = @$cols;
            splice @c, $i, 1;
            place([ @$queens, [$r,$c] ], \@r, \@c);
        }
    }
}

place([], [ 0..7 ], [ 0..7 ]);

