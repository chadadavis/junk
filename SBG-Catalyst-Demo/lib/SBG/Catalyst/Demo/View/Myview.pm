package SBG::Catalyst::Demo::View::Myview;

use strict;
use warnings;

use base 'Catalyst::View::TT';

__PACKAGE__->config(
    TEMPLATE_EXTENSION => '.tt',
    render_die => 1,
);

=head1 NAME

SBG::Catalyst::Demo::View::Myview - TT View for SBG::Catalyst::Demo

=head1 DESCRIPTION

TT View for SBG::Catalyst::Demo.

=head1 SEE ALSO

L<SBG::Catalyst::Demo>

=head1 AUTHOR

Chad Davis

=head1 LICENSE

This library is free software. You can redistribute it and/or modify
it under the same terms as Perl itself.

=cut

1;
