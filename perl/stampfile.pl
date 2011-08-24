
# returns EMBL::Transform
# Transformation will be relative to fram of reference of destdom
sub stampfile {
    my ($srcdom, $destdom) = @_;

    # STAMP uses lowercase chain IDs
    $srcdom = lc $srcdom;
    $destdom = lc $destdom;

    if ($srcdom eq $destdom) {
        # Return identity
        return new EMBL::Transform;
    }

    print STDERR "\tSTAMP ${srcdom}->${destdom}\n";
    my $dir = "/tmp/stampcache";
    `mkdir /tmp/stampcache` unless -d $dir;
#     my $file = "$dir/$srcdom-$destdom-FoR.csv";
    my $file = "$dir/$srcdom-$destdom-FoR-s.csv";

    if (-r $file) {
        print STDERR "\t\tCached: ";
        if (-s $file) {
            print STDERR "positive\n";
        } else {
            print STDERR "negative\n";
            return undef;
        }
    } else {
        my $cmd = "./transform.sh $srcdom $destdom $dir";
        $file = `$cmd`;
    }

    my $trans = new EMBL::Transform();
    unless ($trans->loadfile($file)) {
        print STDERR "\tSTAMP failed: ${srcdom}->${destdom}\n";
        return undef;
    }
    return $trans;
} # stampfile
