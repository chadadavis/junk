#!/usr/bin/env perl
use Modern::Perl;
my @data = map { int rand 10 } 1..100;
my $h = hist(\@data, scale => 79);

for (sort { $a <=> $b } keys %$h) {
    say sprintf("%3d ", $_) . 'x' x $h->{$_};
}

sub hist {
    my ($data, %opts) = @_;
    my %buckets;
    my $max = 0;
    for (@$data) {
        $buckets{$_}++;
        $max = $buckets{$_} if $opts{scale} and $buckets{$_} > $max;
    }
    for ($opts{scale} ? keys %buckets : ()) {
        $buckets{$_} = int $opts{scale} * ($buckets{$_} / $max);
    }
    \%buckets;
}
