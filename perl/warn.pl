#!/usr/bin/env perl
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

