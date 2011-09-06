#!/usr/local/bin/perl -w

sub println($)
{
	my($str) = @_;
	print($str, "\n");
}



sub main()
{


srand;
$tries = 0;
while (1)
{
	if (rand(88) > $ARGV[0] )
	{
		print "Tries: $tries\n";
		last; #the is the last iteration, i.e. break
	}
	else
	{
		$tries++;
	}
}

} # main()


main();
0;

