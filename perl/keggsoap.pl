#!/usr/bin/env perl

use SOAP::Lite;
use Data::Dumper;

use lib "..";
use EMBL::DB;

my $wsdl = 'http://soap.genome.jp/KEGG.wsdl';
my $serv = SOAP::Lite->service($wsdl);

#Provides a desc line
# my $res = $serv->bfind('mpn MPN567');

# Fetches record as string in KEGG format
# NB: the option: '-n a' only applies to Fasta format
# I.e. the AA sequence is still buried
my $res = $serv->bget('-n a mpn:MPN567');
# print Dumper($res), "\n";

use Bio::SeqIO;
use IO::String;

# Make a Bio::Seq
my $strio = new IO::String($res);
my $seqio = new Bio::SeqIO(-fh=>$strio, -format=>'KEGG');
my $seq = $seqio->next_seq;
# print Dumper($seq), "\n";

print "Seq: ", $seq->display_id, "\n";

# Lookup uniprot dblink
print dblink($seq, 'uniprot');

my ($aaseq_comment) = $seq->annotation()->get_Annotations('aa_seq');
# my $aaseq = $aaseq_comment->as_text;
my $aaseq = $aaseq_comment->value();
# Update the sequence object (Need to change alphabet too? It's Automatic!)
$seq->seq($aaseq);
# print "aa\n$aaseq\n";
print Dumper($seq), "\n";


