#!/usr/bin/env perl

=head1 NAME
sub_graph.pl - perl subroutine call graph generator

=head1 SYNOPSIS

sub_graph.pl <options> my_perl_files
sub_graph.pl -help

=head1 VERSION 0.2

July, 2000 by Alan Ferrency (alan@ferrency.com)

=head1 DESCRIPTION

sub_graph.pl Analyzes a list of (perl) files specified on the command
line using B::Xref, and outputs a .dot format calling graph, with
nodes corresponding to perl subs, and edges corresponding to
subroutine calls from one subroutine to another.  The .dot file
format is used by AT&T's free GraphViz software to draw the specified
graph.  dot, part of GraphViz, can be used to create .ps or .gif
format representations of the calling graph.

=cut


use strict;

# Make some options.  Too bad this is as long as the rest of the code.
use Getopt::Simple qw($switch);
my $options = {
    'help|h|?' => {
        'type' => '',
        'env' => '',
        'default' => '',
        'order' => 1,
        'verbose' => 'This message'
        },
    'output_file' => {
        'type' => '=s',
        'default' => '-',
        'verbose' => 'Output filename',
        'order' => 2,
        'env' => ''
        },
    'header_file' => {
        'type' => ':s',
        'env' => '',
        'default' => 'graph.head',
        'order' => 2,
        'verbose' => 'File to prepend to .dot output'
        },
    'skips_file' => {
        'type' => ':s',
        'env' => '',
        'default' => 'graph.skips',
        'order' => 2,
        'verbose' => "File with a list of files we shouldn't parse"
        },   'skip_nonperl_files' => {
        'type' => '!',
        'env' => '',
        'default' => 1,
        'order' => 6,
        'verbose' => "Skip files whose first line doesn't m|^\#\!.*/perl|"
    },
    'graph_name' => {
        'type' => '=s',
        'env' => '',
        'default' => 'mygraph',
        'order' => 3,
        'verbose' => 'name of dot graph'
        },
    'special_nodes_fn' => {
        'type' => '=s',
        'env' => '',
        'default' => 'graph.special',
        'order' => 4,
        'verbose' => 'file containing a list of "special" nodes (subroutines).'
        },
    'special_node_attr' => {
        'type' => '=s',
        'env' => '',
        'default' => '[color=grey,style=filled]',
        'order' => 5,
        'verbose' => 'dot attributes to use on every "special" node'
        },
    'perl' => {
        'type' => '=s',
        'env' => 'PERL',
        'default' => '/usr/bin/perl',
        'order' => 3,
        'verbose' => 'where does perl live?  (used as $PERL -B::Xref <fname>)'
        },
    'verbose' => {
        'type' => '+',
        'env' => '',
        'default' => 0,
        'order' => 3,
        'verbose' => 'Talk a lot, but say very little.'
    },
    'special_edges_only' => {
        'type' => '!',
        'env' => '',
        'default' => 0,
        'order' => 7,
        'verbose' => "Display only edges to and from nodes in the special node list.\n\tThis takes priority over from_special_no
ds_only and to_special_nodes_only."
    },
    'from_special_nodes_only' => {
        'type' => '!',
        'env' => '',
        'default' => 0,
        'order' => 8,
        'verbose' => 'Display only edges from a node in the special node list'
    },
    'to_special_nodes_only' => {
        'type' => '!',
        'env' => '',
        'default' => 0,
        'order' => 9,
        'verbose' => 'Display only edges to a node in the special node list'
    },    'no_internal_edges' => {
        'type' => '!',
        'env' => '',
        'default' => 0,
        'order' => 10,
        'verbose' => 'Use this to suppress drawing of edges between two special nodes\n\t(edges internal to the special node sub
graph)'
    },
    'special_subgraph' => {
        'type' => '!',
        'env' => '',
        'default' => 0,
        'order' => 7,
        'verbose' => "Build special nodes into a subgraph, with optional attributes as specified"
        },
    'subgraph_attributes' => {
        'type' => '=s',
        'env' => '',
        'default' => 0,
        'order' => 7,
        'verbose' => "Attribute codes for special subgraph"
        }

};

my $o = new Getopt::Simple;
exit (-1) if (! $o->getOptions($options, "Usage: $0 [options]"));

# Everything else is a list of perl files to parse.

my @files = @ARGV;

# If we are told, skip all nonperl files

if ($switch->{'skip_nonperl_files'}) {
    @files = grep {
        # implicitly and silently skip files we can't open.
        open F, "$_" and <F> =~ m|^\#\!.*perl|;
    } @files;
}

# Implicitly skip tilde and hash files

@files = grep {!/~$|^#.*#$/} @files;

# read in a list of files to skip, and skip them.

if (-f $switch->{'skips_file'}) {
    my %skip;
    open F, "$switch->{'skips_file'}" or die "Can't open $switch->{'skips_file'} for reading";
    while (<F>) {
        chomp;
        foreach (split /\s+/) {
            $skip{$_}++;
        }
    }
    @files = grep { !$skip{$_} } @files;
}

print "I will analyze the following files:\n". join ("\n", @files) if $switch->{'verbose'};


# Read in "special" nodes list

my %want;
if (-f $switch->{'special_nodes_fn'}) {
    open F, "$switch->{'special_nodes_fn'}" or die "Can't open $switch->{'special_nodes_fn'} for reading";
    while (<F>) {
        chomp;
        foreach (split /\s+/) {
            $want{$_}++;
        }
    }
}

# Next, analyze Xref output for each file specified.

my $cur_sub;  # name of the sub we're currently "in"
my $cur_pkg;  # name of the package, from which cur_sub is calling a method
# HoH, where exists($edges{A}->{B}) means we have an edge from A to B
my %edges;

# Build a HoH with edge counts.
for my $f (@files) {
    my ($fbase) = $f =~ m|.*/(.*?)\.p[ml]$|i;
    # Xref will print "syntax OK" or errors to STDERR; we don't mind.
    open X, "$switch->{'perl'} -MO=Xref $f|";
    while (<X>) {
        chomp;
        print "--Xref: $_\n" if $switch->{'verbose'} > 2;
        if (/Subroutine \(?([^()]*)/) {
            $cur_sub = $1;
            $cur_sub = "${fbase}__main" if $cur_sub eq "main";
            $cur_sub =~ tr/.:/__/; # dot doesn't like punctuation.
        } elsif (/Package\s+(\S+)/) {
            $cur_pkg = $1;
            $cur_pkg =~ tr/.:/__/; # dot doesn't like punctuation.
        } elsif (/^\s*\&(\w\S+)\s*/) {
            next if $cur_sub eq "definitions"; 
#             next if $cur_pkg eq "(lexical)";
            my $s = $1;
            $s =~ s/:/_/g;
            $s = "${cur_pkg}__${s}";
            $edges{$cur_sub}->{$s}++;
        }
    }
}


# Output a .dot file

open DOT, ">$switch->{'output_file'}"
    or die "Can't open $switch->{'output_file'} for writing";

# First, start the graph
print DOT "digraph $switch->{'graph_name'} {\n";

# If we have a header file, stick it in
if (-f $switch->{'header_file'}) {
    open HEAD, "$switch->{'header_file'}"
        or die "Can't open $switch->{'header_file'} for read";
    while (<HEAD>) { print DOT; }
    close HEAD;
}


# Next, give Wanted Nodes special treatment.  As a side effect, these
# nodes will always show up on the graph even if no edges touch them.

map { print DOT "$_$switch->{'special_node_attr'};\n" } keys %want;

# Build a special subgraph if we request it
if ($switch->{'special_subgraph'}) {
    print DOT "{$switch->{'subgraph_attributes'};";
    map { print DOT " $_" } keys %want;
    print DOT "}\n";
}
# set up some temporary variables for brevity and speed

# we want all the edges unless specified otherwise

my $want_all = !$switch->{'special_edges_only'} &&
    !$switch->{'from_special_nodes_only'} &&
        !$switch->{'to_special_nodes_only'};

my $want_from = $switch->{'special_edges_only'} ||
    $switch->{'from_special_nodes_only'};

my $want_to = $switch->{'special_edges_only'} ||
    $switch->{'to_special_nodes_only'};

my $want_int = !$switch->{'no_internal_edges'};

if ($switch->{'verbose'}) {
    print "I am" . (!$want_all && " not") ." printing all edges\n";
    print "I am" . (!$want_to && " not") ." printing edges to special nodes.\n";
    print "I am" . (!$want_from && " not") ." printing edges from special nodes.\n";
    print "I am" . (!$want_int && " not") ." printing edges between special nodes.\n";
}

for my $n1 (sort keys %edges) {
    foreach my $n2 (sort keys %{$edges{$n1}}) {
        print DOT "   $n1 -> $n2;\n" if
            $want_all ||
                ($want{$n1} && $want_from && ($want_int || !$want{$n2})) ||
                        ($want{$n2} && $want_to && ($want_int || !$want{$n1}));
        # Announce large edge counts if we're talking too much
        if ((my $n = $edges{$n1}->{$n2}) > 1) {
            print "There are $n edges between $n1 and $n2\n" if $switch->{'verbose'} > 1;
        }
    }
}


print DOT "}";
close DOT;
__END__

