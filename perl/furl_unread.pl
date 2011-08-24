#!/usr/bin/env perl

# Mails an unread article from my Furl archive (of a given category) to an email

use strict;
use warnings;

use LWP::Simple;

my $furl_user = "chad_davis";
my $category = "wiki";
my $email = 'davis@embl.de';

my $homepage = "http://www.furl.net/member/${furl_user}?topic=${category}&read=0";
#print STDERR "Homepage:$homepage\n";

my $page = get($homepage);
$page or die("No page at:$homepage\n");

# print STDERR "Page:\n$page\n";

# Previous (failed) matching attempts
# my $re = '<strong>(.*?)</strong>.*?<div id="pubItemText">.*?<a href="/item/[0-9]+/forward">(.*?)</a></div>';
# my $re = '<div id="pubItemText">.*?<a href="/item/[0-9]+/forward">(.*?)</a></div>';

# Match entries
my $re = '<a href="/item/[0-9]+/forward" title="Go to: (.*?)">(.*?)</a>';

while ($page =~ /$re/gs) {

    my $URL = $1;
    # This really only makes sense for Wikipedia articles here
    my ($name) = $URL =~ /.*\/(.*)$/;

#    print STDERR "Name: $name\n";
#    print STDERR "\tURL: $URL\n";

    open my $fh, "|mail -s \"Furled Wiki: $name\" $email" or 
        die("Failed to mail $email\n");
    print $fh "$URL\n";
    close $fh;

#    system("echo $URL | mail -s \"Furled Wiki: $name\" $email") == 0 or
#        die("Failed to mail $email\n");
    
    # Just do the first one, until I read it
    # Don't worry about going through all pages
    exit;

}


__END__

Here is what the page source looks like

<strong>Categorical imperative - Wikipedia, the free encyclopedia</strong></a><br />

  Rated 3 in 


in


<a href="/member/chad_davis/items/search?topic=wiki" class="topicLink">wiki</a>

 on Sat Aug 11 09:14:28 UTC 2007.<br />

  

  

  <div id="pubItemText">
    <img src="/sites/furl/i/globe.gif" width="17" height="15" alt="Link" style="border:none" /> <a href="/item/23952976/forward">http://en.wikipedia.org/wiki/Categorical_imperative</a>
  </div>
