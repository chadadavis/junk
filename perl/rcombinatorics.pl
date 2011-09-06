#!/usr/bin/env perl

use Math::Combinatorics;

sub rcombine {
    my ($n, @set) = @_;
    $n ||= int(rand(@set));
    my @combinations = combine($n, @set);
    return @{$combinations[int(rand(@combinations))]};
}

sub rpermute {
    my (@set) = @_;
    my @permutations = permute(@set);
    return @{$permutations[int(rand(@permutations))]};
}

1;
__END__
