#!/usr/bin/env perl
use strict;
use DBI;
my $dbh = DBI->connect('dbi:mysql:host=russelllab.org;database=trans_3_0;', '%');
use Data::Dumper; 
print Dumper $dbh->selectall_arrayref('select current_user;');

print Dumper $dbh->selectall_arrayref('select * from trans limit 1;');


# anonymous user
my $local = DBI->connect('dbi:mysql:database=trans_3_0;host=russelllab.org','anonymous');

