#!/usr/bin/env perl

use Moose::Autobox;
use subs::parallel;

$|=1;


my $a = [0..19];

sub doit {
    # Another incompatible change
    my ($x) = @_;
    for (my $i = 0; $i < 10000000; $i++) { }
    print ".";
    return $x;
}
parallelize_sub('doit');

my $s = $a->map(sub{doit $_});

use Data::Dump qw/dump/;
print "\n@$s\n";

# foreach (1..20) {
#     doit($_);
# }


# sleep 30;
