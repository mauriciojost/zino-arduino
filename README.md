# README

This is a low-budget plant watering system.

It is based on the [Arduino Pro Mini](https://www.arduino.cc/en/Main/ArduinoBoardProMini) board.

## Develop

The project makes use of `platformio`, an IoT open-source ecosystem.

### Environment setup

About the ecosystem: use `platformio` v3.1.0 or higher.

About the IDE: use Intellij IDEA 14 with the C/C++ plugin works like a charm. 

About the formatter: use `clang-format` tool.

#### Includes (.h) in Intellij

After compilation (see below), the file `.gcc-flags.json` contains the path of header `.h` files resolved by `platformio`.

Use the scripts provided in `xtras` to add them to your IDE.

You will have to declare the macro `INTELLIJ` in the project, so that missing libraries (like the ones usually resolved by `platformio`) are included by Intellij.

### Compile

```
platformio run
```

### Test

```
bash misc/test.bash
```

### Upload

```
platformio run --target upload
```

### Monitor serial port

```
pio -f -c atom serialports monitor --port /dev/ttyUSB0 -b 115200
```

