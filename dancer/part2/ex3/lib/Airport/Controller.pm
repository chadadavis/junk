package Airport::Controller;
use 5.010;
use Dancer;
use Airport::Data;
use Airport::Search;
use Data::Types qw/is_float/;
use List::MoreUtils qw/all/;

my $airports = Airport::Data::parse_airports(config->{airports_csv});

get '/' => sub {
    template 'index';
};


get '/results' => sub {
    my $search_string = params->{search_string};
    my $search_result = Airport::Data::parse_search_string($search_string);
    template 'index' => {
        search_string => $search_string,
        num_airports => scalar @$airports,
        search_results => [
            # For each airport
            grep {
                my $airport = $_;
                # Does is match any of the keys requested
                all {
                    is_float $search_result->{$_}
                        ? $airport->{$_} ~~ /^$search_result->{$_}/
                        : $airport->{$_} ~~ /$search_result->{$_}/
                } keys %$search_result
            } @$airports
        ],
    };
};

get '/near' => sub {
    # slice of hashref
    my ($latitude, $longitude) = @{params()}{qw(latitude longitude)};

    template 'index' => {
        num_airports => scalar @$airports,
        search_string => "$latitude, $longitude",
        search_results => Airport::Search::get_latlong_matching_airports(
            airports => $airports,
            latitude => $latitude,
            longitude => $longitude,
            max => 1.0,
        ),
    };
};

1;
