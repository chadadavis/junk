#!/usr/bin/env perl
use Modern::Perl;
use Readonly;
Readonly my $PI => 3.1415926535;

say "Can interpolate $PI";

use Try::Tiny;
# use English qw(-no_match_vars); # Don't need $EVAL_ERROR with Try::Tiny

try {
    $PI = 3.1;
}
catch {
    # Note this is $_ and not $EVAL_ERROR
    say "Caught exception:\n\t$_";

    # Need a semicolon
};

# Or subsequent statement is an error
say '';


