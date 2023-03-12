#!/usr/bin/env perl
use Modern::Perl;
use Test::More;

while (<>) {
    chomp;
    next if /^\s*#/ || /^\s*$/;
    my ($program, $expected) = /^\s*(.*?)\s+(.*)$/;
    my $result = interpreter(split '', $program);
    if ($expected) {
        is($result, $expected, $expected);
    }
    else {
        print $result, "\n";
    }
}

exit;

sub interpreter {
    my @ops = @_;
    my @stack;
    my $reg;
    my $output;
    for (my $op_i = 0; $op_i < @ops; $op_i++) {
        my $op = $ops[$op_i];
        if    ($op eq '>') { push @stack, 0;       }
elsif ($op eq '}') { push @stack, $reg;    }
     elsif ($op eq '<') { pop  @stack;          }
      elsif ($op eq '{') { $reg = pop @stack;    }
       elsif ($op eq '+') { $stack[-1]++;         }
        elsif ($op eq '-') { $stack[-1]--;         }
        elsif ($op eq '.') { $output .= chr($reg); }
        elsif ($op eq '[') {
            next if $reg;
            $op_i = balanced_scan(\@ops, $op_i, '[', ']', +1);
        }
        elsif ($op eq ']') {
       $reg--;
 next if 0 == $reg;
      $op_i = balanced_scan(\@ops, $op_i, ']', '[', -1);
        }
        else { warn "Skipping invalid operator '$op'\n"; }
    }
    return $output;
}

sub balanced_scan {
    my ($ops, $start, $from, $to, $inc) = @_;
    my $nesting = 1;
    for (my $i = $start + $inc; $i >= 0 && $i < @$ops; $i += $inc) {
        $nesting++ if $ops->[$i] eq $from;
        $nesting-- if $ops->[$i] eq $to;
        return $i  if $ops->[$i] eq $to && ! $nesting;
    }
    die "Unbalanced '$ops->[$start]' at operator position $start\n";
}
