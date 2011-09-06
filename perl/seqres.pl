#!/usr/bin/env perl

my $files = 0;
my $n = 0;
# foreach my $f (</g/data/pdb/pdb*ent>) {
foreach my $f (</g/data/pdb/pdb1t*ent>) {
    
# foreach my $f (</g/data/pdb/pdb1ti*ent>) {
    $files++;
    print "$files\n" unless $files % 1000;
    @ARGV = ($f);
    while (<>) {
        next unless /^SEQRES/;
        my $l = substr $_, 19, 51;
        my @a = split /\s+/, $l;
        $n += @a;
    }
}

print $n, "\n";
