#!/bin/bash

FLAGS="-D SIMULATION_MODULE -D UNIT_TEST -D SUBCYCLES_2 -D LOG_LEVEL=0"
SRC="src/*.cpp src/actors/*.cpp src/ui/*.cpp src/hardware/*.cpp"
HEADERS="-I src/ -I src/ui/ -I src/actors/ -I src/hardware/"
g++ -o simulator.bin $FLAGS test/Simulation.cpp $SRC $HEADERS 
./simulator.bin
rm simulator.bin
