#!/usr/bin/env perl

my $pdbid = shift;
$pdbid or die("Usage: $0 <4-char_pdbid | pdb_file> [chain_id]\n");
my $chain = shift || "";

our $base = '/g/data/pdb/';
# If the parameter was a readable file open that, else assume a 4-letter ID
our $file = (-r $pdbid) ? $pdbid : "${base}/pdb${pdbid}.ent";

open my $fh, "<${file}";

# Map atom IDs to molecular weight
my %wts = (
    C => 12.01115,
    H => 1.0079,
    N => 14.0067,
    O => 15.9994,
    P => 30.9738,
    S => 32.064,
    );

my $sum = 0; 

while (<$fh>) {
    # Atom records
    next unless (/^ATOM/);
    # If chain defined, must match
    next if ($chain && $_ !~ /^.{21}$chain/);

    # Atom type (without any trailing number, e.g. O not OP and not OP3' )
    my ($atom);

    # These are all hydrogen or variants thereof
    if (/^.{12}H/ || /^.{12}DD/) {
        $atom = 'H';
    } elsif (/^.{13}([A-Z])/) {
        $atom = $1;
    } else {

    }
    my $wt = $wts{$atom};
#     $wt or print STDERR "No $atom type\n";
    $wt or print STDERR "$pdbid: No $atom type\n";        
#     print  "$chain $atom $wt\n";
    $sum += $wt;
}

close $fh;

printf "%4s %1s %6.2f KDa\n", $pdbid, $chain, $sum / 1000;

