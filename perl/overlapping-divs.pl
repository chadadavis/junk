#!/usr/bin/env perl

=head1 SYNOPSIS



=head1 DESCRIPTION



=cut

use v5.14;
use strict;
use warnings;
no warnings 'experimental::autoderef';
use autodie;
use open qw(:utf8 :std);
use Log::Any qw($log);
use Carp qw(cluck confess);
$SIG{__DIE__} ||= \&confess;
use IO::All;
use Data::Dump qw(dump);
use Devel::Comments;
use Test::More qw(no_plan);
use Getopt::Long;

sub next_div {
    # Assume 1024x768
    my $x1 = 
    return 
}

