# ZINO README

This is a _very low-budget_ indoor **plant watering system** project. 

[![Build Status](https://api.travis-ci.org/mauriciojost/zino-arduino.svg)](https://travis-ci.org/mauriciojost/zino-arduino)

If you like it, feel free to **star the project!**

See the Youtube videos here: 

- [ZINO v0.6](https://youtu.be/Y1kThF5C82E).

- [ZINO v0.5](https://youtu.be/keEHZ9MFcKU).

## Some details

About this project:

- materials cost should be **around 25 euros**

- a **user guide** and a **full set of features** can be found [here](USER-GUIDE.md)

- it is **operational** (it has been fully implemented at least twice, and it's been since then running at home, taking care of my small ficus)

- hardware is based on the **[Arduino Pro Mini](https://www.arduino.cc/en/Main/ArduinoBoardProMini) board**

- **unit tests** are provided (executed on PC rather than on the embedded system)

## Develop

The project makes use of [PlatformIO IoT open-source ecosystem](http://platformio.org/) for building and testing purposes.

Use _platformio v3.1.0_ or higher. As it comes with CLI tools, compiling and testing can be launched even without IDE.

If you really want an IDE (I do): I personally use _Intellij IDEA 14_ with the _C/C++_ plugin, and it works very well. Also I have used _bluefish_ and I was pretty satisfied with it.

About the formatter, I recommend using _clang-format_ tool. Use [this script](misc/format.bash) for automatic formatting.

To **compile** the project execute: 

```
platformio run
```

To execute the **unit tests** (that will run on your PC) execute: 

```
bash misc/test.bash
```

To **upload** the project to your embedded system connect it and launch: 

```
platformio run --target upload
```

To get **debug logs** you will have to enable the _DEBUG_ flag in [platformio.ini](platformio.ini), compile and upload the program. Keeping it connected launch the following to display the logs:

```
pio -f -c atom serialports monitor --port /dev/ttyUSB0 -b 115200
```

### Arduino includes (.h)

After compilation (see below), the file _.gcc-flags.json_ contains the path of header _.h_ files resolved by _platformio_.

Use [this script](misc/generate-list-of-includes.bash) to add them to your IDE.

You will have to declare the macro _INTELLIJ_ in the IDE project, so that Arduino missing libraries and headers (like the ones usually resolved by _platformio_) are included by Intellij.

## Use

If you have already implemented a module including firmware and hardware, you can use it following the [User Guide](USER-GUIDE.md). 

## License

Note that this project is released under the [GNU General Public License v3](https://www.gnu.org/licenses/gpl.txt). 

## Contact

For any information regarding this project, including:

- schematics
- components list 
- implementation details
- printed circuit board (_PCB_)
 
send me an e-mail to _mauriciojostx@gmail.com_. 

I will be glad to share it!


