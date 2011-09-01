package Airport::Controller;
use Dancer;

get '/' => sub {
    template 'index';
};

get '/results' => sub {
    my $search_string = params->{search_string};
    template 'index' => {
        search_string => $search_string,
        search_results => [
            { name => 'Kentucky Fried Airport', iso_country => 'Kentucky'  },
            { name => 'McAirport', iso_country => 'United States of Texas' },
        ],
    };
};



1;
