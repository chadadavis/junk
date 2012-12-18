#!/usr/bin/env perl
use strict;
use warnings;
use autodie;
use IO::All;

use Carp qw(cluck);
use Data::Dump qw(dump);
use Test::More;
#use Devel::Comments;

sub bf {
    my ($input) = @_;
    my $output;

    my @ops = split '', $input;
    my $opsptr;

    my @memory;
    my $memptr = 0;

    # Duplicate lookup tables for brackets to go: left <-> right
    my %left;
    my %right;
    my @stack;
    foreach my $pos (0 .. $#ops) {
        if    ($ops[$pos] eq '[') { push @stack, $pos; }
        elsif ($ops[$pos] eq ']') {
            my $left = pop @stack;
            $right{$left} = $pos;
            $left{$pos} = $left;
        }
    }

### %left
### %right

    for (my $opsptr = 0; $opsptr < @ops; $opsptr++) {
        my $op = $ops[$opsptr];

### $opsptr
### $op
### $memptr

        if    ( $op eq '>' ) {  $memptr++ }
        elsif ( $op eq '<' ) {  $memptr-- }
        elsif ( $op eq '+' ) {  $memory[$memptr]++ }
        elsif ( $op eq '-' ) {  $memory[$memptr]-- }
        # Post-incremented loop will result in landing in pos. after closing bracket
        elsif ( $op eq '[' && 0 == $memory[$memptr] ) { $opsptr = $right{$opsptr}; }
        # Post-incremented loop: so substract 1 to land in pos before opening bracket
        elsif ( $op eq ']' && 0 != $memory[$memptr] ) { $opsptr = $left{$opsptr} - 1; }

        elsif ( $op eq '.' ) {  $output .= chr($memory[$memptr]) }
        elsif ( $op eq ',' ) { }
        else {
        }

### $memptr
### $output
### @memory
    }

    return $output;
}

my %tests = (

#    '++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.' 
#    => "Hello World!\n",

#    '>+++++++++[<++++++++>-]<.>+++++++[<++++>-]<+.+++++++..+++.>>>++++++++[<++++>-]<.>>>++++++++++[<+++++++++>-]<---.<<<<.+++.------.--------.>>+.'
#    => "Hello World!",

    '+++>+++++<[>>>+>+<<<<-]>>>>[<<<<+>>>>-]<[<<[>>>+>+<<<<-]>>>>[<<<<+>>>>-]<[<<+>>-]<-]'
    => '15',

);

for my $test (keys %tests) {
    my $expect = $tests{$test};
    is bf($test), $expect, "Test:$expect";
}

done_testing();
