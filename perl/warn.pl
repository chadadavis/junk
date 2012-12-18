#!/usr/bin/env perl
# Playing with warn
use Modern::Perl;
one();
sub one {
    two();
}
sub two {
    three();
}
sub three {
    warn "Broken";
}

