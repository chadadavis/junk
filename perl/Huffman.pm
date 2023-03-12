#!/usr/bin/env perl
=head1

Generate a huffman code given a sample.
Save code for reuse.

Use that huffman code to encode inputs.

Use that huffman code to decode inputs.

=cut

# Sort chars by freqency. Must also know alphabet.
# Binary tree:
# While array has stuff in it, merge the last two nodes, 0 is left, 1 is right
# I.e. pop one node off stack and merge with root of tree

package Huffman;
use Moose;
use autodie;
use Test::More;
use Devel::Comments;
use Data::Dump qw(dump);
with 'MooseX::Getopt';

has 'tree' => (
    is => 'rw',
    lazy_build => 1,
);

has 'trainer' => (
    is => 'rw',
    isa => 'Str',
);

sub _build_tree {
    my ($self) = @_;
    my $input_f = $self->trainer;
    my $_table = $self->_table;
    open my $fh, '<', $input_f;
    my %dict;
    while (<$fh>) {
        chomp;
        my @chars = split //;
        $dict{$_}++ for @chars;
    }
    my @freq_chars = sort {
        $dict{$b} <=> $dict{$a}
    } keys %dict;
    my $next_val = pop @freq_chars;
    my $next_node = { v => $next_val };
    $_table->{$next_val} = $next_node;
    my $tree = $next_node;
    # This is a misunderstanding.  It's not the two least frequent nodes,
    # rather the new parent nodes sums the frequencies of the children. Should
    # be implemented as a heap, where a newly created node is re-inserted onto
    # the heap.
    while ($next_val = pop(@freq_chars)) {
        $next_node = { v => $next_val };
        $_table->{$next_val} = $next_node;
        $tree      = { l => $next_node, r => $tree, };
    }
    return $tree;
}

# Traversal from node to root is the reverse of the code to be used
sub encode {
    my ($self, $input_f) = @_;
    my $tree = $self->tree;
    open my $fh, '<', $input_f;
    while (<$fh>) {
        
    }
}

if ( __FILE__ eq $0 ) {
    my $h = Huffman->new_with_options();
    print $h->trainer;
    my $t = $h->tree;
    dump $t;
}
