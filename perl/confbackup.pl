#!/usr/bin/env perl

# Basic backup program for system config files (i.e. stuff under /etc )
# Backs up files and directories (recursively).
# File system structure is built up as needed at backup destination.
# Unchanged files are identified (with diff) and won't be backed up.
# When invoked with -r and the archived file,
#   previously backed up file are restored.

# Location of backup archive
our $destbase = "/home/davis/misc/conf/systems/ubuntu";


################################################################################

use strict;
use warnings;
use File::Spec;
use File::Copy;
use File::Copy::Recursive qw(rcopy);
use File::Basename;
use File::Path;

# Copy files, instead of Symlinks
# Not what I'd prefer, but doesn't work otherwise
$File::Copy::Recursive::CopyLink = 0;

exit(main());

################################################################################

sub main {

    if ($ARGV[0] =~ /^-u/) {
        # Freshen archive to include most recent copies from live file system
        print "Update:\n";
        return update();
    } elsif ($ARGV[0] =~ /^-r/) {
        # Restore the files. Path given should be original file system path.
        print "Restore:\n";
        # Shift off the '-r'
        shift @ARGV;
        restore($_) for @ARGV;
    } else {
        backup($_) for @ARGV;
	}
    return 0;
}

sub swap (\$\$) { my ($a, $b) = @_; my $c = $$a; $$a = $$b; $$b = $c; } 

sub backup {
    my ($target) = @_;
    print "Backup: $target\n";
    my ($f, $dest) = paths($target);
    unless ($f && -r $target) {
        return;
    }        
	rcopy($f, $dest) or 
        print STDERR "Cannot copy $f -> $dest\n  $!\n";
}

sub restore {
    my ($p) = @_;
    my ($f, $dest) = paths($p);
    unless ($f && -r $dest) {
        return;
    }        
    unless (rcopy($dest, $f)) {
        print STDERR "Cannot copy $dest -> $f\n  $!\n";
        return;
    }
}

sub diff {
    my ($a, $b) = @_;
 	if (-r $b && -r $a && system("diff -q $a $b > /dev/null 2>&1") == 0) {
		print "  Unchanged\n";
        return 0;
	} else {
        return 1;
    }
}

# Get relative paths and paths to archived version of a file
sub paths {
    my ($p) = @_;
    print "  Local  : $p\n";
	# Get absolute path
	$p = File::Spec->rel2abs($p);
	# Where to archive it
	my $dest = "$destbase/$p";
	print "  Archive: $dest\n";
	# What directory that will be
	my $destdir = dirname($dest);
	# Make sure it exists
	mkpath($destdir, 1, 0755);
	# If it's a regular file, check whether it's already backed up
    return unless diff($p, $dest);
    return ($p, $dest);
}

sub update {
    for (`/usr/bin/find $destbase -type f`) {
        chomp;
        # Don't copy any svn admin directories
        next if $_ =~ /[.]svn/;
        s/$destbase//;
        backup($_);
    }

}
