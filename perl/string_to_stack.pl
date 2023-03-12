#!/usr/bin/env perl
use warnings;
use strict;

use Math::Factor::XS('factors', 'matches');

use constant DEBUG => 0;

sub string_to_stackr
{   
    my ($str) = @_;
    warn 'INPUT: ' . $str if +DEBUG;
    my @chars = split(//, $str);
    
    # allocate initial memory
    my $output = ('>') x scalar(@chars);
    
    foreach my $char (@chars)
    {     
        my $ord = ord($char);
        my @factors = factors($ord);
        my @matches = matches($ord, \@factors, { skip_multiples => 0 });
        
        my ($f0, $f1); 
        if(@factors == 0)
        {   
            ($f0, $f1) = (1, $ord);
        }
        elsif(@factors == 2)
        {   
            ($f0, $f1) = @factors;
        }
        else
        {   
            warn 'FACTOR_COUNT: '. scalar(@factors) if +DEBUG;
            warn 'FACTORS: ' . join(', ', @factors) if +DEBUG;
            warn 'ORDINAL: ' . $ord if +DEBUG;
            warn 'MATCH_COUNT: ' . scalar(@matches) if +DEBUG;
            warn 'MATCHES: ' . join(', ', @{$matches[-1]}) if +DEBUG;
            ($f0, $f1) = @{$matches[-1]};
        }
        
        $output .= '>' . join('',('+') x $f0) . '{';
        $output .= '[' . join('',('+') x $f1) . ']';
        $output .= '{.'
    }
    
    print $output . "\n";
}

my $input = do { local $\ = undef; <> };
chomp $input;
string_to_stackr($input);

