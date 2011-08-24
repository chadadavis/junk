# Perl script that logs in to Gmail, retrieves the user defined labels
# Then prints out all new messages under the first label

require "Gmail.pm";

my $user = "user.name";
my $pass = "<password>";
chomp $user, $pass;

my $gmail = Mail::Webmail::Gmail
    ->new( 
           username => $user, password => $pass, 
           proxy_name => 'http://proxy.lrz-muenchen.de:8080'
           );
$gmail or die "No gmail";
$gmail->login;
print $gmail->error || 0, $gmail->error_msg || "<No error>\n";

exit;

my @labels = $gmail->get_labels();

print "labels: @labels";

my $messages = $gmail->get_messages( label => $labels[0] );

foreach ( @{ $messages } ) {
    if ( $_->{ 'new' } ) {
        print "Subject: " . $_->{ 'subject' } . " / Blurb: " . $_->{ 'blurb' } . "\n";
    }
}
