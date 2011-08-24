#!/usr/bin/env perl

# http://en.wikipedia.org/wiki/Affine_transformation

use strict;
use warnings;

use lib "..";
use EMBL::Table;
use EMBL::Point;
use EMBL::Sphere;

use Data::Dumper;

use PDL;
use PDL::Matrix;
use PDL::MatrixOps;
use PDL::IO::Misc;


approachb();

exit;

################################################################################

sub approacha {

    my $trans = `./transform.sh 2uzeA 2okrA`;
    # print "$trans\n";
    
    # Includes radius, but doesn't matter
    # my @cofm = (23.812, 20.757, -14.894, 20.163);
    my @cofm = (23.812, 20.757, -14.894);
    my $sphere = new EMBL::Sphere(@cofm);
    
    # my $table = new EMBL::Table($trans);
    my $table = table($trans);
    # print "Table:\n", Dumper($table), "\n";
    
    # my $transf = pdl $table;
    my $mtransf = mpdl $table;
    # print "transf:\n", $transf;
    print "mtransf:\n", $mtransf;
    
    # Add 1 for affine computation
    my $pcofm = mpdl (@cofm, 1);
    # Transpose row to a column vector
    $pcofm = transpose($pcofm);
    print "pcofm:$pcofm";
    
    my $z = mpdl zeroes(4,4);
    
    # print "z:$z";
    
    # unity matrix
    # set the elements along the diagonal to 1
    # (my $tmp = $e->diagonal(0,1)) .= 1; 
    
    $z->slice('0:2,0:3') += $mtransf;
    $z->slice('3,3') += 1;
    print "z:$z";
    
    # Finally, transform vect using matrix
    my $new = $z x $pcofm;
    print "new:$new";
    # print wcols(transpose $new);

}

sub approachb {

    # Try to read ASCII directly using PDL
    my $rasc = mpdl zeroes(4,4);
    $rasc->rasc('2uzeA-2okrA-FoR.trans');
    $rasc = transpose $rasc;
    $rasc->slice('3,3') += 1;
    print "rasc:$rasc";
    
    my @cofm = (23.812, 20.757, -14.894);
    # Add 1 for affine computation
    my $pcofm = mpdl (@cofm, 1);
    # Transpose row to a column vector
    $pcofm = transpose($pcofm);
    print "pcofm:$pcofm";
    
    # Finally, transform vect using matrix
    my $new = $rasc x $pcofm;
    print "new:$new";
    # print wcols(transpose $new);

}

# How to get this back as a perl list ? This doesn't work
# my @new = $new->at(0,1,2);
# print "new:",join(":", $new), "\n";




