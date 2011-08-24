#!/usr/bin/env perl

#ATOM   3736  CB  HIS B 248      45.288  79.750 -15.238  1.00  0.00           C 

my $gmin = 1;
my $cur;
foreach my $pdb (</g/data/pdb/pdb*ent>) {
# foreach (</home/davis/.bash*>) {
    @ARGV = ($pdb);
    my $min = 1;
    while (<>) {
        next unless /^ATOM/;
        $cur = substr $_, 22, 4;
        if ($cur < $min) {
            $min = $cur;
        }
    }
    if ($min < $gmin) {
        $gmin = $min;
    }
    printf "$pdb %4d %4d\n", $min, $gmin;
}
