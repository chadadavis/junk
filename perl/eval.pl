#!/usr/bin/env perl


use File::Temp qw(tempfile);

use lib "../..";
use EMBL::AssemblyIO;
use EMBL::Assembly;
# use EMBL::DomIO;
use EMBL::CofM;
use EMBL::STAMP; # stampfile

exit(main(@ARGV));

################################################################################

sub main {
    my ($modeldom) = @_;

    open my $fh, "<$modeldom";
    my $assio = new EMBL::AssemblyIO($fh);
    my $assem = $assio->next_assembly;
    my @components = keys %{$assem->{cofm}};

    my $refcofm;
    for my $label (@components) {
        print STDERR "$c: tainted:", $assem->cofm($label)->{tainted}, ":\n";
        if (! $assem->cofm($label)->{tainted}) {
            $refcofm = $assem->cofm($label);
            print STDERR 
                "ref:label:", $refcofm->label, ":id:", $refcofm->id, ":\n";
            last;
        }
    }

    # id is the template structure used
    # label is, when benchmarking, the PDBID of the native structure
    # So, this superposes the untransformed template onto the native chain
    # This provides a transformation that will be applied to entire model
    # This puts model into frame-of-reference of native complex structure
    my $trans = stampfile($refcofm->id, $refcofm->label);


}

__END__

    my $cmd;
    # Get a domain file for the true structure
    $cmd = "pdbc -d $pdbid > $pdbdom";
    `$cmd`;
    unless (-r $file && -s $pdbdom) {
        print STDERR "Failed: $cmd\n";
        return;
    }

    # Generate CofM (PDB format) files for two domain files (true, model)
    $cmd = "cofm -f $pdbdom > ${pdbdom}.cofm";
    `$cmd`;
    # TODO check file size

    $cmd = "cofm -f $modeldom > ${modeldom}.cofm";
    `$cmd`;
    # TODO check file size    

    # RMSD of two CofM files.
    # OR just do_stamp them?



}
