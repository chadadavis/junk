#!/usr/bin/env perl

use lib '..';
use Bio::DB::KEGG;

use EMBL::DB;

my $kegg = new Bio::DB::KEGG();

my $seq = $kegg->get_Seq_by_id("mpn:MPN567");

use Data::Dumper;
print Dumper($seq);

print "dblink ", dblink($seq, 'uniprot');

