#!/usr/bin/env perl

=head1 SYNOPSIS



=head1 DESCRIPTION



=cut

use strict;
use warnings;
use autodie;
use feature 'say';
use open qw(:utf8 :std);
use Carp qw(cluck confess);
$SIG{__DIE__} ||= \&confess;
use List::Util qw(min max sum sum0);
use JSON::PP;
use Getopt::Long;
# use Devel::Comments '#####';

my $t = <STDIN>;
chomp $t;

STRING: while (my $s = <STDIN>) {
    ##### $s
    chomp $s;
    next unless $s;

    my $r = reverse $s;
    CHAR: for (my $i = 1; $i < length($s); $i++) {
        ##### for
        if ( abs( ord(substr($s, $i, 1)) - ord(substr($s, $i-1, 1)) ) != abs( ord(substr($r, $i, 1)) - ord(substr($r, $i-1, 1)) ) ) {
            say 'Not Funny';
            next STRING;
        }
    }
    say 'Funny';
}
