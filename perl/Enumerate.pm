#!/usr/bin/env perl

package Enumerate;
# use overload '++' => \&next_seq;

# Library used to generate all possible sequences of given length on an alphabet
use Math::BaseCalc;

# Use base conversion as analogy for "counting through" sequences
# This will iterate through all possible sequences of given length (n)
# E.g. 
# @alphabet = qw(a c g t);
# n=6 generates all nucleotides sequences of length 6
sub new { 
    my ($class, $n, @alphabet) = @_;
    my $self = {};
    bless $self;
    $self->calc(new Math::BaseCalc(digits=>[@alphabet]));

    # Start at smallest $alphabet-digit number. 
    # Avoid the leading-zero problem 
    # (i.e. the nucleotide '0001' is '   c', not a 4-mer), instead we want aaac
    $self->count(@alphabet ** $n);
    $self->i($self->count);
    return $self;
}

sub next_seq {
    my ($self) = @_;
    return undef unless $self->i < 2 * $self->count;
    my $calc = $self->calc;
    # Convert integer number to sequence of bases
    # Strip off leading character (effectively the leading 0)
    my $string = substr($calc->to_base($self->i), 1);
    $self->i($self->i + 1);
    return $string;
}

sub _reset {
    my ($self) = @_;
    $self->i($self->count);
}

sub AUTOLOAD {
    my ($self, $arg) = @_;
    our $AUTOLOAD;
    return if $AUTOLOAD =~ /::DESTROY$/;
    my ($pkg, $file, $line) = caller;
    $line = sprintf("%4d", $line);
    # Use unqualified member var. names,
    # i.e. not 'Package::member', rather simply 'member'
    my ($field) = $AUTOLOAD =~ /::([\w\d]+)$/;
    $self->{$field} = $arg if defined $arg;
    return $self->{$field} || '';
} # AUTOLOAD


1;

__END__
