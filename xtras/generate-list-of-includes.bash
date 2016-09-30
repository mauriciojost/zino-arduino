#!/bin/bash

echo "This script aims to help adding the includes in the setup of Intellij."

CURRDIR=$(readlink -e `dirname $0`)


echo "Cleaning environment..."

rm -fr lib *.list


echo "Bringing UC libraries to an isolated library and including in the list..."

mkdir lib
cp $HOME/.platformio/packages/toolchain-atmelavr/avr/include/avr/iom328p.h lib
echo '<include-dir path="'$CURRDIR/lib'"/>' >> includes.list


echo "Including platformio detected libraries in the list..."

cat $CURRDIR/../.gcc-flags.json | grep gccIncludePaths | sed "s/gccIncludePaths//" | sed 's/"//g' | sed 's/://g' | sed 's/,/\n/g' > libraries.list
while read -r line
do
  echo '<include-dir path="'$line'"/>' >> includes.list
done < libraries.list
rm libraries.list

echo "Done."

echo "Replace the lines in includes.list in the .idea/misc.xml IDEA configuration file."
