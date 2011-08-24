
################################################################################
=head2 dblink

 Title   : dblink
 Usage   : my $kegg_id = dblink($seq_obj, 'kegg');
 Function: Finds an xref idendifier within a L<Bio::Seq>
 Returns : Scalar string, e.g. "mpn:MPN567" or undef
 Args    : A L<Bio::Seq>, will often contain L<Bio::Annotation::DBLink> objects

Looks up, within a L<Bio::Seq> any database cross-references, given an
case-insentitive ID.

=cut

sub dblink {
    my ($seq, $db) = @_;
    # All Annotation objects tagged as a 'dblink' annotation
    my @values = $seq->annotation()->get_Annotations('dblink');
    foreach my $value ( @values ) {
        # value is an Bio::AnnotationI and a Bio::DB::DBLink
        # Find the xref of choice
        next unless lc($value->database()) eq lc($db);
        # Found our DB, grab the ID
        return $value->primary_id();
    }
}
