#!/usr/bin/env perl

=head1 SYNOPSIS

=head1 DESCRIPTION

You want to add a feature on the site to allow a customer to book certain
add-ons once they arrive at their hotel. Add-ons have different prices, e.g.:

{
    champagne: 50,
    massage: 35,
    late-checkout: 15,
    spa: 25
}

For 100 Euro, identify what ameneties a person could book. It's possible to book ameneties more than once.

Followups:
* 100 is not fixed
* Some ameneties are per person (breakfast), i.e. if you get it,  you have to get it for all N guests.
* Some ameneties are only available once (e.g. late checkout)
* What if a set of ameneties adds up to 105?
** Would you still offer it?
** What if we dont' want the customer to have to specify 100 Euro, how would you determine the best N
** ...

=cut

use v5.14;
use strict;
use warnings;
use autodie;
use open qw(:utf8 :std);

use Log::Any qw($log);
use Carp qw(cluck);
$SIG{__DIE__} ||= \&confess;

use IO::All;
use Data::Dump;
use Devel::Comments;
use Test::More qw(no_plan);
use Getopt::Long;

# TODO dynamic programming ?

