#!/usr/bin/env perl
# Glassdoor: http://u.booking.com/su
use Modern::Perl;
use Devel::Comments;
my $n = shift || 7;
print cube_surface($n);
exit;

sub cube_surface {
    my $n = shift;
    return 1 if $n == 1;
    return 8 if $n == 2;
    my $res = $n**3 - ($n-2)**3;
    return $res;
}
