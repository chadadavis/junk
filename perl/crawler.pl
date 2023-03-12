#!/usr/bin/env perl

=head1 SYNOPSIS

Implement a web crawler

Figure out what pages are reachable from a start page.

index($url) will take care of indexing it for you.

$page.links() will return an array/list of links from this page.

Follow-up: identify duplicate targets (same as identifying cycles via
$visited)

Follow-up: URL params and duplicate pages (what's the cache key?)

Follow-up: Now it's all been indexed, how to schedule which pages to index
next? In what order?

Follow-up: What about pages that aren't connected from the original pages. How
would you find out about those, in practice?

=head1 DESCRIPTION

=cut

use v5.14;
use strict;
use warnings;
no warnings 'experimental::autoderef';
use autodie;
use open qw(:utf8 :std);
use Log::Any qw($log);
use Carp qw(cluck);
$SIG{__DIE__} ||= \&confess;
use IO::All;
use Data::Dump qw(dump);
use Devel::Comments;
use Test::More qw(no_plan);
use Getopt::Long;

my @edges = (
    [ 3,  2 ],
    [ 5,  1 ],
    [ 5,  2 ],
    [ 77, 1 ],
    [ 15, 16 ],
    [ 2,  15 ],
    [ 15, 3 ],
);

my %edges;
for my $e (@edges) {
    my ($src, $dest) = @$e;
    $edges{$src} //= [];
    push $edges{$src}, $dest;
}
### %edges

traverse(keys %edges);

sub do_index {
    my ($url) = @_;
    ### index: $url
}

sub get_links {
    my ($url) = @_;
    my $e = $edges{$url};
    return unless $e;
    return @$e;
}

# Only visit once per day (save most recent timestamp)
sub should_revisit {
    my ($url) = @_;
    state %visited;
    ### revisit? : $url
    if ($visited{$url} && $visited{$url} <= time() - 86400) {
        ### not revisiting young age: time() - $visited{$url}
        return;
    }
    # Mark visited as of now
    #     $visited{$url} = time(); # realistic example
    # Simulate having visited about a day ago
    $visited{$url} = time() - (86400 + (int(rand(4)) - 2));
    ### requeued at: $visited{$url}
    return 1;
}

sub traverse {
    my (@starts) = @_;
    my %visited;
    my @q;
    push @q, @starts;
    while (my $next = shift @q) {
        $visited{$next}++;
        do_index($next);
        my @e = get_links($next);
        for my $e (@e) {
            if (should_revisit($e)) {
                push @q, $e;
            }
        }
    }
}


