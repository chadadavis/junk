#!/usr/bin/env perl
package BST;
use Modern::Perl;
use Moose;
use Data::Dump qw(dump);

has value => (
    is => 'rw',
    isa => 'Int',
);

has [qw(left right)] => (
    is => 'rw',
    isa => 'Maybe[BST]',
);

sub insert {
    my ($self, $val) = @_;
    unless (defined $self) {
        return BST->new(value => $val);
    }
    unless (defined $self->value) {
        $self->value($val);
        return;
    }
    if (not 'recursive') {
        my $side = $val <= $self->value ? 'left' : 'right';
        my $ret = insert($self->$side, $val);
        $self->$side($ret) unless defined $self->$side;
    }
    else {
        my $cur = $self;
        while (1) {
            my $side = $val <= $cur->value ? 'left' : 'right';
            if (! defined $cur->$side) {
                $cur->$side(BST->new(value => $val));
                last;
            }
            else {
                $cur = $cur->$side;
            }
        }
    }
}

if ( __FILE__ eq $0 ) {
    my $t = BST->new();
    $t->insert($_) for (10,20,5,15,30);
    dump $t;
}

1;
