#!/usr/bin/perl -W
use strict;

use Bio::EnsEMBL::DBSQL::DBAdaptor;

my $host = 'ensembldb.ensembl.org'; 
my $user = 'anonymous'; 
my $dbname = 'homo_sapiens_core_20_34c';

my $db = new Bio::EnsEMBL::DBSQL::DBAdaptor(-host => $host, 
											-user => $user, 
											-dbname => $dbname);

################################################################################
# getting adaptors

my $gene_adaptor = $db->get_GeneAdaptor(); 
my $slice_adaptor = $db->get_SliceAdaptor();

################################################################################
# getting slices

my $slice;

#obtain a slice of the entire chromosome X: 
#$slice = $slice_adaptor->fetch_by_region('chromosome', 'X');

#obtain a slice of the entire clone AL359765.6 
$slice = $slice_adaptor->fetch_by_region('clone','AL359765.6'); 

#obtain a slice of an entire NT contig 
#$slice = $slice_adaptor->fetch_by_region('supercontig', 'NT_011333');

#obtain a slice of 1-2MB of chromosome 20 
#$slice = $slice_adaptor->fetch_by_region('chromosome', '20', 1e6, 2e6);

# return a slice containing a gene, with 5000 bp of flanking seq. on each side
# 2nd arg. may be omitted
#my $slice = $slice_adaptor->fetch_by_gene_stable_id('ENSG00000099889', 5000);

################################################################################
# getting seqs.

my $sequence;

# to obtain a seq. from a slice use seq() or subseq()
$sequence = $slice->seq(); 
print "$sequence\n"; 

$sequence = $slice->subseq(100, 200);
print "$sequence\n"; 
