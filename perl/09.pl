#!/usr/local/bin/perl -w

sub println($)
{
	my($str) = @_;
	print($str, "\n");
}



sub main()
{


$dir= $ARGV[0];

opendir DIR, $dir or die "Can't open directory $dir: $!\n";

while ($file= readdir DIR) {
        print "Found a file: $dir/$file\n" if -T "$dir/$file" ;
	}



} # main()


main();
0;

