#!/usr/bin/env perl

use CGI;
use CGI::Ajax;

my $cgi = new CGI;

################################################################################
# Print  form

my $pjx = new CGI::Ajax( 'exported_func' => \&perl_func );

print 
    $cgi->header(-type=>"text/html"), 
#     $cgi->start_html(-title=>"Web Server Info."),
    ;

print $pjx->build_html( $cgi, \&Show_HTML);

print $cgi->end_html();


################################################################################

sub perl_func {
  my $input = shift;
  # do something with $input
  my $output = $input . " was the input!";
  return( $output );
}

sub Show_HTML {
  my $html = <<EOHTML;
  <HTML>
  <BODY>
    Enter something:
      <input type="text" name="val1" id="val1"
       onkeyup="exported_func( ['val1'], ['resultdiv'] );">
    <br>
    <div id="resultdiv"></div>
  </BODY>
  </HTML>
EOHTML
  return $html;
}
