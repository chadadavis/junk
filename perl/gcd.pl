#!/usr/bin/perl -w

use strict;

sub abort;

if (@ARGV != 2) { 
	abort();
}

my($a, $b) = @ARGV;
($a, $b) = (int($a), int($b));

if ($a == 0 || $b == 0) {
	abort();
}

my($x, $y, $k, $l, $u, $v, $q);

if ($b > $a) {
	($a, $b) = ($b, $a);
}

($x, $y, $k, $l, $u, $v) = ($a, $b, 0, 1, 1, 0);

while ($x % $y) {
	$q = int($x / $y);
	($x, $y, $u, $v, $k, $l) = ($y, $x % $y, $k, $l, $u - $k * $q, $v - $l * $q);

}

print "gcd: ", $y, " = ", 
	$k, " * ", $a, " + ", $l, " * ", $b, " = ", $a * $k + $l * $b;
print "\n";

sub abort
{
	print "Provide two non-zero integers as input. \n";
	exit;
}
