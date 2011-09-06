#!/usr/local/bin/perl -w

sub println($)
{
	my($str) = @_;
	print($str, "\n");
}


sub main()
{

@names=("Muriel","Gavin","Susanne","Sarah","Anna","Paul","Trish","Simon");

print @names;
print "\n";
print "@names";
println("");
println @names[1,3,5,7];

println scalar(@names);

println($#names);

println $names[-1];

}


main();
0;

