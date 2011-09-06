#!/usr/bin/perl -W
use strict;

my $D = 0;
sub printd { if ($D) { my(@str) = @_ ? @_ : ""; print(@str); } }
sub max { my ($a, $b) = @_; ($a > $b) ? $a : $b; }

sub kmp {
	my ($text, $pattern) = @_;
	my ($n, $m) = (length($text), length($pattern));
	my @border;
	my @hits;
	my ($i, $j) = (0, 0);
	my $comparisons = 0;

	# compute borders
	@border[0..1] = (-1, 0);
	for (my ($i, $j) = (0, 2); $j <= $m; $j++, $i++, $border[$j] = $i) {
		while ($i >= 0 && 
			   substr($pattern, $i, 1) ne substr($pattern, $j - 1, 1)) {
			$i = $border[$i];
		}
	}

	printd "border: ", join(" ", @border), "\n";

	while ($i <= $n - $m) {
		printd "\ni: $i ";
		$comparisons++;
		while (substr($text, $i + $j, 1) eq substr($pattern, $j, 1)) {
			printd "j: $j, ";
			$j++;
			$comparisons++;
		}
		if ($j == $m) {
			printd "hit: $i\n";
			push(@hits, $i);
		}
		$i += $j - $border[$j];
		$j = max(0, $border[$j]);

	}
	printd "\n";
	print "comparisons: $comparisons\n";
	
	return @hits;

} # kmp

sub main {
	if (@ARGV != 2) {
		print "\nUsage: $0 <pattern> <text>\n";
		exit;
	}

	my ($p, $t) = @ARGV;

	my @hits = kmp($t, $p);

	my $i = 0;
	my $m = length($p);
	my $shift;
	my $REDBACK = "\e[0;41m";
	my $BLUEBACK = "\e[0;44m";
	my $NORMAL = "\e[0;0m";

	print "text:\n";
	for (my $j = 0; $j < @hits; $j++) {
		printd "\nhit: $j, i: $i\n";
		print substr($t, $i, $hits[$j] - $i);
		$i += $hits[$j] - $i;
		if (!defined($hits[$j + 1]) || $m < $hits[$j + 1] - $i) {
			$shift = $m;
		}
		else {
			$shift = $hits[$j + 1] - $i;
		}
		printd "shift: $shift\n";
		print $REDBACK, substr($t, $hits[$j], $shift) ,$NORMAL;
		$i = $hits[$j] + $shift;
	}
	# now print the rest, after the last hit
	print substr($t, $i), "\n";

	print "pattern: $p\n";
	print "text length: ", length($t), "\npattern length: $m\n";
	print "matches at: ", join(" ", @hits), "\n";

} # main()

main();
0;

