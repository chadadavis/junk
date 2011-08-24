#!/usr/bin/env perl

# For rcombine() (requires Math::Combinatorics from CPAN)
require "rcombinatorics.pl";

# Assume genes exist, named a-..
my @genes = 'a'..'z';
# Assume TFs exist, named 1..
my @tfs = 1..10;

# Example relationship between genes and TFs
# For each gene, identifies the TFs that regulate it
my %gene2tf;
# Just use a random combination of the existing TFs, for this example
print "Assumed regulatory rules:\n";
foreach (@genes) {
    # Get a random combination (of random size) of the TFs
    $gene2tf{$_} = [ rcombine(undef, @tfs) ];
    print "gene: $_ reg'd by TFs: ", join(" ", @{$gene2tf{$_}}), "\n";
}
print "\n";

# Differentially expressed genes (the actual input)
# Just use a random subset of 1/3 of existing genes for this example
my %diff;
for (1..int(scalar(@genes)/3)) {
    my $idx = int(rand(@genes));
    $diff{$genes[$idx]} = 1;
}
my @diff = keys %diff;
print "Assumed diff. expr. genes: @diff\n\n";

# Now, given the diff. expr. genes, score each TF that *could* reg. it, once
my %count;
foreach my $deg (@diff) {
    foreach my $tf (@{$gene2tf{$deg}}) {
        $count{$tf}++;
    }
}

# Now go through all genes again, and for each gene, 
# choose the TF with the highest count as the most parsimonous explanation
my %explanations;
foreach my $deg (@diff) {
    # The TF (for this gene), with highest count
    my @possible_tfs = @{$gene2tf{$deg}};
    my @sorted_tfs = sort { $count{$b} <=> $count{$a} } @possible_tfs;

    print "gene: $deg possible TFs:\n  ";
    foreach (@sorted_tfs) {
        print "$_ ($count{$_} genes) ";
    }
    # No explanation? Skip it.
    @sorted_tfs or next;

    # Choose highest scoring TF to explain this gene
    my $tf = $sorted_tfs[0];
    print "\n  choose TF: $tf\n";
    # This TF is part of the minimal set
    $explanations{$tf} = 1;
}
print "\n";

print "Minimal explanation:\n";
# Most parsimonious set of TFs, explaining all diff. expr. genes, not in order:
foreach (keys %explanations) {
    print "$_ ($count{$_} genes) ";
}
print "\n";
