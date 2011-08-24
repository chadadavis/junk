#!/usr/bin/env perl

# client

use LWP::Simple;
# For serialization:
use Storable;
# Just for testing, to print the results
use Data::Dumper;

my $base = "http://localhost/cgi-bin/server.pl";

# A reference to some data structure
my $data = {
            'name' => 'test',
            'age' => 20,
            'data' => {
                'field1' => 3.14159,
                'field2' => 2e-10,
                'field3' => "stuff things",
            },
        };

# Package up the data into network-compatible serialized string
# Note: $data must be a reference
# use nfreeze() instead of freeze() on the network
my $packaged = Storable::nfreeze($data);

# Call server CGI process with network-serialized data
my $url = "${base}?data=${packaged}&";
my $result = get($url);
            
# Result is a network serialized string,
# converted back to a reference
my $newref = Storable::thaw($result);

# Use Data::Dumper, for example, to see whole object
print Dumper($newref);

