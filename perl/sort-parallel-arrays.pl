#!/usr/bin/env perl
use v5.14;

my @people = ( 'john', 'manuel', 'ivan', 'david', 'mohamed' );
my @ages   = (  23,     45,       32,     23,      19 );

my @sorted_indexes = sort { $ages[$b] <=> $ages[$a] } keys @people;
my @sorted_people  = @people[@sorted_indexes];

say "@sorted_people";
