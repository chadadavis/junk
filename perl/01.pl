#!/usr/bin/perl -w

sub println
{	
	my($str) = @_;
	print($str, "\n");
}

   $a = "8";    # Note the quotes.  $a is a string.
   $b = $a + "1";   # "1" is a string too. = 9
   $c = $a . "1";   # But $b and $c have different values! = 81

@alist = (1, 2, 4, 0);
@list = sort @alist;

$first = "chad";
$last = "davis";
$all = $first . " " . $last;
print $all, "\n";


@people = ("me", "you", "us");

print @people, "\n", $people[1], "\n";

@a = (1, 2, 3);
@b = (4, 5, 6);

@double = (@a, @b);

print @double;

print "\n $#double \n";


$#double++;

$double[$#double] = "hello";

$double[++$#double] = "end";

print @double;


%dict = ( 1 => 1, 2 => 4, 3 => 9, 4 => 16);

print "\n";
print %dict;

print "\n $dict{4} \n";

print(keys(%dict));

@alphabet = ("a" .. "z");

print @alphabet, "\n";

for $i (keys %dict)
{
	print $i, "\n";
}

%hash = ( AB => 1, SDA => 4, DG => 9, GDGGG => 16);
print join("-", reverse sort map {lc} keys %hash);
print "\n";

println("Integer math:");
println int (5.5 + 6.6 / 9);
println("Done");


print keys (%dict), "\n"; 

print "--------------\n";

@words = ("one", "two", "three", "two", "one", "one", "one");


for $i (0 .. $#words)
{
	$store{$words[$i]} ++;
}

print %store;
print("--------------\n");


if ("hi" eq "hi")
{
	print "yes\n";
}


open (LOGFILE, "<$ARGV[0]")
	and println("Successfully opened file: $ARGV[0]")
	or die "Could not open file: $ARGV[0] ($!)\n";


@filelen = <LOGFILE>;
println("File has ", $#filelen + 1, " lines");

close LOGFILE;

print "\n";

(5 == 5) and print "yes\n";
(4 == 5) or print "no\n";


sub hi;

@sdf = hi (I, Want, Something);

print "returned: @sdf\n";


sub hi
{
	for $i (0 .. $#_)
	{
		print "$_[$i]\n";
	}
	return @_;


}


for $i (0 .. 4)
{
	print "$str\n";
	$str .= " " . "hello";
}


print @ARGV;

$varx = bob;

if ('string with bob' eq 'string with $varx')
{
	print("They are equal\n");
}
else
{
	print("What happened\n");
}


$teststr = "abcdefg";

print($teststr, "\n");

println("I'm a prinln");

println("Original string: $teststr");

substr($teststr, 5, 0) = "-ins-";

println("New string: $teststr");





