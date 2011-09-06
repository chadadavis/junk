#!/usr/bin/perl -w 

use strict; 

print @ARGV;

print "What is your username? ";

my $username;
$username = <STDIN>;
chomp($username);
print "Hello, $username.\n";

my $what = 4;
$what **= 2; 
print $what;

my $blah = 6;

for (my $i; $i < $blah; $i++)
{
	print "-";
}
	
