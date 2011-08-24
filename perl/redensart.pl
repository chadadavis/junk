#!/usr/bin/env perl

# Mails an unread article from my Furl archive (of a given category) to an email

use strict;
use warnings;

use LWP::Simple;

my $email = shift || 'davis@embl.de';
my $homepage = "http://www.redensarten-index.de/zufall.php";

#print STDERR "Homepage:$homepage\n";

my $page = get($homepage);
$page or die("No page at:$homepage\n");

# print STDERR "Page:\n$page\n";

# Match entries
my $re = "<td id='td22'.*?>(.*?)</td>.*?<td id='td22'.*?>.*?</.*?<tr><td id='tdrartmitte2'>(.*?)</";

while ($page =~ /$re/gs) {

    my $def = $1;
    my $phrase = $2;

    $def =~ s/&nbsp;//;
    $phrase =~ s/&nbsp;//;

#     print "$phrase:\n";
#     print "\t$def\n";

    my @emails = split ' ', $email;
#    print STDERR "emails:@emails\n";
    my ($first, @cc) = @emails;
#    print STDERR "$first, @cc\n";
    my $cmd = "mail -s \"[Redensart] $phrase\" $first";
    $cmd .= " -c @cc" if @cc;
#    print STDERR "$cmd\n";
    open my $fh, "|$cmd" or 
        die("Failed to mail: $cmd\n");
    print $fh "$phrase:\n\t$def\n";
    close $fh;

    # Just do the first one, until I read it
    # Don't worry about going through all pages
    exit;

}


__END__

Here is what the page source looks like

<tr><td id='tdrartoben2'>&nbsp;</td><td id='td22' rowspan='3'>Das kannst du einem Dummen erzählen! Das glaube ich dir nicht!&nbsp;</td><td id='td12' rowspan='3'>&nbsp;</td><td id='td22' rowspan='3'>umgangssprachlich, salopp&nbsp;</tr><tr><td id='tdrartmitte2'>Das kannst du einem erzählen, der sich die Hose mit der Kneifzange / Beißzange zumacht / anzieht!&nbsp;</td></tr>

