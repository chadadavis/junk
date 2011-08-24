

# Options to implement
# Option to rescale
# Option to convert to PNG
# Optional output file name
# Option to sort by intensity (row/column)

# Scan table for min/max values

sub print_map() {

	my ($table, $peak, $maxpeak, $minpeak) = @_;

	# Dimensionen
	my ($n, $m) = (scalar(@$peak), scalar(@{$peak->[0]}));

	print 
		"<h2>Topological Confidence Map</h2><hr /><br>\r\n",

		"<a title=\"Click for Help on Confidence Maps\" ",
		"href=\"javascript:openHelp(\'$rootdir/$cgidir/help?app=",
		"$0#confidence_map\')\">", 
		"<img align=\"middle\" border=0 ",
		"src=\"$rootdir/$iconsdir/mshelp.gif\" /></a>",
		"&nbsp;",		
		"<a title=\"Click for Help on Confidence Maps\" ",
		"href=\"javascript:openHelp(\'$rootdir/$cgidir/help?app=",
		"$0#confidence_map\')\">", 
		"Help on interpreting this confidence map</a><br /><br />",
		;

	  printf
		"Highest Peak: <b>%10.2f</b><br />\r\n" . 
		"Lowest Valley: <b>%10.2f</b><br />\r\n", $maxpeak, $minpeak;

	# Gewaehre, das Verzeichnisse existieren und schreibbar sind
	if (! -d "$uploaddir")      { mkdir "$uploaddir"; }
	if (! -d "$uploaddir/topo") { mkdir "$uploaddir/topo"; }

	open(EPSFILE, ">$uploaddir/topo/$$.eps" ) or 
		fail "Could not open file $$.eps for writing";
	
	print EPSFILE <<EOF;
%!PS-Adobe-2.0 EPSF-2.0
%%BoundingBox: 0 0 $m $n
%%Creator: Mojca Miklavec
%%Title: Contact Maps
%%EndComments

% takes hue and coordinates between (0,0) and (nx-1,ny-1) as input: h x y
/sq {
	gsave
	% translate by x, y given
	translate
	% turn on greyscale
	setgray 
	% rgb, blue scale
	% dup 1
    % setrgbcolor
	% hue
%	0.7 mul
%	1 1
%	sethsbcolor
	newpath
	0 0 moveto
	0 1 lineto
	1 1 lineto
	1 0 lineto
	closepath
	fill
	grestore
} def

/path {
	gsave
	% translate by x, y given
	translate
	% hue
	0.7 mul
	1 1
	sethsbcolor
	newpath
	0 0 moveto
	0 1 lineto
	1 1 lineto
	1 0 lineto
	closepath
	fill
	grestore
} def

EOF

	# Drucke Intensitaet der Peak-Werte als Graustufenfarbe
	for my $i ( 0 .. $n - 1 ) {
		for my $j ( 0 .. $m - 1 ) {
			printf EPSFILE "%.3f %d %d sq\n", 
			($peak->[$i][$j] - $minpeak) / ($maxpeak - $minpeak), $m - $j - 1, $i;
			if ($table->[$i][$j]{reach}) {
				printf EPSFILE "%.3f %d %d path\n", 0, $m - $j - 1, $i;
			}
		}
	}


	print EPSFILE "\nshowpage\n\%\%EOF\n";

	close EPSFILE;

	# Stelle PNG-Graphik her
	# Fange Ghostscript-Ausgabe als HTML-Kommentar ab
	print "<br /><!-- \r\n";
	system("gs -dSAFER -dBATCH -dNOPAUSE -sDEVICE=png16m -r72 " .
		   "-dDEVICEWIDTHPOINTS=$m -dDEVICEHEIGHTPOINTS=$n " .
		   "-sOutputFile=$uploaddir/topo/$$.png $uploaddir/topo/$$.eps");

	# Entferne EPS-Datei
	unlink "$uploaddir/topo/$$.eps";

	print 
		" --><br />",
		"Preview:<br /><br />",

		"<a title=\"Open in a new window\" ",
		"href=\"javascript:openxWindow(\'$uploaddir/topo/$$.png\', ", $m + 30, 
		", ", $n + 30, ")\">",
		"<img border=0 width=80 ",
		"src=\"$uploaddir/topo/$$.png\" />", 
		"</a><br /><br />\r\n",

		"<a title=\"Open in a new window\" ",
		"href=\"javascript:openxWindow(\'$uploaddir/topo/$$.png\', ", $m + 30, 
		", ", $n + 30, ")\">",
		"<img align=\"middle\" border=0 ",
		"src=\"$rootdir/$iconsdir/greenarrow.gif\" />", 
		"</a>&nbsp;",

		"<a title=\"Open in a new window\" ",
		"href=\"javascript:openxWindow(\'$uploaddir/topo/$$.png\', ", $m + 30, 
		", ", $n + 30, ")\">",
		"Display the map", 
		"</a> in full size in a new window.",
		"\r\n",
		;

} # print_map

