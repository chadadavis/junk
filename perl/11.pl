#!/usr/local/bin/perl -w

sub println
{
	my(@str) = @_ ? @_ : "";
	print("@str \n");
}



sub main()
{

printf "$ENV{'SSH_AGENT_PID'}\n";

$MYDIR="/home/cad8/tmp/code/perl/11.pl";

open MYDIR or die ("Fuck");

while (<MYDIR>)
{
	println("$. $_");
}

################################################################################

@stuff=qw(flying gliding skiing dancing parties racing);

print "There are ",scalar(@stuff)," elements in \@stuff\n";
print join ":",@stuff,"\n";

@mapped  = map  /ing/, @stuff;
@grepped = grep /ing/, @stuff;

print "There are ",scalar(@stuff)," elements in \@stuff\n";
print join ":",@stuff,"\n";

print "There are ",scalar(@mapped)," elements in \@mapped\n";
print join ":",@mapped,"\n";

print "There are ",scalar(@grepped)," elements in \@grepped\n";
print join ":",@grepped,"\n";

################################################################################

@letters= qw(a b c d e);

@ords=map ord, @letters;
print join ":",@ords,"\n";

@chrs=map chr, @ords;   
print join ":",@chrs,"\n";


################################################################################

println();

@stuff=qw(flying gliding skiing dancing parties racing);

print join ":",@stuff,"\n";

@mapped  = map  { s/(^[gsp])/$1 x 2/e } @stuff;
@grepped = grep { s/(^[gsp])/$1 x 2/e } @stuff;

print join ":",@stuff,"\n";
print join ":",@mapped,"\n";
print join ":",@grepped,"\n";

################################################################################



@words=@ARGV;

print "Output Field Separator is :$,:\n";
print '1. Words:', @words, "\n";

&change;

$,='_';

print "\nOutput Field Separator is :$,:\n";
print '2. Words:', @words, "\n";

&change;

sub change {
        print '   Words:', @words, "\n";
	}

################################################################################


} # main()


main();
0;

