#!/usr/bin/env perl

# Load this in every file that wants logging
use Log::Log4perl;

# Do this just once at startup
Log::Log4perl->init("$ENV{HOME}/.log4perl");

# And then from any file that wants logging
my $log = Log::Log4perl->get_logger(__PACKAGE__);

# Log away
$log->error("stuff happened");

