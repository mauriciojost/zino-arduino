#!/bin/bash

set -x 
set -e 

CURR_DIR=$(readlink -e `dirname $0`)
ROOT_DIR="$CURR_DIR/../"

LOGS=${1:-0}
SIMULATOR_BIN=simulator.bin
LIBS="-lpthread"
FLAGS="-D SIMULATION_MODULE -D UNIT_TEST -D SUBCYCLES_2 -D LOG_LEVEL=$LOGS -D BINARY_LEVEL"
SRC="$ROOT_DIR/src/*.cpp $ROOT_DIR/src/actors/*.cpp $ROOT_DIR/src/ui/*.cpp $ROOT_DIR/src/hardware/*.cpp"
HEADERS="-I $ROOT_DIR/src/ -I $ROOT_DIR/src/ui/ -I $ROOT_DIR/src/actors/ -I $ROOT_DIR/src/hardware/"

g++ -o $SIMULATOR_BIN $FLAGS test/Simulation.cpp $SRC $HEADERS $LIBS
./$SIMULATOR_BIN
rm $SIMULATOR_BIN
