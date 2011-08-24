
# One way to support named function parameters. E.g.:
# func(-param1=>2, -param3=>"house");
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
