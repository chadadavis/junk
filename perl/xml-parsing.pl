#!/usr/bin/env perl

=head1 SYNOPSIS

Determine if a (simplified) XML document is well-formed, in terms of the tags used.

=head1 DESCRIPTION

For example This would be well-formed:

<place name="Europe">
  <place name="Netherlands">
  </place>
</place>

But, this is not:

<place name="Europe">
  <place name="Netherlands">
</place>

... because the tags are not all closed.

This is also well-formed:

<continent name="Europe">
  <country name="Netherlands">
  </country>
  <country name="France">
  </country>
</continent>

If it were not well-formed, could you identify where? (and if multiple tags per line?)

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
# use Devel::Comments;
use Test::More qw(no_plan);
use Getopt::Long;

my %strings = (
q(
<place name="Europe">
  <place name="Netherlands">
</place>
)
=> { success => 0, line => 3, got => '<none>', expected => '</place>' },
q(
<continent name="Europe">
  <country name="Netherlands">
  </country>
  <country name="France">
  </country>
</continent>
)
=> { success => 1},
q(
<continent name="Europe">
  <country name="Netherlands">
  </country>
  </place>
  <country name="France">
</country>
</continent>
)
=> { success => 0, line => 3, got => q(</place>), expected => q(</continent>) },
q(
<a></a>
</place>
)
=> { success => 0, line => 1, got => q(</place>), expected => q(</none>) },
q(
<a>
)
=> { success => 0, line => 1, got => q(<none>), expected => q(</a>) },
q(
<a><b>
)
=> { success => 0, line => 1, got => q(<none>), expected => '</b></a>', },
);

while ( my ($k, $v) = each %strings ) {
    my $res = consume($k);
    is_deeply($res, $v);
}

sub consume {
    my $doc = shift;
    #### $doc
    my $stack = [];
    my $line_n = 0;
    while ($doc =~ /^\s*(?<line>.*)\s*$/mg) {
        my $line = $+{line};
        ### $line_n
        ### $+{line}
        ### $stack
        while ( $line =~ m{<(?<closed>/?)(?<name>\w+)(.*?)>}g ) {
            #### $+{closed}
            #### $+{name}
            if ( $+{closed} ) {
                my $expected = pop($stack) || 'none';
                if ( $expected ne $+{name} ) {
                    return {
                        success  => 0,
                        line     => $line_n,
                        got      => "</$+{name}>",
                        expected => "</$expected>",
                    };
                }
            }
            else {
                push $stack, $+{name};
                ### push: $+{name}
            }
            ### $stack
        }
        $line_n++;
    }
    if ( @$stack ) {
        return {
            success  => 0,
            line     => $line_n,
            got      => '<none>',
            expected => join( '', map { "</$_>" } reverse @$stack ),
        };
    }
    return { success => 1 };
}
