#!/usr/bin/env perl

use File::Basename;
use IO::Uncompress::Gunzip qw(gunzip $GunzipError) ;
use Storable qw(nstore retrieve); # For saving object representations

for my $file (</home/data/pdb/pdb3*.ent.gz>) {

    my $name = basename($file);
    my $cached = "${name}.bin";

    unless (-r $cached) {
        # Parse file into some datastructure and then name it $name at save it
        parse($file, $cached);
    }
    
    # Process the binary representation of the data structure
    if (-r $cached) {
        process($cached);
    } else {
        die;
    }
}

exit;

################################################################################

sub process {
    print STDERR "process\n";
    my ($cached) = @_;
    # Load cached object from disk
    my $obj = retrieve $cached;

    my @a = @{$obj->[0]};
    for (my $i = 1; $i < @$obj; $i++) {
        my @b = @{$obj->[$i]};
        @a = map { ($a[$_] + $b[$_])/2 } 0..2;
    }
    print "a:@a\n";
}

sub parse {
    print STDERR "parse\n";
    my ($file, $dest) = @_;
    my $z = new IO::Uncompress::Gunzip $file
        or die "gunzip failed: $GunzipError\n";
    my @coords;
    while (my $line = $z->getline()) {
        next unless $line =~ /^ATOM/;
        my ($x,$y,$z) = (
                         substr($line,31,8), 
                         substr($line,39,8), 
                         substr($line,47,8),
                         );
        push @coords, [$x,$y,$z];
    }
    nstore(\@coords, $dest);
}

