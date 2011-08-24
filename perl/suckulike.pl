#!/usr/bin/env perl

use strict;
use warnings;

use LWP::Simple;
use LWP::UserAgent;
use HTTP::Cookies::Netscape;
use File::Basename;

################################################################################
# User options: 

# User name
my $user = "chad_davis";
# Location of a Netscape-formatted cookies file (e.g. from firefox/mozilla)
my $cookies = $ENV{HOME} . "/.mozilla/firefox/profile/cookies.txt";
# If a tag is set, only those articles get downloaded
my $tag = '';
# Sleep between each HTTP retrieval 
my $sleep = 5;

################################################################################
# Vars:

# Site:
 my $site = "http://www.citeulike.org";
#my $site = "http://de.citeulike.org";
# Links to the articles pages
my $baseurl = $site . "/user/${user}" . ($tag ? "/tag/${tag}" : "") . "?page=";
my $biburl = $site . "/bibtex/user/${user}";
my $md5url = $biburl . "?md5=true";
# Start page
my $page = 1;

# Local:
my $basedir = $ENV{HOME} . "/CiteULike";
system("mkdir -p $basedir") and die("Cannot create ${basedir}\n");
my $bibfile = $basedir . "/${user}.bib";
my $md5file = $bibfile . ".md5";

# Browser:
my $ua = LWP::UserAgent->new;
$ua->cookie_jar(HTTP::Cookies::Netscape->new(file => $cookies));

# Temp. vars:
my $url;
my $content;
my $response;

################################################################################

# Quit if the md5sum of the library hasn't changed
my $prev_md5 = `cat $md5file`;
chomp $prev_md5;
print STDERR "Fetching MD5 sum ... ";
my $curr_md5 = get($md5url);
print STDERR "\n";

if ($prev_md5 eq $curr_md5) {
    print STDERR "Library currently up to date (Bibtex MD5 sum unchanged)\n";
    exit unless @ARGV;
} else {
    print STDERR "Library has changed. Fetching updates ... \n";
    print $prev_md5, ":", $curr_md5, "\n";
}

# Go through all pages
do {
    print STDERR "Page $page:\n";
    sleep $sleep;
    $url = $baseurl . $page;

    $response = $ua->get($url);
    if ($response->is_success) {
        $content = $response->content;
    } else { 
        die $response->status_line; 
    }

    # For each PDF href
    while ($content =~ /href=\"(.*?pdf)\"/ig) {
        my $docurl = $site . $1;
        # Get the base file name of the URL (after last slash )
        my ($file) = $docurl =~ /.*\/(.*?)$/;
        # Names contain dashes, which are encoded. Replace them.
        $file =~ s/\%3d/-/g;
        $file =~ s/\%2d/-/g;
        # Use 4-digit years in local files (for JabRef)
        if ($file =~ /^(.*?)_(\d+)_(.*)$/) {
            $file = "$1_" . ($2 < 20 ? "20" : "19") . "$2_$3";
        }
        printf STDERR "\t%40s ... ", $file;
        $file = $basedir . '/' . $file;
        sleep $sleep;
        # Only download new files (need to also use UA, with cookies, here)
        $response = $ua->mirror($docurl, $file);
        print STDERR $response->message(), "\n";
        if ($response->message() eq "Not Modified") {
            # Quit unless some command line option says to check every article
            unless (@ARGV) {
                print STDERR "No more new articles\n";
                exit(bibtex());
            } 
        }
    
    } # while

    $page++;

# While there's a 'next' link on the page (i.e. while there's still pages)
} while ($content =~ /href=\".*?\">Next<\/a>/i);

exit(bibtex());

################################################################################

# Find a unique file name if the desired destination already exists
# Simply appends -a -b -c and so an before the *.pdf extension
sub inc {
    my ($path) = @_;
    my $dir = dirname($path);
    my $base = basename($path, '.pdf');
    my $ext = 'a';
    $path = "${dir}/${base}.pdf";
    while ( -r $path) {
        print STDERR "Exists:$path:\n";
        $path = "${dir}/${base}-${ext}.pdf";
        $ext++;
    }
    print STDERR "got:$path:\n";
    return $path
    
}

sub bibtex {
    # Now that library has been mirrored:
    # Update bibtex and md5sum (no need for UserAgent or cookies here)
    print STDERR "Fetching bibtex ... ";
    $response = mirror($biburl, $bibfile);
    print STDERR ($response eq "200")?"OK":"Error:$response","\n";
    print STDERR "Fetching bibtex md5sum ... ";
    $response = mirror($md5url, $md5file);
    print STDERR ($response eq "200")?"OK":"Error:$response","\n";

    # TODO need to do processing on Bibtex file here 
    # Use perl libs for OO interface
    # Strip bad characters (like #) from DOI and other bad chars from fields

}

