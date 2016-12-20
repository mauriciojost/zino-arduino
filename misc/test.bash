#!/bin/bash

set -x
set -e

CURRDIR=`readlink -e $(dirname $0)`

cd $CURRDIR/..

platformio test -e module-test
platformio test -e delayer-test
platformio test -e bot-test
platformio test -e misc-test
platformio test -e clock-test
platformio test -e pump-test
platformio test -e level-test
platformio run
