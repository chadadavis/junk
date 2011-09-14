#!/usr/bin/env perl

use Inline C;
use Benchmark qw(:all :hireswallclock);

my $n = shift || 1_000;
my $max = shift || 1_000;

my @ints    = int rand $max for 1..$n;
my @doubles =     rand $max for 1..$n;


cmpthese(-5, {
    count   => sub { count(  1_000_000) },
    count_c => sub { count_c(1_000_000) },
});


sub count  {
    for (my $i = 0; $i < $_[0]; $i++) {
    }
    return;
}

sub sum {
    my $sum;
    $sum += $_ for @_;
    return $sum;
}


__END__
# Other languages can be added after the END

# This marker needs to name the language of the following code
__C__

void count_c(int n) {
    int i;
    for (i = 0; i < n; i++) {
    }
}



