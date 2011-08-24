#!/usr/local/bin/perl -w

sub println
{
	my($str) = @_;
	print($str, "\n");
}

sub addit($$)
{
	my($a, $b) = @_;
	$a + $b;
}

sub main
{

	println("hello");
	$items = @ARGV;
	$lastindx = $#ARGV;
	println("Items in \@ARGV: $items");
	println("Last Index: $lastindx");
	println("\@ARGV is: (" . join(", ", @ARGV) . ")");
	$dict{AZ} = Arizona;
	$dict{CA} = California;

	@bob = %dict;

	println("Size is: " . -x "04.pl");
	println("The answer is: @bob" );

	println(addit(3, 4));

	open(MYFILE, "<./04.pl");

	println(qq:This string contains a literal " character:);

	@arr = <MYFILE>;

	$str = "hello";
	$str++;
	$str++;
	println("Incremented string: $str");
	
	#println("\n\nThe whole file is: \n\n @arr");

}


main();
0;

