#!/usr/bin/env perl

# Example of non-ncurses-based line control
# I.e. just print a "\r" to print carriage return, without a new line ("\n")
# Need to remember to clear the line (e.g. print spaces)
#   in case subsequent lines are shorter

# Unbuffered output on STDOUT, otherwise it won't flush, because of no "\n"
$| = 1;

# Count down to see what happens when subsequent lines are shorter
$x = 105;
for (my $i = $x; $i > 0; $i--) {
    sleep(1);
    # Of course using fixed width output (e.g. %3d) would obviate need to clear
    my $str = sprintf "Number $i of $x";
    print $str;
    # Clear the line by printing an equal number of blanks
    # Note: it'd be unwise to do this often, 
    #   especially because this is unbuffered output
    print " " x length($str);
    # Return cursor to beginning of same line, without new line (carriage)
    print "\r";
}

