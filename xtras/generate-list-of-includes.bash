#!/bin/bash

echo "This script aims to help adding the includes in the setup of Intellij."

CURRDIR=`dirname $0`

cat $CURRDIR/../.gcc-flags.json | grep gccIncludePaths | sed "s/gccIncludePaths//" | sed 's/"//g' | sed 's/://g' | sed 's/,/\n/g' > libraries.list

echo "" > includes.list
while read -r line
do
  echo '<include-dir path="'$line'"/>' >> includes.list
done < libraries.list

echo "Replace the lines in libraries.list in the .idea/misc.xml IDEA configuration file."