#!/usr/bin/env perl

use Test::More 'no_plan';

#NB 
# Least significant bit on the right
# To get the better scoring templates to be tried together first, sort desc
my @sorted_sedges = qw/great good better ok worse worst/;
@sorted_sedges = reverse @sorted_sedges;

use Data::PowerSet;
# Set min=>1 to skip the null set
my $powerset = Data::PowerSet->new({min=>1}@sorted_sedges);

while (my $set = $powerset->next) {
    print "set: @$set\n";

}


__END__

