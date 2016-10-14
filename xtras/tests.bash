#!/bin/bash

set -x
set -e

CURRDIR=`readlink -e $(dirname $0)`

cd $CURRDIR/..

platformio test -e bot-test
platformio test -e misc-test
platformio test -e clock-test
