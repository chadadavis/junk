#!/usr/bin/env perl

=head1 NAME

B<vocab> - A basic vocabulary testing program

=head1 SYNOPSIS

 vocab ./yourdictionary.csv [ -l(ang) <de|en|fr|ia> ]

    
=head1 DESCRIPTION

Reads a tab-separated text file dictionary of word/gender/definition triples to
test yourself. Words not in the dictionary can be queried at online web
dictionaries.

Example:

 Abdruck	m	1. das Abdrucken, der Druck: den A. eines Romans
 abermals		noch einmal, wiederum;

For non-nouns you can use the second column to insert the part of speech, or any
other info. e.g.:

 abermals	adv	noch einmal, wiederum;

In any case, every line has two tab-separated fields. When editing a definition
the definition itself may wrap onto subsequent lines. They are automatically
collapsed into a single line before saving.

    
=head1 OPTIONS


=head2 -h|elp 

This help

=head2 -b|rowser Web browser for online dictionary lookups

Default: Your shell's $BROWSER or the standard "x-www-browser --new-window"


=head2 -d|ict Provide your own web dictionary URL

The query word will be appended to the URL, e.g.: with 'perplex'

 http://www.google.com/search?rls=ig&hl=en&q=define%3A

becomes:

 http://www.google.com/search?rls=ig&hl=en&q=define%3Aperplex


=head2 -e|ditor Select a text editor program on your computer

Used to allow you to edit your individual definitions.

E.g. for emacs, you might like:    

 -e "emacs22-nox -Q +1:80"

 (-Q is quick, +1:80, means line 1 and column 80, i.e. end of line)

Default: Your shell's EDITOR variable, otherwise 'vi'


=head2 -f|ile Provide a custom path to your local dictionary database

Default is $HOME/$lang.csv


=head2 -l|ang Select a predefined language website (required)

 en English
   http://www.answers.com/
 de German
   http://www.dwds.de/?woerterbuch=1&qu=
 fr French
   http://dictionnaire.tv5.org/dictionnaires.asp?mot=
   http://www.dicocitations.com/definition_littre.php?motcle=
 ia Interlingua
   http://www.interlingua.com/ied/cerca?op=Cerca&edit[keys]=


=head2 -v|erbose

Print additional detailed inner workings.

This setting is also saved between sessions. Disable it with:

 vocab -no-v

=head1 HISTORY

0.51: Minor bug fixes

0.5: More flexible (regular expression) search. Returns exact matches first.

0.4: Saving settings between sessions, configurable editor/browser

0.3: Command line options, default dictionaries

0.2: Configurable web dictionary URL

0.1: ANSI screen control and keyboard commands

=head1 TODO

It is not possible to search partial matches.

NB also that capitalisation counts.

There is no facility (yet) for automatically extracting the web defitions into
your private dictionary.

The 'db' object should be a separate class

Could be using L<DBI::CSV> or L<Tie::CSV_File>


=cut


################################################################################

use strict;
use warnings;

use File::Temp qw/tempfile/;
use Term::ReadKey;
use Getopt::Long;
use Pod::Usage;
use Data::Dump qw/dump/;
use List::MoreUtils qw/uniq/;

# Web dictionaries
our %langs = (
    'en' => 'http://www.answers.com/',
#    'de' => 'http://www.dwds.de/?woerterbuch=1&qu=',
    'de' => 'http://beta.dwds.de/?qu=',
    'fr' => 'http://dictionnaire.tv5.org/dictionnaires.asp?mot=',
    'ia' => 'http://www.interlingua.com/ied/cerca?op=Cerca&edit%5Bkeys%5D=',
    );

# Load any saved ops
my $opfile = "$ENV{HOME}/.vocab.pl";
our %ops = _load_ops($opfile);

# Overwrite with any command line ops
my $result = GetOptions(\%ops,
                        'h|help',
                        'b|browser=s',
                        'd|dict=s',
                        'e|editor=s',
                        'f|file=s',
                        'l|language=s',
                        'v|verbose!',
    );   

# Save explicit command line options back to file
_save_ops($opfile, %ops);
warn "Options: ", Data::Dump::dump(%ops), "\n" if $ops{'v'};

# Fill in any default ops
my ($GBROWSER) = split ' ', `gconftool-2 --get '/desktop/gnome/url-handlers/http/command'`;
$ops{'b'} ||= $ENV{BROWSER} || $GBROWSER || "x-www-browser --new-window";
$ops{'d'} ||= $langs{ $ops{'l'} };
$ops{'e'} ||= $ENV{EDITOR} || 'vi';
$ops{'f'} ||= "$ENV{HOME}/$ops{'l'}.csv";

warn "Browser: $ops{'b'}\n" if $ops{'v'};
warn "URL: $ops{'d'}\n" if $ops{'v'};
warn "Editor: $ops{'e'}\n" if $ops{'v'};
warn "Database: $ops{'f'}\n" if $ops{'v'};

# Check that all ops satisfied
if ($ops{'h'}) {
    pod2usage(-exitval=>0, -verbose=>2);
} elsif (! $ops{'l'}) {
    pod2usage(-msg=>"-l <language> required", -exitval=>1, -verbose=>1);
}

warn "Do not know a URL for language: $ops{'l'}\n" if $ops{'v'} && ! $ops{'d'};

my $db = init($ops{'f'});
run($db, $ops{'f'});

exit;


################################################################################

sub _load_ops {
    my ($opfile) = @_;
    local $/ = undef;
    open my $fh, $opfile or return;
    my $content = <$fh>;
    my %ops = eval $content;
    return %ops;
}

sub _save_ops {
    my ($opfile, %ops) = @_;
    # Don't save help option as default
    delete $ops{'h'};
    
    if (open my $fh, ">$opfile") {
        print $fh Data::Dump::dump(%ops), "\n";
    } else {
        warn "Cannot save settings to: $opfile\n";
    }
}

# Command processors, waits for one-letter keyboard command 
sub run {
    my ($db, $file) = @_;

    # Reset terminal before being interrupted
    $SIG{'INT'} = \&quit;

    my $prompt = "\n[S]earch, [L]ookup, [E]edit, [D]elete, e[X]it, <RET> random : ";

    # Current entry
    my $entry;
    # Function
    my $cmd;

    # Event loop
    while (1) {
        ReadMode 3; 
        print green(), $prompt, uncolor();
        # -1 busy wait, 0 uses getc(), >0 is a timeout. See Term::ReadKey
#         while (not defined ($cmd = ReadKey(-1))) {}
        while (not defined ($cmd = ReadKey(0))) {}
        print blank();
        $entry = process($cmd, $db, $entry, $file);
    }
}


# Determine which command to dispatch
sub process {
    my ($cmd, $db, $key, $file) = @_;

    if ($cmd eq "\n" || $cmd eq " ") {
        $key = random($db);
        $key = show($db, $key, 1);
    } elsif ($cmd eq "x") {
        save($db, $file);
        quit();
    } elsif ($cmd eq "d") {
        # delete
        if ($key) {
            print blank(), "Deleting $key\n";
            delete $db->{$key};
        }
        $key = undef;
   } elsif ($cmd eq "l") {
       # Lookup
#       $key = lookup($key);
       $key = lookup();
    } elsif ($cmd eq "s") {
        # Search
        $key = search($db);
    } elsif ($cmd eq "e") {
        #edit 
        $key = editor($db, $key);
        save($db, $file) if $key;
    } else {
        warn "Unrecognized command\n";
    }
    
    # if current DB entry was changed, return new value
    return $key;
}

# Open database
sub init {
    my ($file) = @_;
    my %db; 
    our %ops;
    if (! -r $file) {
        if (-e $file) {
            die "Dictionary not readable: $file\n";    
        } else {
            open my $fh, ">>$file" or 
                die "Cannot create dictionary: $file\n";
            warn "Creating new dictionary at: $file\n" if $ops{'v'};
            return \%db;
        }
    }

    open my $fh, "<$file";
    while (<$fh>) {
        my ($key, $attr, $def) = split /\t/;
        chomp $def;
        # Check duplicate
        if (defined $db{$key}) {
            warn blank(), "Appending duplicate: $key\n" if $ops{'v'};
            $db{$key}{'def'} .= ". $def";
        } else {
            $db{$key}{'def'} = $def;
        }
        $db{$key}{'attr'} ||= $attr;
    }
    warn "Loaded dictionary with ", scalar(keys %db), " entries\n" if $ops{'v'};
    return \%db;
}

sub save {
    my ($db, $file) = @_;
    our %ops;
    open my $fh, ">$file";
    for (sort { lc($a) cmp lc($b) } keys %$db) {
        print $fh join("\t", $_, $db->{$_}{'attr'}, $db->{$_}{'def'}), "\n";
    }
    close $fh;
    warn blank(), "Saved ", scalar(keys(%$db)), " entries\n" if $ops{'v'};
}


sub show { 
    my ($db, $key, $pause) = @_;
    return $key unless defined($key) && exists($db->{$key});

    # Count words reviewed in this review session
    our $counter;
    $counter++;

    print yellow();
    print blank();
    printf "%3d %s ", $counter, $key;
    print(", ", $db->{$key}{'attr'}) if $db->{$key}{'attr'};
    print "\n";

    if ($pause) {
        print green(), "[ENTER show]: ";
        ReadMode 3;
        # ReadKey(-1) does busy wait (100% CPU), ReadKey(0) uses getc()
        while (not defined (ReadKey(0))) { }
    }
    print blank(), uncolor(), "\t", $db->{$key}{'def'}, "\n";
    return $key;
}

# Print a random key
sub random {
    my ($db) = @_;
    my @keys = keys(%$db);
    unless (@keys) {
        warn blank(), 
        "Dictionary empty, use 's' to search, then 'e' to edit\n" if $ops{'v'};
        return;
    }
    my $i = int(rand(@keys));
    return $keys[$i];

}

# Launch browser to lookup phrase in on-line dictionary
sub lookup {
    my ($key) = @_;
        
    our $ops;
    unless ($ops{'d'}) {
        warn blank(), "Lookup URL not defined\n";
        return;
    }

    ReadMode 0;
    unless ($key) {
        print "Lookup what: ";
        $key = <STDIN>;
        chomp $key;
    }
    # Don't worry about background processes
    local $SIG{'CHLD'} = 'IGNORE';

    print "Looking up: $key\n";
    system("$ops{'b'} \"$ops{'d'}${key}\" 2>/dev/null");
    return $key;
}

# Search for specified entry
# Print all found entries, or print no match, option to add entry
sub search {
    my ($db) = @_;
    our $prevkey;
    our @prevhits;
    our $previdx;
    ReadMode 0;
    print blank(), 
    "Search for term", 
    ($prevkey ? " [$prevkey ".($previdx+1)."/".scalar(@prevhits)."]":''), ": ";
    my $key = <STDIN>;
    chomp $key;
    if ($key) {
        $previdx = 0;
        @prevhits = ();
    }
    $key ||= $prevkey;
    return unless $key;
    $prevkey = $key;

    unless (@prevhits) {
        # Exact matches
        push @prevhits, grep { /^$key$/ } keys %$db;
        # Case insensitive
        push @prevhits, grep { /^$key$/i } keys %$db;
        # Unanchored Regex
        push @prevhits, grep { /$key/i } keys %$db;
        @prevhits = List::MoreUtils::uniq @prevhits;
    }
    unless (@prevhits) {
        warn "\"$key\" not found\n";
        $prevkey = undef;
        # Remove funky chars
        $key =~ s/[\^\$]//g;
        return lookup($key);
    }
    print "Result ", $previdx+1, " of ", scalar(@prevhits), "\n";
    my $found = $prevhits[$previdx];
    $previdx++;
    unless ($previdx < @prevhits) {
        $previdx = 0;
        @prevhits = ();
        $prevkey = undef;
    }
    return show($db, $found);
}


# Allows editing of an entry, whether it exists yet or not
sub editor {
    my ($db, $key) = @_;
    our $ops;

    unless ($key) {
        warn "\nTry [s]earching for an entry first\n";
        return search($db);
    }
    
    # Write query/existing line to a temp file
    my ($fh, $name) = tempfile();

    if ($db->{$key}) {
        print $fh join("\t", $key, $db->{$key}{'attr'}, $db->{$key}{'def'});
    } else {
        print $fh "$key\t\t";
    }
    close $fh;

    # Launch editor on temp file
    system("$ops{'e'} $name");
    
    # Re-open file
    open $fh, "<$name";
    # Get first line
    my $line = <$fh>;
    chomp $line;
    # Parse in to 3 tab-separated blocks
    my ($k, $attr, $def) = split/\t/, $line;
    unless (defined($def)) {
        warn 
            "Need 3 tab-separated fields: phrase, case, definition\n",
            "Try editing again\n"; 
        return $key; 
    }
    # Suck up any remaining lines, these belong to definition
    my @lines = <$fh>;
    chomp for @lines;
    close $fh;

    # Remove blank lines
    @lines = grep { $_ } @lines;
    # Append any other lines to definition
    $def = join(" ", ($def, @lines));
    chomp $def;

    # Save entry
    $db->{$k}{'attr'} = $attr;
    $db->{$k}{'def'} = $def;

    show($db, $key) if $ops{'v'};

#    return $key;
    return show($db, $key);

}

sub quit {
    ReadMode 0;
    print "\n";
    exit;
}

# Screen coloring/uncoloring
sub yellow {
    return "\e[0;33m";
}

sub green {
    return "\e[0;32m";
}

sub uncolor {
    return "\e[0;0m";
}

sub blank {
    return "\033[1K\r";
}
