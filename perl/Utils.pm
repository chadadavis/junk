#!/usr/bin/env perl

package Utils;
use base qw(Exporter);
our @EXPORT = qw(
                 verbose
                 min
                 max
                 sum
                 avg
                 median
                 stddev
                 sequence
                 rearrange
                 slurp
                 spit
                 nlines
                 linei
                 nhead
                 run
                 put
                 wlock
                 nsort
                 overlap
                 swap
                 );

use File::Temp qw(tempfile);
use Term::ANSIColor qw(:constants);
use File::Basename;
use File::Spec::Functions qw/rel2abs catdir/;


################################################################################

# Print diagnostic/debugging messages
sub verbose  {
    return unless defined($::DEBUG) && $::DEBUG;
    my ($pkg, $file, $line, $func) = caller(1);
    $line = sprintf("%4d", $line);
    print STDERR 
#         BLUE, ">$file|$line|$func|", 
        BLUE, ">$pkg|$line|$func|", 
        GREEN, "@_\n", 
        RESET;
}

# Minimum of a list
sub min {
    my $x = shift @_;
    $x = $_ < $x ? $_ : $x for @_;
    return $x;
}

# Maximum of a list
sub max {
    my $x = shift @_;
    $x = $_ > $x ? $_ : $x for @_;
    return $x;
}

# Sum of a list
sub sum {
    my $x = 0;
    $x += $_ for @_;
    return $x;
}

# Average of a list
sub avg {
    # Check if we were given a reference
    my $r = $_[0];
    my @list = (ref $r) ? @$r : @_;
    return 0 unless @list;
    my $sum = 0;
    $sum += $_ for @list;
    return $sum / @list;
}

# Median, 1st quartile, 3rd quartile, etc of a list
# p should be (0:1)
sub percentile {
    my ($p) = shift @_;
    # Check if we were given a reference
    my $r = $_[0];
    my @list = (ref $r) ? @$r : @_;
    return undef unless @list;
    @list = sort { $a <=> $b } @list;
    return $list[int($p * @list)];
}

sub median {
    return percentile(.50, @_);
}

# numeric sort
sub nsort {
    return sort {$a <=> $b} @_;
}


# length of overlap of two segments on a number line
sub overlap {
    my ($astart, $aend, $bstart, $bend) = @_;
    return undef unless 
        defined($astart) && defined($aend) && 
        defined($bstart) && defined($bend);
    # Put segments in left-to-right order
    ($astart, $aend) = nsort($astart, $aend) if $aend < $astart;
    ($bstart, $bend) = nsort($bstart, $bend) if $bend < $bstart;
    
    # This is negative when no overlap.
    # E.g. -2 means, the segments are two units apart, 0 means back-to-back
    return min($aend, $bend) - max($astart, $bstart);
}

# Stddev of a list
sub stddev {
    # Check if we were given a reference
    my $r = $_[0];
    my @list = (ref $r) ? @$r : @_;
    return 0 unless @list;
    my $sum = 0;
    my $avg = avg(\@list);
    for (my $i = 0; $i < @list; $i++) {
        $sum += ($list[$i] - $avg)**2;
    }
    return sqrt($sum/(@list - 1));
}

# Creates a sequence of numbers (similar to in R)
sub sequence {
    my ($start, $inc, $end) = @_;
    my @a;
    for (my $i = $start; $i <= $end; $i+=$inc) {
        push @a, $i;
    }
    return @a;
}

# Support for named function parameters. E.g.:
# func(-param1=>2, -param3=>"house");
sub rearrange  {
    # The array ref. specifiying the desired order of the parameters
    my $order = shift;
    # Make sure the first parameter, at least, starts with a -
    return @_ unless (substr($_[0]||'',0,1) eq '-');
    # Make sure we have an even number of params
    push @_,undef unless $#_ %2;
    my %param;
    while( @_ ) {
        (my $key = shift) =~ tr/a-z\055/A-Z/d; #deletes all dashes!
        $param{$key} = shift;
    }
    map { $_ = uc($_) } @$order; # for bug #1343, but is there perf hit here?
    # Return the values of the hash, based on the keys in @$order
    # I.e. this return the values sorted by the order of the keys
    return @param{@$order};
} # rearrange

# Slurps the contents of a (unopened) file (given path to file) into a string
sub slurp {
    my ($file) = @_;
    local $/;
    open my $fh, "<$file";
    my $data = <$fh>;
    close $fh;
    return $data;
}

# Spits anything into a file, creating/overwriting that file (given path)
sub spit {
    my ($data, $file) = @_;
    my $fh;
    # File path was given, otherwise open a tempfile
    if ($file) {
        open $fh, ">$file" or die($!);
    } else {
        ($fh, $file) = tempfile();
    }
    print $fh $data;
    close $fh;
    return $file;
}

# Returns number of lines in file
sub nlines {
    my ($file) = @_;
    open my $fh, "<$file";
    my $i;
    for ($i = 0; <$fh>; $i++) {}
    close $fh;
    return $i;
}

# Returns line number i from some file (0-based counting)
sub linei {
    my ($file, $i) = @_;
    # Open for reading only
    open my $fh, "<$file";
    # Read over the first $i - 1 lines
    for (my $j = 0; $j < $i; $j++) { <$fh> or return;}
    # Read line number i
    my $line = <$fh>;
    chomp $line;
    close $fh;
    return $line;
}


# Pop the first n lines of a file, and return them (useful for removing headers)
# File is re-written in-place w/o those lines
sub nhead {
    my ($n, $file) = @_;
    return 1 unless -r $file && -w $file;
    open FH, "<$file";
    my @lines = <FH>;
    close FH;
    # if $n negative, chop off all but the last |$n| lines
    if ($n < 0) { $n += @lines; }
    # The lines to be removed
    my @pop = @lines[0..$n-1];
#     verbose("Chopping first $n lines:\n", @pop);
    # The rest
    @lines = @lines[$n..$#lines];
    # write back out
    open FH, ">$file";
    print FH @lines;
    close FH;
    return @pop;
}

# Run a shell command on given input file, returns path to output file
sub run {
    my ($in, $cmd) = @_;
    my ($fh, $out) = tempfile("/tmp/disoconsXXXXXXXXXX", UNLINK=>!$::DEBUG);
    close $fh;
    # Use eval to substitute the current values of $in and $out
    $cmd = eval "\"$cmd\"";
    verbose("cmd:\n$cmd");
    unless (system($cmd) == 0) {
        print STDERR "Failed to run:\n\t$cmd\n";
        return 0;
    }
    return $out;
}

# Prints an arry with indices in tabular form in a temp text file, for gnuplot
sub put {
    my ($array) = @_;
    my ($fh, $out) = tempfile("/tmp/disoconsXXXXXXXXXX", UNLINK=>!$::DEBUG);
    for (my $i = 1; $i < @$array; $i++) {
        print $fh "$i ", $array->[$i], "\n";
    }
    close $fh;
    return $out;
}

=head2 wlock

 Title   : wlock
 Usage   : my $lockfile = wlock("file.txt", 10); unlink $lockfile;
 Function: Sets a lock (as symlink) on the given file
 Returns : The link to the lock file, if it could be created, otherwise nothing
 Args    : The file that is to be exclusively opened.
           The number of attempts to try to lock the file. Default 5.

Routine fails, returning nothing, if it cannot create the lock within the given
number of attempts.

If you to test a lock without waiting, just set the number of attempts to 1.
Otherwise the function will sleep for a second between each attempt to give any
other processes a chance to finish what they are doing before attempting to lock
the file again.

The lock file returned by this method must be deleted by the caller after the
lock is no longer needed. E.g.:

my $database = "mydatabase.dat";
my $lockfile = wlock($database);
open (DB, ">$database");
# Read or write the file
close DB;
# Release the lock, so that other processes can access the data file
unlink $lockfile;

=cut

sub wlock {
    my $file = shift or return;
    my $attempts = shift || 5;
    my $lock = "${file}.lock";
    until (symlink("$ENV{HOSTNAME}-$$", $lock)){
        $attempts--;
        return unless $attempts > 0;
        sleep 1 + int rand(3);
        my $full = rel2abs($lock);
        print STDERR "Waiting for lock: $full ($attempts)\n";
    }
    return $lock;
} # wlock

# Intended to be used with references
sub swap (\$\$) {
    my ($a, $b) = @_;
    my $c = $$a;
    $$a = $$b;
    $$b = $c;
}

################################################################################
1;
__END__

