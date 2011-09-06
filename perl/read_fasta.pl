#!/usr/bin/env perl

my $file = shift or die;

my $data = slurp($file);

# Strip off Fasta header and remove new lines and whitespace
$data =~ s|>.*?\n||;
$data =~ s|\n||g;
$data =~ s|\s||g;



# Slurps the contents of a (unopened) file (given path to file) into a string
sub slurp {
    my ($file) = @_;
    local $/;
    open my $fh, "<$file";
    my $data = <$fh>;
    close $fh;
    return $data;
}
