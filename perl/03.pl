#!/usr/local/bin/perl
# Report on disk usage under specified files
# The Unix command "du -sk ..." (on BSD Unix, "du -s ...")
# produces a series of lines:
#   1942    bin
#   2981    etc
#   ...
# listing the K bytes used under each file or directory.
# It doesn't show other information, such as the
# modification date or owner.
# This program gets du's kbytes and filename, and merges
# this info with other useful information for each file.
#

    $files = join(' ',@ARGV);

    # The trailing pipe "|" directs command output
    # into our program:

    if (! open (DUPIPE,"du -sk $files | sort -nr |"))  {
        die "Can't run du! $!\n";
    }

    printf "%8s %-8s %-16s %8s %s\n",
        'K-bytes','Login','Name','Modified','File';
    while (<DUPIPE>) {
        # parse the du info:
        ($kbytes, $filename) = split;

        # Call system to look up file info like "ls" does:
        ($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,
            $size,$atime,$mtime,$ctime)
            = stat($filename);

        # Call system to associate login & name with uid:
        if ($uid != $previous_uid) {
            ($login,$passwd,$uid,$gid,$quota,$comment,
                $realname,$dir,$shell) = getpwuid($uid);
            ($realname) = split(',',substr($realname,0,20));
            $previous_uid = $uid;
        }

        # Convert the modification-time to readable form:
        ($sec,$min,$hour,$mday,$mon,$myear) = localtime($mtime);
        $mmonth = $mon+1;

        printf "%8s %-8s %-16s %02s-%02d-%02d %s\n",
            $kbytes, $login, $realname,
            $myear, $mmonth, $mday, $filename;
    }
