#!/usr/bin/env perl
use strict;
my $debug = shift;

my $res;

# Get IP(s)
$res = `LANG=C /sbin/ifconfig`;
my ($ip) = $res =~ /inet addr:(88(.[0-9]{1,3}){3})/;
my @ips = $res =~ /inet addr:([0-9.]+)/g;
print "ip $ip\n" if $debug;
print "all ips: ", join(" ", @ips), "\n" if $debug;

# Get hostname
my $host;
if ($ip) {
    $res = `host $ip`;
    ($host) = $res =~ /(dslb-.*?.net)./;
}
print "host $host\n" if $debug;

# Save to local file

my $lfile = "$ENV{HOME}/.ip";
open my $fh, ">$lfile" or die;
print $fh "$host\n", join("\n", @ips), "\n";

# FTP file
#my $url = "ftp://ftp-exchange.embl.de/pub/exchange/davis/incoming";
#my $date = `date +"%F-%T"`;
#chomp $date;
#my $rfile = "ip-${date}.txt";
#open my $ftp, "| ncftp $url";
#print $ftp "put -z $lfile $rfile";
#close $ftp;

# SCP file
#print "scp $lfile praktikum.bio.wzw.tum.de ..." if $debug;
#`scp $lfile 141.40.43.211:`;
#print "\n" if $debug;

# Try to update ~/.hosts on pc-russell...
print "update pc-russell ..." if $debug;
# system('ssh 10.1.103.186 \'scp praktikum.bio.wzw.tum.de:.ip .; export ip=`cat .ip`; perl -pi -e "s/dslb-\S+arcor-ip.net/$ip/" .hosts\'');
#`scp $lfile 10.1.103.186:`;
`scp $lfile 10.1.103.185:`;
print "\n" if $debug;




