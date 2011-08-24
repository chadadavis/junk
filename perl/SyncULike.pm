#!/usr/bin/env perl

package SyncULike;
use base qw(Exporter);
our @EXPORT = qw(
create_keys
set_url
rm_abstract
);

use File::Basename;
use File::Temp qw(tempfile);
use Text::BibTeX;

# Derive CuL bibtex key, e.g. 'jones_2003_analyzing'
# This function needs to be further abstracted to simply process 'some' change on the bibtex file, as they're may be other function that also change certain fields
sub create_keys {
    my ($bibtex) = @_;
    my $dir = dirname($bibtex);
    my (undef, $tempfile) = tempfile(DIR=>$dir);
#     print STDERR "tempfile:$tempfile:\n";
    my $bib = new Text::BibTeX::File("$bibtex");
    my $nbib = new Text::BibTeX::File(">$tempfile");
    # Prevent duplicate keys
    my %unique;
    # Append these to year, to make keys unique, prepend with dummy index 0
    my @alpha = (0, 'a'..'z');

    my $entry;
    while ($entry = new Text::BibTeX::Entry $bib) {
        # Returns an array, we just take the first author
        my ($first_auth) = $entry->names('author');
        unless ($first_auth) {
            print STDERR $entry->key(), ": No authors! Skipping.\n";
            next;
        }
        # CiteULike joins last names with '+'
        my ($von) = $first_auth->part('von');
        my ($last_name) = $first_auth->part('last');
        $last_name = $von . '+' . $last_name if $von;

        # Note: we're using 4-digit years here (CiteULike uses 2-digit years)Ja
        my $year = $entry->get('year');
        # Get first *real* word of title (as defined by CiteULike)
        my $short_title = first_word($entry->get('title'));
        # Default key pattern of CUL
        my $key = lc("${last_name}_${year}_${short_title}");
        if ($unique{$key}) {
            # Append a,b,c etc
            $year .= $alpha[$unique{$key}];
            $key = lc("${last_name}_${year}_${short_title}");
            $unique{$key}++;
        } else {
            $unique{$key} = 1;
        }
#         verbose("key:$key:");
        $entry->set_key($key);
        # This preserves the original order of the bibtex database
        $entry->write($nbib);
        # TODO BUG
        # Check if $key unique (save list of keys first, in hash)
        # Increment year by appending 'a', 'b', etc if key not unique
        # But what determines the order?
        # CUL appends new entries to beginning of file
        # Of course, when there are dups, CUL doesn't change the file name!
        
        # So, even if we fix it here, it's not consistent.
        # Note this when downloading files, they need to be renamed!
        # They files arlready need to be renamed to compensate for the year
    }   

    $bib->close();
    $nbib->close();
    rename($tempfile, $bibtex);
}

################################################################################

sub first_word {
    my ($str) = @_;
    # Clean it first, then tokenize. Seems to be how CUL does it
    $str =~ s/[^\w ]//g;
    my @tokens = split(/ /, $str);
    # These are the stop words that CiteULike *seems* to skip
    my @skip = qw(a the an is); 
    my $first;
    for my $token (@tokens) {
        if (grep { lc($token) eq $_ } @skip) {
            next;
        } else {
            $first = $token;
            last;
        }
    }
    unless ($first) {
        print STDERR "No valid word found in title!\n";
    }
    return $first;
}

sub set_url {
    my ($bibtex, $base_url) = @_;

    $base_url ||= "http://www.citeulike.org/group/2524/article";

    my $dir = dirname($bibtex);
    my (undef, $tempfile) = tempfile(DIR=>$dir);
    my $bib = new Text::BibTeX::File("$bibtex");
    my $nbib = new Text::BibTeX::File(">$tempfile");

    my $entry;
    while ($entry = new Text::BibTeX::Entry $bib) {
        my ($url) = $entry->get('url');
        my ($citeulike_article_id) = $entry->get('citeulike-article-id');
        $entry->set('url', "${base_url}/${citeulike_article_id}");
        # This preserves the original order of the bibtex database
        $entry->write($nbib);
    }

    $bib->close();
    $nbib->close();
    rename($tempfile, $bibtex);
}

sub rm_abstract {

    my ($bibtex, $base_url) = @_;

    $base_url ||= "http://www.citeulike.org/group/2524/article";

    my $dir = dirname($bibtex);
    my (undef, $tempfile) = tempfile(DIR=>$dir);
    my $bib = new Text::BibTeX::File("$bibtex");
    my $nbib = new Text::BibTeX::File(">$tempfile");

    my $entry;
    while ($entry = new Text::BibTeX::Entry $bib) {
        $entry->delete('abstract');
        # This preserves the original order of the bibtex database
        $entry->write($nbib);
    }

    $bib->close();
    $nbib->close();
    rename($tempfile, $bibtex);

}

