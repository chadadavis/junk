#!/usr/bin/perl

use strict;
use Getopt::Std;

my %args;
getopts('x', \%args);

unless ($args{'x'}) {
	# Happens the first time
	my $prog = ($0 =~ /^[.]/) ? $ENV{'PWD'} . "/" . $0 : $0;
	my $term = `which $ENV{'TERM'}`;
	chomp $term;

	# program reruns itself, but with an option that stops it the 2nd time
	exec("$term -e $prog -x " . join(" ", @ARGV));

}

# Real body of the program

print "Options: ", join(" ", @ARGV), "\n";
print "Doing stuff ... \nDone.\n";


<STDIN>;


