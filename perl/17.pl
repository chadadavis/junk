#!/usr/local/bin/perl -W
use strict;
use English;

my $D = 1;

sub main {
	if (@ARGV != 0) {
		print "\nUsage: \n";
		exit;
	}

	print "Debug mode.\n" if $D;

} # main()

main();
0;

__END__

