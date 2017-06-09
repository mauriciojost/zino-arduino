#!/bin/bash

g++ -o simulator.bin -D LEVEL_TEST -D SIMULATION_MODULE -D UNIT_TEST -D SUBCYCLES_2 -D LOG_LEVEL=0 -D BINARY_LEVEL libs/log4ino-arduino/src/log4ino/*.cpp libs/main4ino-arduino/src/main4ino/*.cpp libs/Unity/src/*.c test/LevelTest.cpp src/Main.cpp src/Module.cpp src/actors/Delayer.cpp src/actors/Level.cpp src/actors/Pump.cpp src/ui/Lcd.cpp src/hardware/Servox.cpp -I src/ui/ -I src/actors/ -I src/hardware/ -I libs/log4ino-arduino/src/log4ino/ -I libs/main4ino-arduino/src/ -I libs/Unity/src -I src 

./simulator.bin
