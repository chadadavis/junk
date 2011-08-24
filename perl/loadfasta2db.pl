#!/usr/bin/env perl

use strict;
use warnings; 

use DBI;
use Bio::SeqIO;
use Bio::Seq;
use File::Basename;

sub usage {
    return "$0 <dbname> <fastafile.fa>\n";
}

my $db = shift or die usage();

# If file is /home/davis/stuff.fa then DB is called 'stuff'
my $table = $ARGV[0] or die usage();
$table = basename($table, ".fa");
# Name the table the same as the DB that it's in
# DB name can also be given on cmd line, otherwise same as table name

our $host = "pc-russell12";
our $dsn = "dbi:mysql:$db:$host";

print "Database: $dsn $table\n";

my $dbh = DBI->connect($dsn) or die;

# Open the fasta file given on command line
my $in = new Bio::SeqIO(-fh => \*ARGV,
                        -format=>'Fasta');
my $count;
# Preparing SQL statements once and running them several times is faster
# This database must already exist and must have a table with the fields:
# 'id' and 'seq'
# One of the fields should have been set as a primary key, for fast searching
my$sth = $dbh->prepare("insert into $table (id,seq) values (?,?)");

# For each sequence object in the fasta file
while (my $seq = $in->next_seq) {
    my $id = $seq->id;
#     $id =~ s/UniRef90_//i;
    printf "%5d %s\n", $count++, $id;
    # Now runs the query, 
    # replacing the ?,? with the sequence's ID and AA sequence
    $sth->execute($id,$seq->seq)
}

$in->close();
$dbh->disconnect();

