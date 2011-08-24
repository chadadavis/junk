#!/usr/bin/env perl

use Text::BibTeX;
use feature 'say';

my $newbib = new Text::BibTeX::File(">out.bib");

Text::BibTeX::bibloop(\&mysub, \@ARGV, $newbib);

sub mysub {
    my ($entry) = @_;

    my $pri = $entry->get('priority');
    my @keys = split(/,\s*/, $entry->get('keywords'));
    if ($pri eq 0) {
        push @keys, '_read';
    } else {
        push @keys, '_unread';
        push @keys, "_pri${pri}";
    }
    my $keys = join(', ', sort @keys);
    say $keys;
    $entry->set('keywords', $keys);
    return $entry;
}

__END__

