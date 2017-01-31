#!/bin/bash

set -x
set -e

CURRDIR=`readlink -e $(dirname $0)`

cd $CURRDIR/..

echo $PATH
which platformio
which pio

export version=0.7-SHT
export subcycles=SUBCYCLES_2
export loglevel=0
#export debug=DEBUG
export debug=NO_DEBUG
export level_actor_config=BINARY_LEVEL
#export level_actor_config=ADC_LEVEL

export test_build_flags="-D UNIT_TEST -D VERSION=v${version} -D ${subcycles} -D ${level_actor_config} -D DEBUG"

ARDUINO_AVR_FR="$HOME/.platformio/packages/framework-arduinoavr"
ARDUINO_AVR_TC="$HOME/.platformio/packages/toolchain-atmelavr"
TOOL_UNITY="$HOME/.platformio/packages/tool-unity/"
HEADERS_CORE_TEST="-I $TOOL_UNITY/"
HEADERS_PROJECT="-I src/ -I src/ui/ -I src/actors/ -I src/hardware/"
HEADERS_CORE="-I $ARDUINO_AVR_FR/cores/arduino/ -I $ARDUINO_AVR_TC/avr/include/ -I $ARDUINO_AVR_FR/variants/atmega328pb/"
HEADERS_DEPS="-I $ARDUINO_AVR_FR/libraries/Servo/src/"

function test_native() {

  EXTRAS="$1"
  TESTER_BIN=tester.bin
  FLAGS="$test_build_flags"
  SRC="test/*.cpp src/*.cpp src/actors/*.cpp src/ui/*.cpp src/hardware/*.cpp $TOOL_UNITY/*.c"
  g++ -o $TESTER_BIN $FLAGS $SRC $HEADERS_PROJECT $HEADERS_CORE $HEADERS_CORE_TEST $HEADERS_DEPS $EXTRAS -fpermissive
  ./$TESTER_BIN
  rm $TESTER_BIN
}

#
#platformio test -e module-test
#platformio test -e delayer-test
#platformio test -e bot-test
#platformio test -e misc-test
#platformio test -e clock-test
#platformio test -e pump-test
#platformio test -e level-test


test_native "${test_build_flags} -D BOT_TEST -D LOG_LEVEL=0"
#test_native "${test_build_flags} -D MISC_TEST -D LOG_LEVEL=0"
#test_native "${test_build_flags} -D CLOCK_TEST -D LOG_LEVEL=2"
#test_native "${test_build_flags} -D PUMP_TEST -D LOG_LEVEL=2"
#test_native "${test_build_flags} -D LEVEL_TEST -D LOG_LEVEL=0"
#test_native "${test_build_flags} -D DELAYER_TEST -D LOG_LEVEL=0"
#test_native "${test_build_flags} -D MODULE_TEST -D LOG_LEVEL=0"

platformio run
