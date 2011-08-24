#!/usr/bin/perl -w 

use strict; 


undef $/;           # read in whole file, not just one line or paragraph
while ( <> )
{

	while ( /BEGIN(.*?)END/ms ) # /s makes . cross line boundaries
	{

        	print "$1\n";
	}
}

