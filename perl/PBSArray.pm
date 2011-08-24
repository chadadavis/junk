#!/usr/bin/env perl

package PBSArray;

use base qw(Exporter);
our @EXPORT = qw(qsubarray);

# CPAN
use File::Spec;
use File::Temp qw(tempfile);
use File::Basename;

# Custom
use Utils;

# Number of sub-jobs allowed in an array by EMBL cluster policy
our $jlimit = 10000;

################################################################################

# Submit a file and script to process each separate line of the file
# Each line of the input file becomes one job, processed by the given script
# The script will be called with the name of the file and an index
# The index identifies the line number
sub qsubarray {
    my ($input_file, $process_script) = @_;
    return unless $input_file;
    # Rerun this same sript (with an index), if no alternative given
    $process_script ||= $0;

    # Absolutize paths
    $process_script = File::Spec->rel2abs($process_script);
    $input_file = File::Spec->rel2abs($input_file);

    # Count number of inputs (numer of sub jobs, cluster needs to know this)
    my $last = nlines($input_file) - 1; 

    # Arrays have a maximum size. Submit new jobs until exhausted
    my @jobs;
    my $start = 0;
    my $end = $jlimit-1;
    for (my $i=0,my $start=0,my $end=$jlimit-1;$start<=$last;$i++,$start+=$jlimit,$end+=$jlimit) {
        push @jobs, qsubblock($input_file, $process_script, $start, min($end,$last),$i);
    }

} # qsubarray


sub qsubblock {
    my ($input_file, $process_script, $start, $end, $i) = @_;

    my $name = basename($input_file) . "-${i}";
    my ($tmpfh, $jobscript) = tempfile("job_${name}_XXXXX");
    close $tmpfh;

# Write PBS array shell script
open my $jobfh, ">$jobscript";

# TODO add these:
# Won't take more than 60 seconds
# #PBS -l pcput=60
# #PBS -l walltime=60
# NB no simple way to concatenate stdout of array jobs

print $jobfh <<EOF;
#!/usr/bin/env sh
#PBS -N $name
#PBS -q clusterng\@pbs-master2
#PBS -M $ENV{USER}\@embl.de
#PBS -m ae
# This one PBS job script will be run from $start to $end , inclusive
#PBS -J ${start}-${end}
# Each of these will be passed to the script at the end of the command line:
# This variable PBS_ARRAY_INDEX is provided by the PBS environment

$process_script $input_file \$PBS_ARRAY_INDEX

EOF

    close $jobfh;
    
    my $cmd = "qsub $jobscript";
    print STDERR "$cmd\n"; 
#     my $job_arrary_id = `$cmd`;
    my $job_arrary_id;
    return $job_arrary_id;

} # qsubblock

