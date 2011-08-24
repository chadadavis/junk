#!/usr/bin/env perl

my @cd = (5, 6, 2, 7, 3, 1, 9, 4, 8, 0);
my @al = (7, 2, 3, 5, 4, 6, 1, 9, 8, 0);

my $al=dist(\@cd, \@al);

my $n = 10000;
my $better;
for (1..$n) {
    my @a = permute(100, \@cd);
    my $d = dist(\@cd, \@a);
    $better++ if $d < $al;
}

print "$al : ", $better / $n, "\n";




sub dist {
    my ($a, $b) = @_;

    my $sum;
    for (my $i=0;$i<@$a;$i++) {
        my $key = $a->[$i];
        my $j = find($key, @$b);
        my $diff;
        if ($j == -1) {
            # Not found? Max length punishmen
            $sum += @$a;
        } else {
            $diff = abs($i - $j);
            $sum += $diff;
        }
#         print STDERR "$i $j $diff\n";
    }
    return $sum;
}

sub find {
    my ($key, @array) = @_;
    for (my $i=0;$i<@array;$i++) {
        if ($key == $array[$i]) {
            return $i;
        }
    }
    return -1;
}

sub permute {
    my ($iters, $arr) = @_;
    my @nums = @$arr;
    for (my $i = 1; $i <= $iters; $i++) {
        if (rand(1) <= .5) {
            #  Flip a coin, and if heads swap
            # a random adjacent pair of elements.  
            my $k = int(rand(@nums-1));
            ($nums[$k], $nums[$k+1]) = ($nums[$k+1], $nums[$k]);
        }
    }
    return @nums;
}
