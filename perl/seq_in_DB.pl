#!/usr/bin/env perl

use strict; 
use warnings;
use DBI;

our $host = "pc-russell12";
my $db = "uniref90";

################################################################################

my $dbconn = DBI->connect("dbi:mysql:$db:$host") or die;
our $sth = $dbconn->prepare("select seq from uniref90 where id=?");

# Whatever your while loop is here ... 
my $count;
while ($count++ < 2) {
    my $id = "Q5SMN3";
    my $seq = fetch($id);
    print "$id\n$seq\n";
}

$sth->finish;
$dbconn->disconnect();

################################################################################

sub fetch {
    my ($id) = @_;
    $sth->execute($id);
    # Since there is only one:
    my ($seq) = $sth->fetchrow_array;
    return $seq;
}




