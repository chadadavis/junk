package SBG::Catalyst::Demo::Controller::Root;
use Moose;
use namespace::autoclean;

BEGIN { extends 'Catalyst::Controller' }

#
# Sets the actions in this controller to be registered with no prefix
# so they function identically to actions created in MyApp.pm
#
__PACKAGE__->config(namespace => '');

=head1 NAME

SBG::Catalyst::Demo::Controller::Root - Root Controller for SBG::Catalyst::Demo

=head1 DESCRIPTION

[enter your description here]

=head1 METHODS

=head2 index

The root page (/)

=cut

sub index : Path : Args(0) {
    my ($self, $c) = @_;

    # Hello World
#     $c->response->body($c->welcome_message);
}

# But how to link this to the /text URI ? : Local is sufficient
# Will otherwise look for root/text.tt (based on method name)
sub text : Local {
    my ($self, $c) = @_;
    $c->res->content_type('text/text-plain');
    # The suggested download file name
    $c->res->header(
        'Content-Disposition' => qq(attachment; filename="data.txt"),
    );
    $c->res->body("Some text!");
}



=head2 default

Standard 404 error page

=cut

sub default :Path {
    my ( $self, $c ) = @_;
    $c->response->body( 'Page not found' );
    $c->response->status(404);
}

use Data::Dumper;
sub list : Local {
    my ($self, $c) = @_;

    $c->log->debug(Dumper { some => 1, thing => 2 });
#     $c->response->body('hello');
    $c->stash(template => 'some_template.tt');
}

=head2 end

Attempt to render a view, if needed.

=cut

sub end : ActionClass('RenderView') {}

=head1 AUTHOR

Chad Davis

=head1 LICENSE

This library is free software. You can redistribute it and/or modify
it under the same terms as Perl itself.

=cut

__PACKAGE__->meta->make_immutable;

1;
