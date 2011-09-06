#!/usr/bin/perl

open (GFILE, "<$ARGV[0]")
        or die "Could not open file: $ARGV[0] ($!)\n";
@lines = <GFILE>;
chop(@lines);	
$_ = join("",@lines);

/.*?(<table class="listTable">.*?Critical.*?<\/table>).*/is;

print "\n", $1, "\n";



