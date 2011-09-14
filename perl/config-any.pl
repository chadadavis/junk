#!/usr/bin/env perl

use Modern::Perl;
use Config::Any;
my @files = @ARGV;
# flatten_to_hash will give one HashRef rather than ArrayRef[HashRef]
my $cfg = Config::Any->load_files({ files => \@ARGV, flatten_to_hash => 1});
use Data::Dumper;
say Dumper $cfg;

use Config::INI::Reader;
my $ini = Config::INI::Reader->read_file($ARGV[0]);
say Dumper $ini;

