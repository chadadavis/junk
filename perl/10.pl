#!/usr/local/bin/perl -w

sub println
{
	my(@str) = @_ ? @_ : "";
	print("@str \n");
}



sub main()
{

%countries=('976','Mongolia','52','Mexico','212','Morocco','64','New Zealand','33','France');

foreach (sort { $countries{$a} cmp $countries{$b} } keys %countries) {
        print "$_ $countries{$_}\n";
	}

#println(join(":", split(//, ($_ = <STDIN>))));


println(reverse(sort(@ARGV)));



println("hello", "bye");

println();

$str = "hello";

$str =~ tr/a-z/c-za-b/;

println($str);


} # main()


#main();

0;

