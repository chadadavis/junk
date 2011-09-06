#!/usr/local/bin/perl -w

sub println($)
{
	my($str) = @_;
	print($str, "\n");
}



sub main()
{

if ($ARGV[0])
{
	$HANDLE = $ARGV[0];
}
else
{
	die("Specify a file name on the command line\n");
}

open(HANDLE) or die("I can't find the fucking file! ($HANDLE)\nCode: $!\n");

#record separator
$/ = ":";

while (<HANDLE>)
{
	chomp;
	print("$. ", $_, "\n"); # print rec# followe by record
}


} # main()


main();
0;

