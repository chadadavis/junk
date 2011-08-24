#!/usr/bin/env perl

=head1 NAME

hepp::Fasta - Class for parsing Fasta files

=head1 SYNOPSIS

use hepp::Fasta;

# Create a new Fasta object
my $obj = new hepp::Fasta(-file => "/home/abc/genome.fa");

my ($head, $seq) = $obj->next_seq();

=head1 DESCRIPTION

This class represents a Fasta file. It's a simple wrapper for easily retrieving
sequences from a Fasta file, sequentially.

=head1 BUGS

o This program assumes that FASTA format means that each sequence header begins
  with '>' as the first character on the line. Blank lines (consisting only of
  whitespace characters, e.g. space, tab, newline) are ignored. Lines whose
  first non-whitespace character is a '#' are considered comments and also
  ignored. All other lines are expected to begin with (after any leading
  whitespace) one of A,C,G,T,N, or X (either lower or upper case). Whitespace is
  subsequently stripped from sequences. Any other characters in the input
  produce an error. The header line may, however, contain any characters, as
  long as it begins with '>' and ends with a new line.

o Whether or not this behaviour truly constitutes a 'bug' depends on your
  definition of FASTA format.

=head1 REVISION

$Id: Fasta.pm,v 1.6 2005/09/19 12:08:06 davis Exp $

=head1 APPENDIX

Details on functions implemented here are described below.
Private internal functions are generally preceded with an _

=cut

################################################################################

package Fasta;

use Utils;


################################################################################
=head2 new

 Title   : new
 Usage   : my $obj = new hepp::Fasta(-file => "/home/abc/genome.fa");
 Function: Create a new instance of a Fasta object
 Returns : New Fasta object
 Args    :
           I<-fh> A handle to an already opened file (default: stdin)
           I<-file> Path to a file to be opened and read

=cut

sub new {
    my ($class, @args) = @_;
    my $self = {};
    bless $self, $class;

    # Get init. params.
    (
     $self->{'file'}, 
     $self->{'fh'}, 
     ) = 
     rearrange([qw(
                   FILE 
                   FH 
                   )], 
               @args);

    # If we got a ref it's probably really a FH (i.e. a glob ref)
    if (ref $self->file) { $self->fh($self->file); $self->file('') }
    # Otherwise assume it's a file and try to open it
    if ($self->file) {
        verbose "Opening: ", $self->file;
        open $self->{'fh'}, "<", $self->file();
        if (! $self->fh) { 
            verbose "Failed to open: ", $self->file; 
            return;
        }
        # Note that we opened the file so that we can close it later
        $self->opened(1);
    }
    $self->fh(\*STDIN) unless $self->fh;
    return $self;

} # new


###############################################################################

=head2 next_seq

 Title   : I<next_seq>
 Usage   : ($head, $seq) = $obj->next_seq();
 Function: Returns the next nucleotide sequence from a FASTA file.
 Returns : I<head> Header line (no new line) 
           I<seq> Sequence, as a single line
 Args    : I<None>

=cut

sub next_seq {
    my ($self, @args) = @_;

    my $fh = $self->fh();
    my $head = $self->next_head();
    my $seq = '';

    # Read all lines
    while (<$fh>) {
        chomp;
        # Ignore blank lines and interpret lines beginning with a # as comments
        next unless $_ && $_ !~ /^\s*#/;
        if (/^\s*>/) { 
            # This is the start of a new sequence
            # If we're in the middle of reading an existing sequence, return it.
            if ($head) {
                # Save new header in object for next call to this function
                $self->next_head($_);
                $self->head($head);
                $self->seq($seq);
                $self->length(length($seq));
                return ($head, $seq);
            }
    
            # Start parsing a new sequence
            $head = $_;
            $seq = '';
            next;
        } else {
            # This is a sequence data line. Append to the running sequence.
            # First remove any white space in the sequence data
            $_ =~ s/\s//g;
            # If it contains invalid characters, replace them with 'X'
#            if (! /^[acgtnx]+$/i) {
#                verbose "Replacing invalid characters with 'X' in line:\n$_\n";
#                $_ =~ s/[^acgtnx]/X/ig;
#            }
            # Make the length of the lines in the output as long as the input
            $self->line_len(max($self->line_len(), length $_));
            # Concatenate currently read line to running sequence
            $seq .= $_;
        }
    }
    # The final sequence is not followed by a header
    $self->head($head);
    $self->seq($seq);
    $self->length(length($seq));
    if ($head && $seq) { return ($head, $seq) } else { return }

} # next_seq


###############################################################################

=head2 write_seq

 Title   : I<write_seq>
 Usage   : $obj->write_seq($head, $seq);
 Function: Prints the given sequence with its header, using the original format
 Returns : I<Success> Boolean
 Args    :
           I<-head> Overrides header line (no new line) 
           I<-seq> Overrides equence (single line)
           I<-line_len> Overrides sequence line length
           I<-fh> File handle to print to, otherwise STDOUT

The length of the output lines is the same as the length of the longest input
line read by this object, not counting headers.

=cut

sub write_seq {
    my ($self, @args) = @_;

    my ($head, $seq, $line_len, $fh) = 
        rearrange([qw(HEAD SEQ LINE_LEN FH)], @args);
    $line_len ||= $self->line_len;
    $head ||= $self->head;
    $seq ||= $self->seq;
    $fh ||= *STDOUT;
    
    # This splits the sequence into a series of lines of max. length $line_len
    print $fh "$head\n", join("\n", $seq =~ /(.{1,$line_len})/g), "\n" or 
        die("Cannot write to output file.\n$!\n$@\n");

} # write_seq 



################################################################################
=head2 DESTROY

 Title   : I<DESTROY>
 Usage   : I<Not be called directly>
 Function: Class cestructor
 Returns : I<Nothing>
 Args    : I<None>

Closes internal file handle, if open.

=cut

sub DESTROY {
    my ($self, $args) = @_;
    verbose;
    close $self->fh if $self->opened;
}


################################################################################
=head2 AUTOLOAD

 Title   : I<AUTOLOAD>
 Usage   : $obj->member_var($new_value);
 Function: Implements get/set functions for member vars. dynamically
 Returns : Final value of the variable, whether it was changed or not
 Args    : I<value> New value of the variable, if it is to be updated

Overrides built-in AUTOLOAD function. Allows us to treat member vars. as
function calls.

=cut

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


###############################################################################

1;

__END__
