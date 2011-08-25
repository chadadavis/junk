#!/usr/bin/perl

use Dancer;

set logger  => 'console';
set log     => 'debug';
set show_errors => 1;

 set engines => {
     template_toolkit => 
     {
         start_tag => '[%',
         stop_tag  => '%]'
     }
 };
 set template  => 'template_toolkit';

get '/' => sub {
#    die 'fail';

    return "<h1>Hello World!</h1>";
};

get '/hello/:name' => sub {
    # params() is dancer method
    return "<h1>Hello, " . params->{name} . "</h1>";
};

use DateTime;
get '/time' => sub {
    my $dt = DateTime->now(time_zone => 'Europe/Riga');
    my $ymd = $dt->ymd;
    my $hms = $dt->hms;
#    return template 'date_time' => { time => $dt->hms, date => $dt->ymd };
    return template 'date_time' => { dt=>$dt };
};

get '/form' => sub {
    template 'hello-adj-index';
};

get '/hello-adj' => sub {
  my $params = params();
    return
  "<h1>Hello ".params->{adjective}." Things!</h1>"
};

# Can have the same name as a get method?
post '/hello-adj' => sub {
  my $params = params();
#    return "<h1>Hello post ".params->{adjective}." Things!</h1>";
  
    # form has multiple fields with same name, added to an ArrayRef
    template 'multi' => { adjective_list => params->{adjective} };
};

get '/multi' => sub {
    my @adjectives = split ',', params->{adjectives};
    template 'multi' => { adjective_list => \@adjectives };
};

get '/post-form' => sub {
    template 'hello-adj-post';
};

get '/params' => sub {
    template 'params' => { params => scalar params };
};

Dancer->dance;
