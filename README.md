# README

This is a _very low-budget_ indoor *plant watering system* project, that provides both hardware and firmware implementation details, together with unit tests (executed on PC rather than on the embedded system). It is based on the [Arduino Pro Mini](https://www.arduino.cc/en/Main/ArduinoBoardProMini) board.

*Please star the project if you liked it* :)

## Contact

For any information regarding this project, including:

- schematics
- components list 
- implementation details
- printed circuit board (or `PCB`)
 
just send me an e-mail to `mauriciojostx@gmail.com`. I will be glad to share it with you.

## Develop

The project makes use of [PlatformIO IoT open-source ecosystem](http://platformio.org/) for building and testing purposes.

### Environment setup

About the ecosystem: use `platformio` v3.1.0 or higher. As it comes with CLI tools, compiling and testing can be launched even without IDE.

If you really want IDE (I do): I personally use _Intellij IDEA 14_ with the _C/C++_ plugin, and it works very well. Also I have used _bluefish_ and I am pretty satisfied with it.

About the formatter: use `clang-format` tool.

#### Includes (.h) in Intellij

After compilation (see below), the file `.gcc-flags.json` contains the path of header `.h` files resolved by `platformio`.

Use the scripts provided in `xtras` to add them to your IDE.

You will have to declare the macro `INTELLIJ` in the project, so that missing libraries (like the ones usually resolved by `platformio`) are included by Intellij.

### Compile

To compile the project execute: 

```
platformio run
```

### Test

To execute the unit tests execute: 

```
bash misc/test.bash
```

### Upload

To upload the project to your embedded system connect it and launch: 

```
platformio run --target upload
```

### Monitor serial port

If you are interested in `debug` logs you will have to enable the `DEBUG` flag in [platformio.ini](platformio.ini) and upload the program. Keeping it connected launch the following to display the logs:

```
pio -f -c atom serialports monitor --port /dev/ttyUSB0 -b 115200
```

## Use

If you have already implemented a module including firmware and hardware, you can use it following the [User Guide][USER-GUIDE.md]. 

## License

Note that this project is released under the [GNU General Public License v3](https://www.gnu.org/licenses/gpl.txt). 

