#!/bin/bash

set -x 
set -e 

LOGS=${1:-0}
SIMULATOR_BIN=simulator.bin
LIBS="-lpthread"
FLAGS="-D SIMULATION_MODULE -D UNIT_TEST -D SUBCYCLES_2 -D LOG_LEVEL=$LOGS -D BINARY_LEVEL"
SRC="src/*.cpp src/actors/*.cpp src/ui/*.cpp src/hardware/*.cpp"
HEADERS="-I src/ -I src/ui/ -I src/actors/ -I src/hardware/"

g++ -o $SIMULATOR_BIN $FLAGS test/Simulation.cpp $SRC $HEADERS $LIBS
./$SIMULATOR_BIN
rm $SIMULATOR_BIN
