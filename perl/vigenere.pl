#!/usr/bin/env perl

#use strict;

$DEBUG = 1;

sub println
{
	my(@str) = @_ ? @_ : "";
	print("@str \n");

} # sub println

sub debug_print
{
	if ($DEBUG)
	{
		println(@_);
	}
} # debug_print


sub main()
{
	#vigenere();

                   $pwd = (getpwuid($<))[1];
	println($pwd); 
                   system "stty -echo";
                   print "Password: ";
                   chomp($word = <STDIN>);
                   print "\n";
                   system "stty echo";
	println(crypt($word, $pwd));

                   if (crypt($word, $pwd) ne $pwd) {
                       die "Sorry...\n";
                   } else {
                       print "ok\n";
                   }  


} # main()



sub vigenere
{
 
        if ($#ARGV < 0)
        {
		#if no key is given, assume (monoalphabetic) Caesar Cipher
                $key = "C";
        }
        else
        {
		#otherwise the key is some passphrase
		#TODO: should allow numbers, not just char inputs as keys
                $key = uc($ARGV[0]);
        }
        debug_print("key = $key");

	undef $/;
	$text = <STDIN>;
	chomp($text = uc($text));
	debug_print($text);

	$len = length $text;
	for ($i = 0; $i < $len - 1; $i++)
	{
		$text_char = substr($text, $i, 1);
		$cipher_char = ord($text_char) + ord($key) - 65*2;
		$cipher_char %= 26;
		$cipher_char = chr($cipher_char + 65);
		$cipher .= $cipher_char;
	}

	println($cipher);

 
} # sub vigenere
   
main();
0;
 
