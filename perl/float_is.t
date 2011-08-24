sub float_is ($$;$$) {
   my ($val1, $val2, $msg, $tol) = @_;
   return unless defined($val1) && defined($val2);
   $tol = '10%' unless defined $tol;

   my $diff = abs($val1-$val2);
   $msg ||= "float_is: $diff < $tol (from $val1)";

   my $ok;
   if ($tol =~ /(\d+)\%$/) {
       my $perc = $1 / 100.0;
       $ok = ok($diff < $perc * abs($val1), $msg);
   } else {
       $ok = ok($diff < $tol, $msg);
   }

   if($ok) {
       return 1;
   } else {
       printf STDERR 
           "\t|%g - %g| == %g exceeds tolerance: %s\n", 
           $val1, $val2, $diff, $tol;
   }
}
