#!/usr/bin/perl -W
use strict;
use English;

my $wheres_the_func;

$wheres_the_func = sub {
	my @stuff = (1, 3, 5, 4, 3, );
	
	my $l = 3;
	foreach  $l (@stuff) {
		$l += 2; 
	}
	print "$l\n";
	print "@stuff\n";
}; 

&$wheres_the_func;


sub up {
	foreach (@_) { $_++; }
	return wantarray ? (1,2) : 1;
}

@_ = (1 .. 10);
print 3+&up, "\n";
print "@_\n";

# make print work like prinln
$\ = "\n";
print "testing the output record separator";
print "on a new line";
undef $\;

# change the name of the running prog, as seen by 'ps'
$0 = "init";


# Read's from the "file" that occurs in this file after the 'END' tag below
print "try reading from DATA\n";
while (<DATA>) {
	print;
}

# how to make a func prototype that converts passed arrays into refs to arrays
sub myappend(\@\@) {
	my ($a, $b) = @_;
	print "a: @$a, b: @$b \n";
	push(@$a, @$b);
	print "a: @$a\n";
}

my @c = (2, 4, 6);
my @d = (1, 3, 5);
myappend(@c, @d);
print "c: @c\n";
  
__END__
Here's some data
here's the next line
