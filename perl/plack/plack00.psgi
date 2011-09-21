#!/usr/bin/env plackup

my $app = sub {
   my ($env) = @_;
   my %headers = (
       'Content-Type' => 'text/html',
   );
   my $content = <<EOL;
<html>
<head></head>
<body>
Hey there
</body>
</html>
EOL

   my $x = 101;
   die if $x > 100;

   return [200, [ %headers ], [ $content ] ];
};

use Plack::Builder;
return builder {
    # Debug info on page requires Content-Type => text/html
#    enable 'Debug';
    # Or enable just the NYTProf panel
    enable 'Debug', panels=> [[Profiler::NYTProf]];

    # InteractiveDebugger launched when app dies/exceptions
    enable 'InteractiveDebugger';
    $app;
}

