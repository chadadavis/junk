#!/usr/bin/env perl

use File::Basename;
use File::Spec::Functions qw/rel2abs catdir/;
use lib catdir(rel2abs(dirname(__FILE__)), "..");

use Data::Dumper;
use Utils;

use Bio::SeqIO;
use Bio::Seq;

$::DEBUG = 1;

# http://search.cpan.org/~birney/bioperl-1.4/Bio/Phenotype/OMIM/OMIMparser.pm
# http://search.cpan.org/~birney/bioperl-1.4/Bio/Phenotype/OMIM/OMIMentry.pm
# http://search.cpan.org/~birney/bioperl-1.4/Bio/Phenotype/OMIM/OMIMentryAllelicVariant.pm

use Bio::Phenotype::OMIM::OMIMparser;
use Bio::Phenotype::OMIM::OMIMentry;
use Bio::Phenotype::OMIM::OMIMentryAllelicVariant;

# TODO write (Perl) code for downloading and cleaning OMIM DB

# wget ftp://ftp.ncbi.nih.gov/repository/OMIM/omim.txt.Z ftp://ftp.ncbi.nih.gov/repository/OMIM/genemap
# gunzip omim.txt.Z (do this in Perl) 
# Clean:
# while (<>) { print if 18==split(/\|/); }

my $omim_dir = $data_dir;
my $genemap = "$omim_dir/genemap-clean";
my $omimtext = "$omim_dir/omim.txt";
die unless -r $genemap && -r $omimtext;

my $omim_parser = 
    Bio::Phenotype::OMIM::OMIMparser->new(-genemap => $genemap, 
                                          -omimtext => $omimtext);

while (my $omim_entry = $omim_parser->next_phenotype()) {

    my @avs = $omim_entry->each_AllelicVariant(); 
    # Positions of the variants
    @avs = grep { $_->position() } @avs;
    # Skip OMIM entries without mutation AA coordinates 
    next unless @avs;

    print $omim_entry->MIM_number(), " ";
    for my $av (@avs) {
        print $av->aa_ori(), $av->position(), $av->aa_mut(), ",";
    }
    print "\n";
}
