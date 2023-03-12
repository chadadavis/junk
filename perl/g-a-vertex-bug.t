#!/usr/bin/env perl

use strict;
use warnings;

use Graph;
use Storable;
use Test::More 'no_plan';
use Data::Dumper;
use Carp qw/cluck/;

package MyNode;
use overload ('""' => '_asstring', fallback=>1);
sub new {
    my ($class, %ops) = @_;
    return bless { %ops }, $class;
}
sub _asstring {
    my ($self) = @_;
    my $str = $self->{'name'};
    Carp::cluck("Stringifying $str");
    return $self->{'name'};
}
1;

package main;

my $gnoref = new Graph;
my $gwithref = new Graph(refvertexed=>1);
my $n1 = new MyNode('name'=>'alpha');
my $n2 = new MyNode('name'=>'beta');
$gnoref->add_edge($n1, $n2);

# If you see no further stack traces from cluck, then : 
print "\nNo more stringification occurs after this point\n\n";
$gwithref->add_edge($n1, $n2);

# And then this fails, meaning that Storable is also not usable, because the
# nodes are hashed by their memory addresses, which will change on retrieve()
is_deeply([sort keys %{$gnoref->[2]->[4]}],[sort keys %{$gwithref->[2]->[4]}]);

print "\n";

__END__




