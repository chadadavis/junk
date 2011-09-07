#!/usr/bin/env perl
use Modern::Perl;
use Log::Dispatch::File;

# Of course, this is also necessary, so that Perl parses the source correctly
use utf8;

sub _log {
    my $log = Log::Dispatch->new(
        outputs => [
            [
                'File',
                min_level => 'info',
                filename  => __FILE__ . '.log',
                mode      => '>>',
                newline   => 1,
                @_,
            ],
        ],
    );
}

# Broken log
my $log0     = _log();
# Fixed for utf8
my $log_utf8 = _log(
    binmode => ':encoding(UTF-8)',
);

$log0->error(    'broken: schön');
$log_utf8->error('fixed : schön');

