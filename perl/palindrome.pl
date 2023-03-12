#!/usr/bin/env perl

=head1 DESCRIPTION

Write a method which determines if it is possible to construct a palindrome for a given set of characters, in a way that all the characters are used?

  Palindrome: string which stays the same when it's reversed

Examples:
Palindromes:
aba
coddoc
aabbcbbaa

Not palindromes:
house
cat
mouse

=cut

use Modern::Perl;
use Smart::Comments;

sub potential_palindrome {
    my ($str) = @_;
    my @chars = split '', $str;
    my %counts;
    for (@chars) { $counts{$_}++ };
    my $n_odd = 0;
    for (keys %counts) {
        $n_odd++ if $counts{$_} % 2;
    }
    return $n_odd > 1 ? 0 : 1;
}

sub is_palindrome {
    my ($str) = @_;
    my @chars = split '', $str;
    for (my $i = 0; $i < (@chars / 2); $i++) {
        ### i: $i
        ### charsi: $chars[$i]
        ### j: $#chars-$i
        ### charsj: $chars[$#chars-$i]
        return 0 if $chars[$i] ne $chars[$#chars-$i];
    }
    return 1;
}

while (<>) {
    chomp;
    my $sorted = join '', sort split '';
    print "$_\t", is_palindrome($_), "\n";
    print "$sorted\t", potential_palindrome($sorted), "\n";
}

