#!/usr/bin/env perl

# server

use CGI qw/:standard/;
# For serialization:
use Storable;

my $q = new CGI;

# Get arguments sent to CGI:
my $packaged;
my $data;

# Check that there are parameters at all
if (param()) {
    # Get the parameter, as passed to the CGI (still serialized here)
    $packaged = param('data');
    # Convert serialized simple string to reference
    $data = Storable::thaw($packaged);

}

# HTTP header must be returned first from CGI servers
print $q->header;

# Do something with data (this is a ref here)
process($data);

# Packages results and return
# Needs to be a reference parameter
# Use nfreeze() instead of freeze() when sending over a network
$packaged = Storable::nfreeze($data);

# Send the client the actual junk
print $packaged;


################################################################################

sub process {
    my ($data) = @_;

    $data->{'data'}{'field1'} *= 10;
    $data->{'name'} = uc $data->{'name'};

}
