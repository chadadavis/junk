#!/usr/bin/env perl

use strict; 
use warnings;

use Utils;
use Data::Dumper;

my $set = genset(10, 10);

print Dumper $set, "\n";

my $diff = pairs($set);

print Dumper $diff, "\n";

my $fdiff = binning($diff);

print Dumper $fdiff, "\n";

################################################################################

sub pairs {
    my ($set) = @_;
    my $n = @$set;
    my $diff = [];
    for (my $i = 0; $i < $n; $i++) {
        $diff->[$i] = [];
        for (my $j = $i+1; $j < $n; $j++) {
            $diff->[$i][$j] = sumstr(edit($set->[$i], $set->[$j]));
            $diff->[$j][$i] = $diff->[$i][$j];
        }
    }
    return $diff;
}

sub binning {
    my ($array2d) = @_;
    my $n = @$array2d;
    for (my $i = 0; $i < $n; $i++) {
        for (my $j = $i+1; $j < $n; $j++) {
            $array2d->[$i][$j] = substr($array2d->[$i][$j], 0, 1);
        }
    }
    return $array2d;
}

sub genset {
    my ($n, $len) = @_;
    my $things = [];
    for (my $i = 0; $i < $n; $i++) {
        push @$things, gen($len);
    }
    return $things;
}

sub testadd {

    my $a = gen(10);
    my $b = gen(10);
    my $c = edit($a, $b);
    print join "\n", $a, $b, $c, sumstr($c), "\n";
}

sub gen {
    my ($len, $alphabet) = @_;
    $alphabet ||= [ 0..9 ];
    # String of random values from alphabet
    my $str;
    for (my $i = 0; $i < $len; $i++) {
        $str .= $alphabet->[int(rand(scalar @$alphabet))];
    }
    return $str;
}

# The string of the manhatten distance, for each dimension of the string
sub edit {
    my ($a, $b) = @_;
    die unless length($a) == length($b);
    my $edit;
    for (my $i = 0; $i < length($a); $i++) {
        $edit .= abs(substr($a,$i,1) - substr($b,$i,1));
    }
    return $edit;
}

# Sum of values in a string
sub sumstr {
    my ($s) = @_; 
    my @a = split //, $s;
    return sum(@a);
}
