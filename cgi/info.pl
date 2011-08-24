#!/usr/bin/env perl

use CGI;

my $cgi = new CGI;

################################################################################
# Print  form

print 
    $cgi->header(-type=>"text/html"), 
    $cgi->start_html(-title=>"Web Server Info."),
    ;

################################################################################

my $uname = `uname -a` || "<unknown>";
chomp $uname;
print "<br />uname: <pre>$uname</pre>\n";

my $file = glob('/etc/*[Vv]ersion*') || glob('/etc/*[Rr]elease');
my $release = `cat $file` || "<unknown>";
chomp $release;
print "<br />Release: <pre>$file\n$release</pre>\n";

print "/etc/issue : " . `cat /etc/issue*` . "<br /><br />\n";

print "Perl Version: $] <br /><br />\n";

print "LIBC: ", join(", ", glob('/lib/libc[.-]*')), "<br /><br />\n";

my $ncpus = `/home/d/davis/bin/numcpus` || "<unknown>";
chomp $ncpus;
print "<br />Number of CPUs on this server: <pre>$ncpus</pre>\n";

my $cpu = `cat /proc/cpuinfo` || "<unknown>";
chomp $cpu;
print "<br />/proc/cpuinfo: <pre>$cpu</pre>\n";

print "CGI Environment:<br/><br />\n";
for (keys %ENV) {
	print "$_ : $ENV{$_}<br />\n";
}

################################################################################
print "<hr />\n";
print $cgi->end_html();


