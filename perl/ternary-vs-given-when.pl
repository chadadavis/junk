#!/usr/bin/env perl

use Modern::Perl;

my $x = 1 + int rand 5;

# Format multiple ternary operators into columns, per PBP
# Better for assignments, as given/when can't be assigned from
my $result = 
    $x == 1 ? 'one'   : 
    $x == 2 ? 'two'   : 
    $x == 3 ? 'three' : 
              "$x"    ;
say $result;

# given / when / default
# Better for flow control, or when smart matching desired
my $given;
given ($x) {
    when (1) { $given = 'one'   }
    when (2) { $given = 'two'   }
    when (3) { $given = 'three' }
    default  { $given = "$x"    }
}
say $given;
