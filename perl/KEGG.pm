#
# $Id: $ 
#
# BioPerl module for Bio::DB::KEGG
#
# Chad Davis <chad.davis@embl.de>
# 
# You may distribute this module under the same terms as perl itself
#

# POD documentation - main docs before the code

=head1 NAME

Bio::DB::KEGG - Database object interface for KEGG entry retrieval

=head1 SYNOPSIS

  use Bio::DB::KEGG;

  $kegg = Bio::DB::KEGG->new();

  $seq = $kegg->get_Seq_by_id('mpn:MPN567'); # KEGG ID

  # or changeing to accession number and Fasta format ...
  $kegg->request_format('fasta');
  $seq = $kegg->get_Seq_by_id('mpn:MPN567'); # KEGG ID

  # or ... best when downloading very large files, prevents
  # keeping all of the file in memory

  # also don't want features, just sequence so let's save bandwith
  # and request Fasta sequence
  $embl = Bio::DB::KEGG->new(-retrievaltype => 'tempfile' ,
 			    -format => 'fasta');
  my $seqio = $kegg->get_Stream_by_id(['mpn:MPN567', 'mpn:MPN020'] );
  while( my $seq =  $seqio->next_seq ) {
 	print "seq is ", $seq->id, "\n";
  }

=head1 DESCRIPTION

    
Allows the dynamic retrieval of sequence objects L<Bio::Seq> from the
KEGG database using the dbfetch script at EBI:
L<http://www.ebi.ac.uk/cgi-bin/dbfetch>.

In order to make changes transparent we have host type (currently only
ebi) and location (defaults to ebi) separated out.  This allows later
additions of more servers in different geographical locations.

The functionality of this module is inherited from L<Bio::DB::DBFetch>
which implements L<Bio::DB::WebDBSeqI>.

KEGG SOAP API

L<http://www.genome.jp/kegg/soap/doc/keggapi_manual.html>

Mapping to BioPerl 
 
L<http://doc.bioperl.org/releases/bioperl-current/bioperl-live/Bio/SeqIO/kegg.html>

=head1 FEEDBACK

=head2 Mailing Lists

User feedback is an integral part of the evolution of this and other
Bioperl modules. Send your comments and suggestions preferably to one
of the Bioperl mailing lists.  Your participation is much appreciated.

  bioperl-l@bioperl.org                  - General discussion
  http://bioperl.org/wiki/Mailing_lists  - About the mailing lists

=head2 Reporting Bugs

Report bugs to the Bioperl bug tracking system to help us keep track
the bugs and their resolution.  Bug reports can be submitted via the
web:

  http://bugzilla.open-bio.org/

=head1 TODO

Should extend L<Bio::DB::WebDBSeqI>

See L<Bio::DB::SwissProt> for an example

=head1 AUTHOR - Chad Davis

Email Chad Davis E<lt>chad dot davis at embl dot deE<gt>

=head1 APPENDIX

The rest of the documentation details each of the object
methods. Internal methods are usually preceded with a _

=cut

# Let the code begin...

################################################################################

package Bio::DB::KEGG;

use SOAP::Lite;
use Bio::SeqIO;
use IO::String;


################################################################################
=head2 new

 Title   : new
 Usage   : $kegg = Bio::DB::KEGG->new(@options)
 Function: Creates a new KEGG handle
 Returns : New KEGG handle
 Args    : 

=cut

sub new {
    my ($class, @args ) = @_;
    my $self = {};
    bless $self, $class;

    my $wsdl = 'http://soap.genome.jp/KEGG.wsdl';
    my $serv = SOAP::Lite->service($wsdl);
    $self->{'serv'} = $serv;

    return $self;
}


################################################################################
=head2 get_Seq_by_id

 Title   : get_Seq_by_id
 Usage   : $seq = $db->get_Seq_by_id('mpn:MPN567')
 Function: Gets a Bio::Seq object by its name
 Returns : a Bio::Seq sequence object
 Args    : the id (as a string) of a sequence
 Throws  : "id does not exist" exception

TODO add otpion whether to fetch nt or aa sequence. Currently AA only.

TODO add option to only fetch Fasta format (faster)

=cut

sub get_Seq_by_id {
    my ($self, $id, @options) = @_;

    # TODO use log4perl
    print STDERR "Fetching $id from KEGG\n";

    # Fetches record as string in KEGG format
    # NB: the option: '-n a' only applies to Fasta format
    # I.e. the AA sequence is still buried in a L<Bio::Annotation::Comment>
    my $record = $self->{'serv'}->bget($id);
#     print STDERR "record:\n$record\n";

    # Make a Bio::Seq
    my $iostr = new IO::String($record);
    my $seqio = new Bio::SeqIO(-fh=>$iostr, -format=>'KEGG');
    my $seq = $seqio->next_seq;
    return unless $seq;

    # Optionally replace the primary sequence (DNA) with the AA sequence
    my $annot = $seq->annotation();
    return $seq unless $annot;

    my ($aaseq_comment) = $seq->annotation()->get_Annotations('aa_seq');
    my $aaseq = $aaseq_comment->value();
    # Update the sequence object (automatically updates alphabet of sequence)
    $seq->seq($aaseq);
    return $seq;
}


################################################################################

1;

__END__


