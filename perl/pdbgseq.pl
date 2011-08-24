#!/usr/bin/env perl

=head1 NAME

B<ensemble.pl> - Credo ensemble algorithm

=head1 SYNOPSIS

ensemble.pl CredoResults/

ensemble.pl CredoResults/ -o <new_output_dir> -m <modelfile.model>

=head1 DESCRIPTION

Runs the Credo Ensemble algorithm on previously generated Credo
predictions. These predictions are saved in Credo's XML output files (unless
they were disabled). These motif predictions provide enough information to
generate new ensemble predictions from the original motif predictions, without
needing to rerun Credo.

See also: B<Ensemble> package

=head1 REQUIRES


=head1 OPTIONS

=head2 -h Print this help page

=head2 -o Directory to save output XML files to. Created if necessary.

=head2 -m SVM model to use. Relative to the 'svm-models' directory of Credo

=head1 AUTHOR

 Chad Davis <chad.davis@embl.de>


=cut

################################################################################

use strict;
use warnings;
use Getopt::Long;
use Pod::Usage;
use File::Basename;

our $offset = 100;
our @res1 = qw/A C D E F G H I K L M N P Q R S T V W Y/;
our @res3 = qw/ALA CYS ASP GLU PHE GLY HIS ILE LYS LEU MET ASN PRO GLN ARG SER THR VAL TRP TYR/;
our %res3to1 = map { $res3[$_] => $res1[$_] } 0..$#res1;

my %ops;
my $result = GetOptions(\%ops, 
                        'h|help',
    );
if ($ops{'h'}) { pod2usage(-exitval=>1, -verbose=>2); }

my $id = shift or pod2usage(-exitval=>1);
my ($pdbid, $chainid, $file);

unless (-r $id) {
    ($pdbid, $chainid) = $id =~ /(.{4})(.)?/ or pod2usage(-exitval=>1);
    $file = "/g/data/pdb/pdb${pdbid}.ent";
} else {
    $file = $id;
    ($pdbid) = $file =~ /(.{4})\..*$/;
    $pdbid ||= basename($file, qw/.pdb .pdb.gz .brk .brk.gz .ent .ent.gz/);
}
$chainid ||= shift || '.';
@ARGV = ($file);
unless (-r $ARGV[0]) {
    print STDERR "Cannot open $ARGV[0]\n";
    exit;
}

# All these are indexed by chainid
# Hash, indexed by chainid, of ArrayRef of sequences
my %seqs;
my %max;
my %min;
my %gaps; 

while (<>) {
    next unless /^ATOM.........CA..(.{3}).(${chainid})(.{4})/;
    my ($res1, $chain, $resid, );
    $res1 = $res3to1{$1};
    $chain = $2;
    unless ($res1) {
        print STDERR "$id: Unknown residue: $1 at $3 in chain $chain\n";
        next;
    }

    # Pad with an extra cell left (for 1-based counting)
    $seqs{$chain} ||= [ ('X') x ($offset+1) ];
    $gaps{$chain} ||= 0;
    $max{$chain} ||= $3;
    $min{$chain} ||= $3;
    $min{$chain} = $3 if $3 < $min{$chain};
    $max{$chain} = $3 if $3 > $max{$chain};
    $resid = $3 + $offset;
    my $exist;
    $exist = $seqs{$chain}[$resid];
    if (defined $exist && $exist ne 'X' && $exist ne $res1) {
        print STDERR 
            "$id: Overwriting $seqs{$chain}[$resid] with $res1 ",
            "at $3 on chain $chain\n";
    }
    $seqs{$chain}[$resid] = $res1;
}
# Fill gaps
foreach my $k (keys %seqs) {
    my $seq = $seqs{$k};
    $seqs{$k} = [map { if (defined $_) { $_ } else { $gaps{$k}++; 'X'} } @$seq];
    # Use 1-based counting
    shift @{ $seqs{$k} };
    # Convert to numbers:
    $min{$k} = 0+$min{$k};
    $max{$k} = 0+$max{$k};
    print 
        ">${pdbid}${k} min: $min{$k} max: $max{$k} ", 
        "padded: $offset gaps: $gaps{$k}\n", join('', @{$seqs{$k}}), "\n";
}


