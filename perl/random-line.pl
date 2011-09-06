#!/usr/bin/env perl
use strict;
use warnings;
use File::Random qw/:all/;

# Get input file name
# Format: word and definitions separated by "::"
my $LIST = shift || die("Need a input file (vocab list)\n");
-r $LIST or die "File ($LIST) not found/readable.\n";

# Number of times to show a word (0 for no limit)
my $limit = shift || -1;
my $wait = $limit == -1;

# Use Ctrl-C to exit
while ($limit--) {
	# Get a random line
    $_ = random_line($LIST);
    # split around dividing characters
    /::/;
	# $` is the left side of the matched split
	# $' is the right side of the matched split (printed below)
	# Skip undefined entries
	next if $' =~ /^\s*$/; # emacs parser broken, so here is a '

    if ($wait) {
        # Change color of keyword
        print("\e[0;33m$`\e[0;0m ");
        # wait for user to touch keyboard (read each char in "real-time")
        system "stty", '-icanon', 'eol', "\001";
        # Wait for ENTER to be pressed
        while (getc ne "\n") {}
        # reset tty
        system "stty", 'icanon', 'eol', '^@'; # ASCII null
    } else {
        # Just print the word
        print("$` :: ");
    }
    # print text after the tab character(s) from the match
    print("$'");
}


