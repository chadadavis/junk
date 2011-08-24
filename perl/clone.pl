#!/usr/bin/env perl

use Spiffy -Base, -XXX;

use strict;
use warnings;
use Clone qw(clone);

my $assembly = { node => {} };
$assembly->{node}{'1g3nA'} = {};

print "assembly ", $assembly, "\n";
print "assembly->{node} ", $assembly->{node}, "\n";
print "assembly->{node}{'1g3nA'} ", $assembly->{node}{'1g3nA'}, "\n";

# depth 2 means: copy assembly (1) and the hashes in assembly (2), 
# but not what is stored in the hashes (3)
my $clone = clone($assembly, 2);

print "clone ", $clone, "\n";
print "clone->{node} ", $clone->{node}, "\n";
print "clone->{node}{'1g3nA'} ", $clone->{node}{'1g3nA'}, "\n";

print "=" x 80, "\n";

use lib "..";
use EMBL::Assembly;
use EMBL::CofM;

my $ass = new EMBL::Assembly;
$ass->cofm('1g3nA', new EMBL::CofM());

print "ass ", $ass, "\n";
print "ass->{cofm} ", $ass->{cofm}, "\n";
print "ass->{cofm}{'1g3nA'} ", $ass->{cofm}{'1g3nA'}, "\n";

my $cl = $ass->clone();

print "cl ", $cl, "\n";
print "cl->{cofm} ", $cl->{cofm}, "\n";
print "cl->{cofm}{'1g3nA'} ", $cl->{cofm}{'1g3nA'}, "\n";
