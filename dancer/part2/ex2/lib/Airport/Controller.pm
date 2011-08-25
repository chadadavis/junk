package Airport::Controller;
use Dancer;
use Airport::Data;

my $airports = Airport::Data::parse_airports(config->{airports_csv});

get '/' => sub {
    template 'index';
};


get '/results' => sub {
    my $search_string = params->{search_string};
    template 'index' => {
        search_string => $search_string,
        num_airports => scalar @$airports,
        search_results => [
            grep { $_->{name} =~ /$search_string/i } @$airports
        ]
#             [
#             { name => 'Kentucky Fried Airport', iso_country => 'Kentucky'  },
#             { name => 'McAirport', iso_country => 'United States of Texas' },
#         ],
    };
};


1;
