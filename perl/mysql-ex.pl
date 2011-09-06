#!/usr/bin/env perl

# $Id: orf2pedant.pl,v 1.2 2005/06/02 12:45:00 davis Exp $

=head1 NAME

orf2pedant.pl - Script to insert ORFs as their corresponding contigs (FASTA
format) into Pedant.
    
=head1 SYNOPSIS

perl orf2pedant.pl <contigs.fa> <orf_nt.fa> <orf_aa.fa>

=head1 DESCRIPTION

Loads ORFs (nucleotid sequences as well their translated amino acid sequences)
from files in FASTA format into Pedant in the 'orf' and 'orf_data' tables

Note: This 

=head1 OPTIONS

=over

=item -db | -d
    
Name of the database containing the ORF tables to be updated.

=item -server | -s

Host name of the database server (defaults to 'localhost')

=item -user | -u

Database user name (defaults to local user name)

=item -pass | -p

Database password. Will be prompted when not given, if standard input is
connected to a terminal (tty).

=item -verbose | -v

Prints more information on the actual execution of the program. 

=item -help | -h | -?      

Display this help.

=back

=head1 REQUIREMENTS

Perl DBI module. 

=head1 BUGS

The database access code could possibly be sped up a somewhat.

Only the 'orf_data' table is currently being updated. The 'orf' table needs
to be updated synchronously with the proper foreign keys. 

Also, the contigs and translated ORFs are not currently being inserted into
Pedant, just the untranslated nucleotide ORF sequences are.

=head1 AUTHOR

=over 

=item Chad Davis

=item Institute for Bioinformatics

=item GSF, Neuherberg, Germany

=item http://mips.gsf.de/staff/davis

=back

=head1 REVISION

$Id: orf2pedant.pl,v 1.2 2005/06/02 12:45:00 davis Exp $

=head1 APPENDIX

=cut

###############################################################################

package hepp;
use hepp::Util;

# Check for modules we want/need
assertmod('Getopt::Long');
assertmod('DBI');


###############################################################################

# TODOFET
# This FASTA file reading should be abstracted into an OO system
# Similar to Bioperl, but don't require that. Just copy the little bit of
#   code that's necessary for a similar approach (i.e. $fa->next() )

###############################################################################

# TODOFET
# this script needs to put a few things into pedant:
# - the original contig, which is sent into framefinder
# - the subsequence nt, which we get from framefinder2orf
# - the translated aa seq., which comes out of framefinder
# Add switches for these three files to the cmd. line ops.

# orf - meta table (start and stop fields of orf from contig)
#

# save contigs on disk, put fs path to contig file in db, 
#  but in which table? 
#  there will naturally be many contigs in a file

# orf_data
# id is just a counter
# code may be Biomax-specific, maybe organism specific

# prot_data
# id is just a counter

# desc fields are for the fasta headers, right?

#  contig_data_id  is for the id number of the contig
# this is produced by phrap, when it generates its consensuses, for example

# can  contig_data_code be left blank then?

###############################################################################


# Set default command line options
my %ops = ( verbose => 0, );
# Parse command line options
GetOptions(\%ops, 
           'db|d=s',
           'server|s=s',
           'user|u=s',
           'pass|p=s',
           'verbose|v',
           'help|h|?',
           )
    or $ops{'help'} = 1;

# If options didn't parse or help was requested ...
$ops{'help'} and usage();

$::VERBOSE = $ops{'verbose'};

while (my ($key, $val) = each %ops) {
    verbose("$key = $val\n");
}

# Setup database connection, etc.
my $dsn = 
    "dbi:mysql:" . 
    ($ops{'db'} || "human_test_chad") . ":" . 
    ($ops{'server'} || "localhost");
my $dbh;
my $dbuser = $ops{'user'} || $ENV{'USER'};

# If password isn't provided, trying prompting, but only when we're on a tty
# TODOBUG make this password business a bit more secure at least don't pass
#   the param on the command line. Check if the param is a filename and read
#   the file to get the pass. 
my $dbpass = $ops{'pass'};
`tty`;
if ($? >> 8 == 0) {
    $dbpass ||= getpass("Password for $dbuser on $dsn");
} else {
    $dbpass or 
        die("No password provided and will not prompt for password, " . 
            "because standard input is not a tty\n");
}

verbose "dsn: $dsn\n";

# TODOFET maintain databank state,without reconnecting ?

# Setup DB connection
$dbh = DBI->connect($dsn, $dbuser, $dbpass, { AutoCommit => 0 }) or
    die("Could not connect to database:" . DBI->errstr . "\n");

# Header and sequence data for each sequence in file
my ($head, $seq);
# Flag which will tell us whether we should commit executed transacions
my $success = 1;
# Number of sequences inserted into database;
my $inserts = 0;

# First find out where the ID needs to begin counting
# TODOFET we probably don't need a prepared stmt. for this
my $max_sql = "SELECT max(id) FROM orf_data";
my $max_stmt = $dbh->prepare_cached($max_sql) or 
    die("Could not prepare statement: " . $dbh->errstr . "\n");
$max_stmt->execute();
# Set our counter to start using the next largest ID number
my $count = ($max_stmt->fetchrow)[0] + 1;
$max_stmt->finish;

# Read all files given on the command line, otherwise read stdin
while (<>) {
    chomp;
    if (/^>/) {
        # First insert the previous sequence before starting a new one
        verbose "$head\n" if $head;
        $inserts++ if $head;
        $success &&= insert($count++, $head, $seq) if $head;
        # Start parsing a new sequence
        $head = $_;
        $seq = '';
        next;
    }
    # This is a sequence data line. Append to the running sequence.
    $seq .= $_;
}

# Insert the final sequence
verbose "$head\n" if $head;
$inserts++ if $head;
$success &&= insert($count++, $head, $seq) if $head;

if ($success) {
    $dbh->commit or 
        die("Database commit failed: " . $dbh->errstr);
} else {
    die("Could not commit transacation: " . $dbh->errstr);
}

print "Inserted $inserts sequences into database.\n";

$dbh->disconnect() if $dbh;

exit;


###############################################################################

=head2 insert

 Title   : insert
 Usage   : $success = insert($id_number, $seq_header, $seq_text);
 Function: Inserts the sequence data in $seq_text into the database. 
           The $id_number must be a unique ID in the ORF tables. 
           The $seq_header is the FASTA header.
 Returns : True or false, whether successful or not.
 Args    : Unique counter, FASTA header, raw sequence 

This insertion transaction must still be 'comitted'. This is the reason that
the method returns true or false. If a series of inserts all return true, you
may assume that it is safe to try to commit all of the transactions.

=cut

sub insert {
    my ($id, $head, $seq) = @_;
    my $success = 1;
    # Setup prepared statements for inserts
    # TODOFET, 'begin', 'end', 'length', need to be inserted into 'orf' as well
    my $insert_orf_sql = qq(
                            INSERT INTO orf 
                            (id, descr) 
                            VALUES 
                            (?, ?)
                            );
    my $insert_orf_data_sql = qq(
                                 INSERT INTO orf_data 
                                 (id, descr, dat)
                                 VALUES
                                 (?, ?, ?)
                                 );
    my $insert_orf_stmt = $dbh->prepare_cached($insert_orf_sql) or
        die("Could not prepare statement: " . $dbh->errstr . "\n");
    # TODOFET, should the contigs als be inserted into PEDANT?
    #   Yes!
    # TODOFET the translated AA sequences from framefinder need to inserted 
    #   also
    # TODOBUG, the foreign keys between these two tables need to be linked
    my $insert_orf_data_stmt = $dbh->prepare_cached($insert_orf_data_sql) or
        die("Could not prepare statement: " . $dbh->errstr . "\n");

    # TODOBUG, need to keep 'orf' in sync with 'orf_data'
    # How does this work?
    #$success &&= $insert_orf_stmt->execute($id, $head);
    $success &&= $insert_orf_data_stmt->execute($id, $head, $seq);

    # Return the return value of the insert(s), success (true if all worked)
    return $success;

} # insert

###############################################################################


1;

__END__

