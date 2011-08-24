#!/usr/bin/env perl

=head1 NAME

EMBL::Table - 

=head1 SYNOPSIS


=head1 DESCRIPTION



=head1 BUGS

None known.

=head1 REVISION

$Id: Prediction.pm,v 1.33 2005/02/28 01:34:35 uid1343 Exp $

=head1 APPENDIX

Details on functions implemented here are described below.
Private internal functions are generally preceded with an _

=cut

################################################################################

use lib "..";

package EMBL::Table;

require Exporter;
our @ISA = qw(Exporter);
# Automatically exported symbols
our @EXPORT    = qw(table);

use overload (
#     '""' => 'stringify',
    );


################################################################################
=head2 new

 Title   : new
 Usage   : 
 Function: 
 Returns : 
 Args    :

=cut

sub new {
    my ($class, $string) = @_;
    my $self = {};
    bless $self, $class;

    $self->{table} = table($string);
    return $self;
}

# Take a string and turns it into a table, assuming whitespace separated values
# Results is a 2D array ref
sub table {
    my ($str) = @_;
    my @lines = split(/\n+/, $str);
    s/^\s+// for @lines;
    my @table = map { [ split(/\s+/, $_) ] } @lines;
    return \@table;
}


sub stringify {
    my ($self) = @_;
    return join("; ", map { join(",", @$_) } @{$self->{table}});
}



###############################################################################

1;

__END__
