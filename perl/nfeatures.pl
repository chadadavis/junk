#!/usr/bin/env perl

my $samples = 0;
my $feature_tot = 0;
my %features;
my $max_features = 0;
my @elems;
while (<>) {
    next unless /^\d/;
    @elems = split / /;
    # throw away label
    shift @elems;

    $samples++;
    $feature_tot += @elems;
    $features{scalar(@elems)}++;
    $max_features = @elems if @elems > $max_features;
}

$samples or die("No samples\n");

for (my $i = 1; $i <= $max_features; $i++) {
    printf "%3d ", $i;
    my $factor = int(70 * $features{$i} / $samples);
    print "x" x $factor, "\n";
}

print "Avg features per sample: ", $feature_tot/$samples, "\n";
print "  Max: $max_features\n";
