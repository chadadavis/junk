#!/usr/bin/env perl

use Net::Google::Calendar;

init();

# Get events from 

$url = "http://www.google.com/calendar/feeds/chad.a.davis%40gmail.com/private-38a9ac16e21f594056e4a3c8631378b8/basic";
$u = "user.name"; # Without domain name
$p = '<yourpassword>';

my $cal = Net::Google::Calendar->new( url => $url );
# my $cal = Net::Google::Calendar->new;
    $cal->login($u, $p);


my @events = $cal->get_events();
print "Events=" . scalar(@events) . "\n";

#     for ($cal->get_events()) {
#         print $_->title."\n";
#         print $_->content->body."\n*****\n\n";
#     }


    my $entry = Net::Google::Calendar::Entry->new();
    $entry->title($title);
    $entry->content("My content");
    $entry->location('London, England');
    $entry->transparency('transparent');
    $entry->status('confirmed');
    $entry->when(DateTime->now() + DateTime::Duration->new(days=>2), 
                 DateTime->now() + DateTime::Duration->new( hours => 6 ) );


    my $author = Net::Google::Calendar::Person->new();
    $author->name('Foo Bar');
    $author->email('foo@bar.com');
    $entry->author($author);

    my $tmp = $cal->add_entry($entry);
    die "Couldn't add event: $@\n" unless defined $tmp;

    print "Events=".scalar($cal->get_events())."\n";

    $tmp->content('Updated');

    $cal->update_entry($tmp) || die "Couldn't update ".$tmp->id.": $@\n";

#     $cal->delete_entry($tmp) || die "Couldn't delete ".$tmp->id.": $@\n";

sub init {

}

sub add {

}

