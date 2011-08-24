#!/usr/bin/env perl

use Bio::SeqIO;
use Bio::Seq;
use Bio::Tools::SeqStats;

my $seq_in = Bio::SeqIO->new(-format=>'Fasta', -fh => \*ARGV);

my $counter = 0;
while (my $seq = $seq_in->next_seq) {
        $counter++;
        print $seq->display_id(), " length ", $seq->length(), "\n";
        my $wt = Bio::Tools::SeqStats->get_mol_wt($seq);
        print "MW lower $$wt[0] upper $$wt[1]\n";

}
