#!/usr/bin/perl -w

use strict;

# Author: David Wright (dave at dexy dot org)
# Description: Display region of DVD from VIDEO_TS.IFO file

my @codes = (
'U.S., Canada, U.S. Territories',
'Europe, Japan, South Africa, and Middle East',
'Southeast Asia and East Asia (including Hong Kong)',
'Australia, New Zealand, Pacific Islands, Central America, Mexico, South America, and the Caribbean',
'Eastern Europe, Indian subcontinent, Africa, North Korea, and Mongolia',
'China',
'Reserved',
'Crazy international venues (airplanes, cruise ships, etc.)'
);


my $VIDEO_TS_IFO = $ARGV[0];
my ($buf, @valid, $int, $mask);

open(VIDEO_TS, $VIDEO_TS_IFO) or die "Can't open $VIDEO_TS_IFO\n";

seek VIDEO_TS, 35, 0;
read VIDEO_TS, $buf, 1;
close VIDEO_TS;

$mask = unpack("B*", $buf);
$int = unpack("C*", $buf);

if( $int ) {
    for(1..8) {
        if( ~$int & 1 ) {
            push @valid, $_;
        }
        $int = $int >> 1;
    }
}

print "VIDEO_TS_IFO has region mask: $mask\n";
if ( @valid ) {
    print "VIEWABLE IN REGIONS: ";
    while( my $reg = pop @valid ) {
        print "$reg ($codes[$reg-1])\n";
    }
}
else {
    print "VIEWABLE EVERYWHERE. WELCOME TO THE FREE WORLD!\n";
}
