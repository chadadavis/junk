#!/usr/bin/env sh

# Add this to the beginning of any wrapper that needs to find its installdir

canonical=`readlink -f "$0"`
installdir=`dirname "$canonical"`
echo "$installdir"

