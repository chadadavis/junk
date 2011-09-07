#!/usr/bin/env perl

# Get a stack trace from an uncaught exception just by doing:
# perl -Mdiagnostics diagnostics.pl
# So, don't always need Carp::Always to get a stack trace upon die
# Or ignore the warnings and just get the trace:
# perl -Mdiagnostics=-traceonly diagnostics.pl

one();
exit;

sub one {
    two();
}
sub two {
    die "two";
}

