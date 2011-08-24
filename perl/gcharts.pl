#!/usr/bin/env perl

=head1

 http://code.google.com/apis/chart/docs/gallery/scatter_charts.html

=cut

use strict;
use warnings;

use Google::Chart;
use PDL::Lite;
use PDL::IO::Misc qw/rcols/;

my $csvfile = shift || die;

# Leave undefined to read all lines
my $nlines = shift || '';

# Skip column headers
my $inputlines = "1:$nlines";

$PDL::IO::Misc::colsep = "\t";

my (undef, $rmsd, $score, $mndoms) = 
    rcols($csvfile, 
          [],
          {
              PERLCOLS => [0..2],
              LINES => $inputlines,
              EXCLUDE => "/nan/",
          },
    );
    
use Data::Dumper;
print Dumper $rmsd;
print Dumper $score;

my $x = [ 1.5,2.3];
my $y = [ 2.1,5.6];

use Google::Chart::Data::TextScaled;
my $chart = Google::Chart->new(
    type => 'ScatterPlot',
    data => 
        Google::Chart::Data::TextScaled->new(
    	dataset => [$rmsd, $score],
    	scaling => [[-10,80],[-20,100]],
    	),
    );
    
print $chart->as_uri, "\n";
#$chart->render_to_file(filename=>"chart.png");

