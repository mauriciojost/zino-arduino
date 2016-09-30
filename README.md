# README

This is a low-budget plant watering system.

It is based on the [Arduino Pro Mini](https://www.arduino.cc/en/Main/ArduinoBoardProMini) board.

## Develop

The project makes use of `platformio` IoT ecosystem.

### Environment setup

After compilation, the file `.gcc-flags.json` contains the path of header `.h` files resolved by `platformio`.

### Compile

```
platformio run
```

### Tests

```
platformio test -e localtest
```

### Uploads

```
platformio run --target upload
```
