#!/usr/bin/env perl

use Modern::Perl;

use utf8;

my $var = 'schön';

my $filename = shift || __FILE__ . '.out';
my $layer = ':encoding(UTF-8)';
# my $layer = '';

open my $fh, '>', $filename;

# binmode select, ':encoding(UTF-8)';
binmode $fh,    ':encoding(UTF-8)';

print $fh $var, "\n";
