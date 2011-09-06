use strict;
use warnings;
use lib 'lib';
use SBG::Catalyst::Demo;

my $app = SBG::Catalyst::Demo->apply_default_middlewares(SBG::Catalyst::Demo->psgi_app);
$app;

