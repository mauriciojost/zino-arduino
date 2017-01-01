#!/bin/bash

LOGS=${1:-0}
FLAGS="-D SIMULATION_MODULE -D UNIT_TEST -D SUBCYCLES_2 -D LOG_LEVEL=$LOGS"
SRC="src/*.cpp src/actors/*.cpp src/ui/*.cpp src/hardware/*.cpp"
HEADERS="-I src/ -I src/ui/ -I src/actors/ -I src/hardware/"
SIMULATOR_BIN=simulator.bin

g++ -o $SIMULATOR_BIN $FLAGS test/Simulation.cpp $SRC $HEADERS 
./$SIMULATOR_BIN
rm $SIMULATOR_BIN
