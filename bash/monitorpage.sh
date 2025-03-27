#!/usr/bin/env bash

# Monitors a web page (URL) for changes
url=$1
if [ -z "$url" ]; then exit; fi
# Email address to be notified (default: $USER@localhost)
email=${2:-$USER@localhost}
# Sleep for N seconds between checks (default 30)
sleep=${3:-30}

dir=~/.monitorpage
mkdir -p $dir
cd $dir

page=`basename $url`

echo "Notifying $email of changes at: "
echo -e "\t$url\n"

# Get initial version
wget -qN $url || exit;
/bin/cp -fu $page ${page}.ref
echo $url | mail -s "Watching $url ..." $email 

echo "Sleeping $sleep seconds between checks"
while sleep $sleep; do
    # use -N for timestamping
    wget -qN $url || exit;
    if [ $page -nt ${page}.ref ]; then 
        echo $url | mail -s "$url changed" $email
        echo "$url changed at:"
    fi
    # This is when it last changed
    echo `stat $page --print="%y"`

    # Copy over, but only if it's been -u ('updated')
    /bin/cp -fu $page ${page}.ref
done


