#!/usr/bin/perl
$n = $ARGV[0] || (print("Fibonacci: "), <STDIN>);
print fib($n,1,1), "\n";
sub fib {
	my ($n, $v, $l) = @_;
	return $l if $n <= 2; 
	return fib($n -1, $l, $v + $l);
}
