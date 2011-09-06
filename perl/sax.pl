#!/usr/bin/perl -W
use strict;
use English;

# include package
use XML::Parser;

# initialize parser
$xp = new XML::Parser();

# set callback functions
$xp->setHandlers(Start => \&start, End => \&end, Char => \&cdata);

# keep track of which tag is currently being processed
$currentTag = "";

# send standard header to browser
print "Content-Type: text/html\n\n";

# set up HTML page
print "<html><head></head><body>";
print "<h2>The Library</h2>";
print "<table border=1 cellspacing=1 cellpadding=5>";
print "<tr><td align=center>Title</td><td align=center>Author</td><td
align=center>Price</td><td align=center>User Rating</td></tr>";

# parse XML
$xp->parsefile("library.xml");

print "</table></body></html>";

# this is called when a start tag is found
sub start()
{
	# extract variables
	my ($parser, $name, %attr) = @_;

	$currentTag = lc($name);

	if ($currentTag eq "book")
	{
		print "<tr>";
	}
	elsif ($currentTag eq "title")
	{
		print "<td>";
	}
	elsif ($currentTag eq "author")
	{
		print "<td>";
	}
	elsif ($currentTag eq "price")
	{
		print "<td>";
	}
	elsif ($currentTag eq "rating")
	{
		print "<td>";
	}

}

# this is called when CDATA is found
sub cdata()
{
	my ($parser, $data) = @_;
	my @ratings = ("Words fail me!", "Terrible", "Bad", "Indifferent", "Good",
"Excellent");

	if ($currentTag eq "title")
	{
		print "<i>$data</i>";
	}
	elsif ($currentTag eq "author")
	{
		print $data;
	}
	elsif ($currentTag eq "price")
	{
		print "$$data";
	}
	elsif ($currentTag eq "rating")
	{
		print $ratings[$data];
	}

}

# this is called when an end tag is found
sub end()
{
	my ($parser, $name) = @_;
	$currentTag = lc($name);
	if ($currentTag eq "book")
	{
		print "</tr>";
	}
	elsif ($currentTag eq "title")
	{
		print "</td>";
	}
	elsif ($currentTag eq "author")
	{
		print "</td>";
	}
	elsif ($currentTag eq "price")
	{
		print "</td>";
	}
	elsif ($currentTag eq "rating")
	{
		print "</td>";
	}

	# clear value of current tag
	$currentTag = "";
}

# end
