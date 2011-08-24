#!/usr/bin/env perl

use Data::Dumper;
use LWP::Simple;

# Disclaimer: this is a cheap hack

# Create new calendar just for EMBL events (called EMBL), private for now
# Invite test users, if it works

# Add other outstations too, invite Fabian

# TODO PROB
# Don't post duplicates
# Does G calendar not have any sort of IDs?
# Could make an ID from date/time, but google doesn't save this
# How to handle updates?
# 
# TODO POD

# Note: this only works for seminars now
# Just parse first half of entry, determine if course or seminar, parse 2nd half

################################################################################

# URL components:
my $base = "http://www-db.embl.de/jss/EmblGroupsOrg/";
my $untilTime = 2;
my $p = 1;
my $eventTypeID = 1;
my $url = $base . "events_0" . '?' . 
    '&' . "untilTime=" . $untilTime .
    '&' . "p=" . $p .
    '&' . "eventTypeID=" . $eventTypeID .
    "\n";
my $content = get($url) or die;

my @events;
# Parse event records (table rows)
while (
       $content =~ m|
       <tr><td\ class=green.*?      # start
       <b>(.*?):</b>                # event type
       .*?
       <b>\"?(.*?)\"?</b>           # title
       .*?
       ([a-z]+),\ (\d+)\ ([a-z]+)\ (\d+)\ (\d+):(\d+)\ (.*?),
                                    # weekday day month year2 hour min room
       .*?
       (<b>)?(.*?)(</b>)?<br>       # outstation
       .*?
       <b>(.*?)</b>                 # presenter
       .*?
       ,\ (.*?)<br>                 # institute
       .*?
       Host:\ (.*?)<br>              # host
       .*?
       href=(\S+)                   # link
       .*?
       </tr>                        # end
        |gsxi) {
    
    my %event;
    $event{'type'}       = $1;
    $event{'title'}      = $2;
    $event{'weekday'}    = $3;
    $event{'day'}        = $4;
    $event{'month'}      = $5;
    $event{'year'}       = $6;
    $event{'hour'}       = $7;
    $event{'minute'}     = $8;
    $event{'room'}       = $9;
    $event{'outstation'} = $11;
    $event{'presenter'}  = $13;
    $event{'institute'}  = $14;
    $event{'host'}       = $15;
    $event{'link'}       = $16;

    # TODO remove leading/training spaces from all values(%event)

    $event{'year'} += 2000;
    $event{'link'} = $base . $event{'link'};

#     print Dumper(%event);

    push @events, \%event;
    print join(' ',
               $event{'weekday'},
               $event{'year'},
               $event{'month'},
               $event{'day'},
               $event{'hour'},
               $event{'minute'},
               $event{'type'},
               $event{'presenter'},
               $event{'title'},
               ), "\n";
               
}


__END__

Get list of events according to these options

untilTime
0 => Future
1 => Today
2 => One Week
3 => One Month

p
0 => All EMBL
1 => EMBL Heidelberg
2 => EMBL-EBI Hinxton
3 => EMBL Hamburg
4 => EMBL Grenoble
5 => EMBL Monterotondo

eventTypeID
0 => All Events
1 => Seminars
2 => Courses and Conferences

Event classes:
External Speaker
Progress Report
Course
Conference
Group Leader Seminar
Seminar by EMBL Speaker 
Science and Society
EMBL Distinguished Visitor Lecture
CISB Seminar

Syntax of an event:

<tr>
<td class=green valign=top width=100><b>
External Speaker:
</b></td>
<td class=dark valign=top>
<b>"Structural biology of ESCT assemblies on endosomal membranes"</b>
<br>
Tuesday, 15 May 07 10:30 CIBB Seminar Room, <b>EMBL Grenoble</b>
<br>
<b>Roger  Williams </b>, MRC-LMB Cambridge, 
<br>
Host: Daren Hart
<br>
[Print version: <a href=events_1?withHeader=x&seminarID=5085 target=_blank>HTML</a>]
</td>
<td class=dark valign=top bgcolor=lightblue><img src="http://emblorg.embl.de/images/spacer.gif" width="10" height="1" border="0">
</td>
</tr>

Syntax of course/conference

<tr>
<td class=green width=15% valign=top><b>
Course:
</b></td><td><img src="http://emblorg.embl.de/images/spacer.gif" width="10" height="1" border="0"></td><td class=dark valign=top align=left>
<a href=http://www-db.embl.de/jss/EmblGroupsOrg/conf_69>
EMBO Practical Course on Microinjection and Detection of Probes in Living Cells</a>
<br>
<b>Sunday, 17 June - Saturday, 23 June 2007, EMBL Heidelberg</b>
<br>
R. Pepperkok, R Saffrich, M. Trendelenburg
</td></tr>
