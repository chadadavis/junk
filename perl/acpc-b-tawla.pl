#!/usr/bin/env perl

=head1 SYNOPSIS

ACPC Problem B


=head1 DESCRIPTION



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

my %single = (
    1 => 'Yakk',
    2 => 'Doh',
    3 => 'Seh',
    4 => 'Ghar',
    5 => 'Bang',
    6 => 'Sheesh',
);

my %double = (
    1 => 'Habb Yakk',
    2 => 'Dobara',
    3 => 'Dousa',
    4 => 'Dorgy',
    5 => 'Dabash',
    6 => 'Dosh',
);

my $case_n = 1;
my $cases_n = <>;
chomp $cases_n;

while (<>) {
    chomp;
    next unless $_;
    my @nums = reverse sort split /\s+/;
    my $spoken;
    if ($nums[0] == 6 && $nums[1] == 5) {
        $spoken = 'Sheesh Beesh';
    }
    elsif ($nums[0] == $nums[1]) {
        $spoken = $double{$nums[0]};
    }
    else {
        $spoken = join ' ', $single{$nums[0]}, $single{$nums[1]};
    }
    say "Case $case_n: $spoken";
    $case_n++;
}
