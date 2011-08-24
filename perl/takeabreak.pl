#!/usr/bin/env perl

# How many minutes long are your work runs (between breaks)
# A short break means: stand up, look around, stretch, sit back down
# Time between short breaks:
my $short = 15;
# A long break means: go have a coffee / go for a walk
# Time between long breaks: 
my $long = 90;
# How often (minutes) to bug user to take a break
my $wait = 5;


# Commond for a console beep to get user's attention
my $beep = "aplay $ENV{HOME}/misc/conf/KDE_Notify.wav 2>/dev/null";
# my $beep;

################################################################################

our $lastbreak = time();
exit(main());

sub main {
    # Convert minutes to seconds
    $short *= 60;
    $long *= 60;
    $wait *= 60;

   while (1) {
       # A short work run
       sleep($short);
       # First check if it's time for a long/coffee break
       if (longbreak()) {
           # Don't need to take a short break if we just took a long break
           next;
       } else {
           system($beep) if $beep;
           system('zenity --info --text "Take a short break, then hit OK."');
       }
   }
}

sub longbreak {
    our $lastbreak;
    # Has it been to long since the last coffee break?
    if (time() - $lastbreak > $long) {
        # Continually bug user to take a break
        while (1) {
            system($beep) if $beep;
            last unless 
                system('zenity --question --text "Take a long break now?"');
            # Not ready to take a break yet
            sleep($wait);
        }
        # Wait for user to get back from break
        system('zenity --info --text "Take a long break, then hit OK."');
        # Break's over. Note the beginning of the next work cycle
        $lastbreak = time();
        # Yes, we took a break
        return 1;
    } else {
        # No, we didn't need to take a break yet
        return 0;
    }
}
