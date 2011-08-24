

################################################################################
=head2 _undash

 Function: Remove any leading '-' character from keynames in hash
 Example : _undash(%some_hash); # or for objects: $self->_undash();
 Returns : Reference to the modified hash. 
 Args    : hash

Give a hash, or hash reference, remove any preceeding dash ('-') from keynames.

Modifies the given hash, and returns a reference to it as well. I.e. call by
reference semantics.

Useful in object constructors that receive named parameters as a hash. E.g

 sub new () {
     my ($class, %o) = @_;
     my $self = { %o };
     bless $self, $class;
     $self->_undash;
     return $self;
 }

=cut
sub _undash (\%) {
    my $o = shift;
    foreach my $old (keys %$o) {
        my $new = $old;
        $new =~ s/^-//;
        $o->{$new} = $o->{$old};
        delete $o->{$old};
    }
    return $o;
} # _undash

