#!/usr/bin/env perl
use Modern::Perl;
use IO::All;

# Mailto seems broken
#io('mailto:chad.a.davis@gmail.com')->print('Hello');

io('http://google.com/') > io('-');
