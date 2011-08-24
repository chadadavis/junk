#!/usr/bin/perl -W
use Gaim;

%PLUGIN_INFO = (
    perl_api_version => 2,
    name             => "Your Plugin's Name",
    version          => "0.1",
    summary          => "Brief summary of your plugin.",
    description      => "Detailed description of what your plugin does.",
    author           => "Your Name <email@address>",
    url              => "http://yoursite.com/",

    load             => "plugin_load",
    unload           => "plugin_unload"
);

sub plugin_init {
    return %PLUGIN_INFO;
}

sub plugin_load {
    my $plugin = shift;
}

sub plugin_unload {
    my $plugin = shift;
}
