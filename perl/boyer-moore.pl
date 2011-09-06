#!/usr/local/bin/perl -W

use strict;

my $D = 0;

sub printd
{
	if ($D) {
		my(@str) = @_ ? @_ : "";
		print(@str);
	}
}

sub bm {
	my ($text, $pattern) = @_;
 	my ($n, $m) = (length($text), length($pattern));
	my @hits;

	# build occ table, works from left to right, so the table ends up with
	# the right most of occurance of each char in the pattern
	my %occ;
	my @spattern = split(//, $pattern);
	for (my $i = 0; $i < @spattern; $i++) {
		$occ{$spattern[$i]} = $i;
	}
	printd "occ table: ", join(" ",%occ), "\n";

	# build suffix shift table
	# ...

	my $comparisons = 0;
	for (my ($i, $j) = (0, 0); $i <= $n - $m;) {
		printd "\ni: $i";
		$j = $m - 1; 
		$comparisons++; # counts the first comparison
		while ($j >= 0 && uc($spattern[$j]) eq uc(substr($text, $i + $j, 1))) {
			$comparisons++; # counts all remaining comparisons
			$j--;
		}
		if ($j < 0) {
			printd "\nhit: $i\n";
			push(@hits, $i);
exit();
#TODO the first method misses some tandem repeats
# the second method might run off the end of the text
#			$i++; 
#			$i += $m - $occ{substr($text, $i + $m, 1)};
		}
		else {
			printd " j: $j,";
			my $char = substr($text, $i + $j, 1);			
			if (!defined($occ{$char})) {
				$i += $j + 1;
			}
			else {
				# don't allow negative shift
				$i += $j - $occ{$char} > 0 ? $j - $occ{$char} : 1;
			}
		}
	}
	printd "\n";
	print "comparisons: $comparisons\n";

	return @hits;

} # bm

sub main {
	if (@ARGV != 2) {
		print "\nUsage: bm.pl <pattern> <text>\n";
		exit;
	}
	my ($p, $t) = @ARGV;

	my @hits = bm($t, $p);

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
	print "matches: " . (0 + @hits) . "\n";
	print "matches at: ", join(" ", @hits), "\n";

} # main()

main();
0;

