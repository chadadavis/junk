#!/usr/bin/perl
#=====================================================================
# DECLARATIONS
#=====================================================================
use warnings;
use strict;

use Syntax::Highlight::Perl;

my $color_table = {
    'Variable_Scalar'   => 'color:#008800;',
    'Variable_Array'    => 'color:#ff7700;',
    'Variable_Hash'     => 'color:#8800ff;',
    'Variable_Typeglob' => 'color:#ff0033;',
    'Subroutine'        => 'color:#998800;',
    'Quote'             => 'color:#0000aa;',
    'String'            => 'color:#0000aa;',
    'Comment_Normal'    => 'color:#006699;font-style:italic;',
    'Comment_POD'       => 'color:#001144;font-family:' .
                               'garamond,serif;font-size:10pt;',
    'Bareword'          => 'color:#33AA33;',
    'Package'           => 'color:#990000;',
    'Number'            => 'color:#ff00ff;',
    'Operator'          => 'color:#000000;',
    'Symbol'            => 'color:#000000;',
    'Keyword'           => 'color:#000000;',
    'Builtin_Operator'  => 'color:#330000;',
    'Builtin_Function'  => 'color:#000011;',
    'Character'         => 'color:#880000;',
    'Directive'         => 'color:#339999;font-style:italic;',
    'Label'             => 'color:#993399;font-style:italic;',
    'Line'              => 'color:#000000;',
};

#=====================================================================
# PROGRAM PROPER
#=====================================================================

my $formatter = Syntax::Highlight::Perl->new();

$formatter->define_substitution('<' => '&lt;', 
                                '>' => '&gt;', 
                                '&' => '&amp;'); # HTML escapes.

# install the formats set up above
while ( my ( $type, $style ) = each %{$color_table} ) {

    $formatter->set_format($type, [ qq|<span style=\"$style\">|, 
                                    '</span>' ] );
}

my $file = shift || die "Give me a perl file to colorize!\n";
-e $file or die "There's no such file: $file\n";

open F, '<', $file or die $!;

print '<PRE style="font-size:8pt;color:#333366;">';
while (<F>) {

    print $formatter->format_string;
}
print "</PRE>";
close F;

exit 0;
#=====================================================================
