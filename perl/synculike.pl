#!/usr/bin/env perl

=head1 NAME

B<synculike.pl> - Syncronize local bibtex/PDF files with CiteULike

=head1 SYNOPSIS

 # Minimum requirement: CUL username
 synculike -user chad_davis

 # Provide all options by config file ~/.CiteULike.pl (in Perl syntax).
 synculike

=head1 DESCRIPTION

TODO

Write this as a module and submit to CPAN, post to list
- Warn about changing the sleep parameter and advise to use mirrors
Provide a test mode that just runs the first five entries/files 
(to save the server bandwidth)

Note: to be able to download PDFs, you need to be logged into CiteULike. This means that you should have a login cookie from the web server. If you have logged in with your web browser recently, that should be sufficient.

Configuration file (file is in Perl syntax). Located in $HOME/.CiteULike.pl

For example (these are the defaults):

 $user = "chad_davis";
 $repository = "$ENV{HOME}/CiteULike";
 $bibtex = "$ENV{'HOME'}/.${user}.bib";
 $mirror = "www.citeulike.org";
 $cookies = "$ENV{'HOME'}/.mozilla/firefox/profile/cookies.txt";

=head1 PARAMETERS

=head2 -h|help 

Print this help page

=head2 -u|user 

Set CiteULike user name. 

No default. Must be provided in config file or on command line.

=head2 -r|repository 

Local path to directory where PDFs are stored.

Default: $HOME/CiteULike/

=head2 -b|bibtex 

Local path to bibtex file to be syncronized

Default: $HOME/.<user>.bib

=head2 -m|mirror 

CiteULike mirror server to use.

Default: www.citeulike.org

=head2 -k|cookies 

Path to Netscape-formatted cookies file.

Default: $HOME/.mozilla/firefox/profile/cookies.txt

=head1 TODO

Should provide options to sync or not sync bibtex and repository

Should provide option to only regenerate keys on existing bibtex

Support arbitary patterns for setting bibtex (like JabRef)

=head1 AUTHOR(S)

 Chad Davis
 European Molecular Biology Laboratory

=head1 RELEASE

April 2007

=cut

################################################################################

use strict;
use warnings;

# CPAN
use LWP::Simple;
use LWP::UserAgent;
use HTTP::Cookies::Netscape;
use File::Basename;
use Text::BibTeX;
use Pod::Usage;
use Getopt::Long;
use File::Temp qw(tempfile);

use SyncULike;

# Custom
# TODO this script should be stand-alone
# use Utils;

################################################################################

$::DEBUG = 0;

# Parse command line and config file options
our ($user, $repository, $bibtex, $mirror, $cookies) = options();

# TODO Mutually exclusive options: 
# -d download (get server bibtex and overwrite local)
# -s syncronize (send changes back to server, otherwise just overwrite local)
# TODO check that both ops aren't specified

# -r regenerate bibtex keys (on local file )
# my $overwrite = 1;
my $overwrite = 1;
my $regenerate = 1;
my $synchronize = 0;

if ($overwrite) {
    # Downloaded bibtex overwrites local file
    get_bibtex($bibtex);
} elsif ($synchronize) {
    # Download to temp file and then sync them
    # TODO
#     my $server_bibtex = tempfile();
#     get_bibtex($server_bibtex);
#     syncbibtex($bibtex,$server_bibtex);
}

# Do additional cleaning of bibtex file: e.g.
# Remove # from DOIs and check that article titles are wrapped in {}

if ($regenerate) {
    create_keys($bibtex);
}

# TODO
# Check if repository should be downloaded or sychronized, mutually exclusive

# Syncronize PDFs (based on time-stamps)
if (0) {
    syncrepository();
}

exit;

################################################################################

# User options: 
sub options {
    # These global vars are changed by the settings in the config file
    our ($help, $user, $repository, $bibtex, $mirror, $cookies);

    # Read config. File should be a perl script
    my $config = "$ENV{'HOME'}/.CiteULike.pl";
    if (-r $config) {
        require $config;
    }

    # Command line options, over-ride config file
    my $result = GetOptions(
                            'd', \$::DEBUG,
                            'h|help', \$help,
                            'u|user=s', \$user,
                            'r|repository=s', \$repository,
                            'b|bibtex=s', \$bibtex,
                            'm|mirror=s', \$mirror,
                            'k|cookies=s', \$cookies,
                            );
    # Error checking
    if (! $result || $help) { pod2usage(-exitval=>1, -verbose=>2); }
#     verbose("user:$user:");
    unless ($user) { pod2usage(-exitval=>2, -verbose=>2); }

    # Defaults
    $repository ||= "$ENV{'HOME'}/CiteULike";
    $bibtex ||= "$ENV{'HOME'}/.${user}.bib";
    $mirror ||= "http://www.citeulike.org";
    $cookies ||= "$ENV{'HOME'}/.mozilla/firefox/profile/cookies.txt";
    print STDERR 
        join(':', $user, $repository, $bibtex, $mirror, $cookies), 
        "\n";
    return ($user, $repository, $bibtex, $mirror, $cookies);
}

################################################################################

# Update bibtex and md5sum from server 
# (no need for UserAgent or cookies here, these are public)
sub get_bibtex {
    my ($dest) = @_;
    our ($user, $mirror);
    my $biburl = $mirror . "/bibtex/user/" . $user;
    # Get MD5 (currently unused)
    my $md5 = "${dest}.md5";
    my $md5url = $biburl . "?md5=true";
    print STDERR "Fetching bibtex: $biburl ... ";
    my $response;
    $response = mirror($biburl, $dest);
    print STDERR "\n", ($response eq "200") ? 
        "OK => $dest" : "Error:$response", "\n";
    print STDERR "Fetching bibtex md5sum: $md5url ... ";
    $response = mirror($md5url, $md5);
    print STDERR "\n", ($response eq "200") ?
        "OK => $md5" : "Error :$response", "\n";
}

__END__

sub syncrepository {
    my ($bibtex, $repository) = @_;
    system("mkdir -p $repository") and die("Cannot create ${repository}\n");

#     # Browser:
#     my $ua = LWP::UserAgent->new;
#     $ua->cookie_jar(HTTP::Cookies::Netscape->new(file => $cookies));
    
#     # Temp. vars:
#     my $url;
#     my $content;
#     my $response;

}



################################################################################


# Go through all fields and remove invalid chars and protect capitals and "
sub _clean {
    my ($entry) = @_;
    
}

sub _post {
    my ($entry) = @_;

    # Get URL, based on user and CUL ID
    # e.g. http://www.citeulike.org/edit_article_details?article_id=1012859
    # <form id="article" method="post" action="/edit_article_details.do">

       # <input type="hidden" name="article_id" value="1012859" />

    # Map bibtex entry types to CUL types
}








################################################################################



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
#     $response = mirror($md5url, $md5file);
    print STDERR ($response eq "200")?"OK":"Error:$response","\n";
}

