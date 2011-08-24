#!/usr/bin/env perl
# googly.pl
# A typical Google Web API Perl script
# Usage: perl googly.pl <query>

# Your Google API developer's key
#my $google_key='insert key here';
my $google_key='TjohAStQFHIT9YXCgx5K6UgTIlJnl+Vf';

# Location of the GoogleSearch WSDL file
my $google_wdsl = "./GoogleSearch.wsdl";

use strict;
use warnings;

# Use the SOAP::Lite Perl module
use SOAP::Lite;

# Take the query from the command-line
my $query = join(' ', @ARGV) or die "Usage: perl googly.pl <query>\n";

# Create a new SOAP::Lite instance, feeding it GoogleSearch.wsdl
my $google_search = SOAP::Lite->service("file:$google_wdsl");

print "Querying ... $query\n";
# Query Google
my $results = $google_search -> 
    doSpellingSuggestion(
      $google_key, $query, 
    );

if ($results) {
	print "$results\n";
} else {
	print "No spelling suggestions\n";
}

