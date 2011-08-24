#!/usr/local/bin/perl -w

#use strict;

sub println
{
	my(@str) = @_ ? @_ : "";
	print("@str \n");
}



sub main()
{



@list=qw(a b c);

$name1 =  $list[4] or "1-Unknown";

$name2 =  $list[4] || "2-Unknown";

print "Name1 is $name1, Name2 is $name2\n";

print "Name1 exists\n" if defined $name1;
print "Name2 exists\n" if defined $name2;



@list=qw(a b c);

$ele1 = $list[4] or print "1 Failed\n";
$ele2 = $list[4] || print "2 Failed\n";

print <<PRT;
ele1 :$ele1:

ele2 :$ele2:

PRT


} # main()


main();
0;

