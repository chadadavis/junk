#!/usr/bin/env perl
use v5.14;

my @colors = qw(red green blue yellow);
my @countries = get_countries();

my $success = choose_color($countries[0]);
if ($success) {
    say join "\n", @countries;
}
else {
    say "Failed";
}

exit $success;

# TODO does this even require backtracking, or is it sufficient to just iterate $color = ($color + 1) % 4 
# I.e. is there a valid graph that can become invalid by adding a node with a specific color somewhere?
sub choose_color {
    my ($c) = @_;
    # Already processed
    return $c->color if $c->color;
    # Neighboring colors already taken
    my @used = grep { defined } map { $_->color } @{ $c->neighbors() };
    # Re-calc avail colors each time?
    my @avail = grep { my $x = $_; ! grep { $_ eq $x } @used } @colors;
    return unless @avail;
    # Choose random color
    for my $col (@avail) {
        $c->color($col);
        my $fail = 0;
        # Try to recursively color neighbors
        for my $n ($c->neighhors) {
            if (! $n->choose_color) {
                $fail = 1;
                $c->color(undef);
                last;
            }
        }
        if (! $fail) {
            return $col;
        }
    }
    return;
}

