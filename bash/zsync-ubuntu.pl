#!/usr/bin/env perl

# Official solution : 
# https://launchpad.net/ubuntu-qa-tools

use Modern::Perl;
use Getopt::Long::Descriptive;

my ($opt, $usage) = describe_options(
    "$0 %o ubuntu/daily ubuntu/daily-live ...",
    ['release|r=s','release codename, e.g. oneiric',{default=>'oneiric'}],
    ['days|d=s','E.g. "20110527". Default: "current"',{default=>'current'}],
    ['archs|a','Architectures, e.g. i386 amd64+mac',{default=>'i386,amd64,amd64+mac'}],
    [],
    [ 'help|h', 'List options' ],
);

print($usage->text), exit if $opt->help;

my %distros = (
    'daily'              => { label=>'ubuntu',  filebase=>'alternate'   },
    'daily-live'         => { label=>'ubuntu',  filebase=>'desktop' },
    'kubuntu/daily'      => { label=>'kubuntu', filebase=>'alternate'   },
    'kubuntu/daily-live' => { label=>'kubuntu', filebase=>'desktop' },
    'ubuntu-server'      => { label=>'ubuntu',  filebase=>'server'    },
    'dvd',               => { label=>'ubuntu',  filebase=>'dvd'       },
);

my $host='http://cdimage.ubuntu.com';
my $release = $opt->release;

@ARGV = keys(%distros) unless @ARGV;

foreach my $distro (@ARGV) {
    my $label = $distros{$distro}{'label'};
    my $filebase = $distros{$distro}{'filebase'};
    foreach my $day (split /,/, $opt->days) {
        foreach my $arch (split /,/, $opt->archs) {
            my $url = 
                "$host/$distro/$day/${release}-${filebase}-${arch}.iso.zsync";
            my $dest = "${label}-${filebase}-${day}-${arch}.iso";
            my $cmd = "zsync $url -o $dest";
            system($cmd) == 0 or warn $!
        }
    }
}





