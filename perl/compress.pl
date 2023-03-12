#!/usr/bin/env perl
# Glassdoor : http://u.booking.com/sv
use Modern::Perl;
use Data::Dump qw(dump);
use Devel::Comments;

my @in = (1,1,1,2,2,55,55,55,238,238,238,238,91,91,91,2,2,2,2,1,4,7,7,7,12,127,);

comp(\@in, \&delta);
comp(\@in, \&rle);

sub comp {
    my ($in, $sub) = @_;
    my $in_str = "@$in";
    my @out = $sub->(@$in);
    my $out_str = "@out";
    my $diff = length($in_str) - length($out_str);
    say length($in_str) . ' ' . $in_str;
    say length($out_str) . ' ' . $out_str;
    say sprintf "diff: $diff %0.2f%%", 100*$diff/length($in_str);
}

sub rle {
    my @res;
    my $block_len = 1;
    for (my $i = 1; $i < @_; $i++) {
        if ($_[$i] == $_[$i-1]) {
            $block_len++;
        }
        else {
            push @res, $_[$i-1], $block_len;
            $block_len = 1;
        }
    }
    # block_len will be correct, but last element always skipped, add explicitly
    push @res, $_[-1], $block_len;
    return @res;
}

sub delta {
    my @res = $_[0];
    for (my $i = 1; $i < @_; $i++) {
        push @res, $_[$i] - $_[$i-1];
    }
    return @res
}
