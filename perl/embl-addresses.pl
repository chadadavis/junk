#!/usr/bin/env perl

# Script tries to collect as many email addresses from EMBL-D website as possible
# Outputs them in Mozilla CSV format
# Note: Evolution will not accept the non-ascii chars in this encoding
#   I.e. need to change the output manually
# EBI addresses are not properly parsed, due to different format of website
# Only people with pictures are found
# Is there no LDAP server at EMBL ???

use warnings;
use strict;
use LWP::Simple;

my $base = "http://www-db.embl.de/jss/EmblGroupsHD";
my $idxurl = "${base}/per_0000.html";
my $idx = get($idxurl);

$|=1;

while ($idx =~ m|<a href=(.*?)><img .*? title="(.*?)"></a>|g) {

    my $name = $2;
    my $url = "${base}/$1";
    my $page = get($url);

#     my ($title,$group,$room,$tel,$email) = 
#         $page =~ m|<b>(.*?)</b><br> <a href=/jss/EmblGroupsHD/g_\d+.html>(.*?)</a<br>.*?<b>Room:</b> (.*?)<br>.*?<b>Tel:</b> (.*?)<br>.*?<b>E-mail:</b>.*?>(.*?)</a>|s;

    my ($tel) = $page =~ m|<b>Tel:</b> (.*?)<br>|s;
    my ($room) = $page =~ m|<b>Room:</b> (.*?)<br>|s;
    my ($email) = $page =~ m|<b>E-mail:</b> <a.*?>(.*?)</a>|s;

#     my ($room,$tel,$email) = 
#         $page =~ m|<b>Room:</b> (.*?)<br>.*?<b>Tel:</b> (.*?)<br>.*?<b>E-mail:</b>.*?>(.*?)</a>|s;

#     print join(',', $name,$title,$group,$room,$tel,$email,$url), "\n";
#     print join(',', $name,$title,$group,$room,$tel,$email), "\n";
#     print join(',', $name,$email,$room,$tel), "\n";
    print "$name,,,,$email\n";


}
