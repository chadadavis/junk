#!/usr/bin/env perl

package oldUtil;
use base qw(Exporter);

our @EXPORT = qw(
                 rearrange
                 file_content
                 verbose
				 int2bitarray
                 modprobe
                 );
our @EXPORT_OK = qw();

################################################################################

# Sucks the contents of a file into a string
sub file_content {
    my ($file) = @_;
    open FH, $file;
    my $content = join("", <FH>);
    close FH;
    return $content;
}

# Print diagnostic/debugging messages
sub verbose  {
    my ($pkg, $file, $line, $func) = caller(1);
    $line = sprintf("%4d", $line);
    return print STDERR ">$file|$line|$func|@_\n" if 
        defined($::VERBOSE) && $::VERBOSE;
}

# Support for named function parameters. E.g.:
# func(-param1=>2, -param3=>5);
sub rearrange  {
    # The array ref. specifiying the desired order of the parameters
    my $order = shift;
    # Make sure the first parameter, at least, starts with a -
    return @_ unless (substr($_[0]||'',0,1) eq '-');
    # Make sure we have an even number of params
    push @_,undef unless $#_ %2;
    my %param;
    while( @_ ) {
        (my $key = shift) =~ tr/a-z\055/A-Z/d; #deletes all dashes!
        $param{$key} = shift;
    }
    map { $_ = uc($_) } @$order; # for bug #1343, but is there perf hit here?
    # Return the values of the hash, based on the keys in @$order
    # I.e. this return the values sorted by the order of the keys
    return @param{@$order};
} # rearrange

# Convert integer to bit array
sub int2bitarray {
    my $i = shift;
    my @a;
    for (my $j = 0, my $c=2**0; $c <= $i; $j++, $c*=2) {
        $a[$j] = ($i & $c) ? 1 : 0;
    }
    return @a;
}

###############################################################################
=head2 modprobe

 Title   : modprobe
 Usage   :    modprobe(MyModule);
           or modprobe(MyModule, qw(function1 function1));
           or modprobe("MyModule.pl");
 Function: Tries to the load the module, with any given parameters
 Returns : Success or failure
 Args    : Module name, plus any symbols to be imported explicitly.

This wrapper is an alternative to letting the interpreter crash,
    when a module is not available. In some cases a program may
    be able to work around a missing module, without aborting,
    by for example loading an alternative module, or by
    implenting a light-weight version of a module-function
    ourselves. The name of the module should generally be given
    as a Bareword (i.e. not inside quotes) parameter. In that
    case, the ".pm" file extension should not be added,
    i.e. modprobe(POSIX). If, however, you want to load a file
    directly, put it quotes, providing its file extension as
    well (either .pl or .pm) for example: modprobe("Thing.pl") 

=cut

sub modprobe {
    my $raw = shift;
    # Set module name to raw parameter name first
    my $mod = $raw;
    # If a raw filename is given (with .pm or .pl extension), then
    #   it needs to be wrapped in quotes before being sent to require()
    if ($mod =~ /[.]p[lm]$/i) { $mod = "\"$mod\"" }
#    $logger->info("Probing $mod");
    eval "require($mod);";
    # However, import will not accept a string, need to use the $raw here
    if (!$@) { import $raw @_; }
#    $logger->warn("Couldn't load $mod") if $@;
    # Return success, if there was no error, otherwise return the error
    return !$@;
} # modprobe


################################################################################

1;
