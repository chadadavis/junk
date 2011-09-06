#!/usr/local/bin/perl -w

if ($#ARGV >= 0) { $who = join(' ', @ARGV); }
else { $who = 'World'; }
println("Hello, $who!");
#println(('false', 'true')[$ARGV[0]]);

$data{"chad", 1} = 5;
$data{"chad", 2} = 10;
$data{"bob", 1} = 3;
$data{"bob", 2} = 11;
println(%data);
println(@data);

println((1, 2, 3, 4, 5)[-1]); #last elem in array prints

println(
	"This text doesn't belong here at all."
	);

println($#ARGV);
println(@ARGV);
println(@ARGV[2,3]);


println("one". "two");

sub println
{
	my($str) = @_;
	print("$str", "\n");
}
