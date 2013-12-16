#!/usr/bin/env perl

use Modern::Perl;

my @words = @ARGV || load_dict('/usr/share/myspell/dicts/en_GB.dic');

sub load_dict {
    my $dict_file = shift;
    open my $fh, '<', $dict_file;
    # skip header
    <$fh>;
    my @words = <$fh>;
    return @words;
}



