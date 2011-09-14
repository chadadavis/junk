#!/usr/bin/env perl
use Modern::Perl;
use DBI;
my $dbh = DBI->connect('dbi:mysql:host=russelllab.org;database=trans_3_0;', '%');
use Data::Dumper; 
print Dumper $dbh->selectall_arrayref('select current_user;');

print Dumper $dbh->selectall_arrayref('select * from trans limit 1;');
