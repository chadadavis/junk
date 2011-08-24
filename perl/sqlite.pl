#!/usr/bin/env perl

         use DBI;
         my $dbh = DBI->connect("dbi:SQLite:dbname=somefile","","");






my $sql = "create table user (id integer primary key, name varchar(40) not null);";


#    $dbh->do($sql);

#$dbh->do("insert into user values (4, 'joe');");

$pdb_code1=4;

my $prepared_stmt = $dbh->prepare('SELECT * FROM user WHERE id=?');
$prepared_stmt->execute($pdb_code1);
$row1 = $prepared_stmt->fetchrow_hashref();

# Wenn keine Eintraege gefunden werden
if (! $row1) { 
        print "<br />PDB ID: <i>$pdb_code1</i> not found!<br />\r\n";

        exit;
    } else {
        print "Found: ", $row1->{'id'}, "\n";

    }

$prepared_stmt->finish();

$dbh->disconnect() if $dbh;
