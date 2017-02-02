# ZINO README

This is a _very low-budget_ indoor **plant watering system** project. 

[![Build Status](https://api.travis-ci.org/mauriciojost/zino-arduino.svg)](https://travis-ci.org/mauriciojost/zino-arduino)

If you like it, feel free to **star the project!**

See the schematics and PCB here: 

- [ZINO v0.8 (same as v0.7)](https://easyeda.com/mauriciojost/Zino_0_7-83ae540d977c4ccca22baec4ba52e15a)

- [ZINO v0.7](https://easyeda.com/mauriciojost/Zino_0_7-83ae540d977c4ccca22baec4ba52e15a)

- [ZINO v0.6](https://easyeda.com/mauriciojost/Zino_0_6-c71b03df566040c1ab1992ca502e4733)

- [ZINO v0.5](https://easyeda.com/mauriciojost/Zino_0_5-0b7e945594b840929fab59ab73e4050d)

See the diagrams here: 

- [ZINO v0.7](https://www.draw.io/#G0B5MD-6bhZtjBaEVReC0zRE43czA)

- [ZINO v0.6](https://www.draw.io/#G0B5MD-6bhZtjBbkE3azBUQlBPZ00)

See the Youtube videos here: 

- [ZINO v0.6](https://youtu.be/Y1kThF5C82E)

- [ZINO v0.5](https://youtu.be/keEHZ9MFcKU)

## Some details

About this project:

- materials cost should be **around 38 euros** (included PCB order, power supply and components)

- a **user guide** and a **full set of features** can be found [here](USER-GUIDE.md)

- it is **operational** (it has been fully implemented at least 4 times, and it's been since then running at home, taking care of my small ficus, a couple of calas, tulipans, and aloes veras)

- hardware is based on the **[Arduino Pro Mini](https://www.arduino.cc/en/Main/ArduinoBoardProMini) board**

- **unit tests** are provided (executed on PC rather than on the embedded system)

### Costs

So far this is a brief description of the cost of the components used (at least here in Europe): 

-  3e: Arduino pro mini
-  9e: PCB by easyeda (42 euros including shipping / 5 pcbs)
-  3e: servo
-  1e: regulator L7805
-  1e: TIP120
-  7e: water pump
-  2e: LCD 16x2
-  1e: buzzer
-  8e: power supply 12v
-  5e: other components


## Develop

The project makes use of [PlatformIO IoT open-source ecosystem](http://platformio.org/) for building and testing purposes.

Also this project makes use of the libraries `main4ino` and `log4ino`.

Use _platformio v3.1.0_ or higher. As it comes with CLI tools, compiling and testing can be launched even without IDE.

Installation of _PlatformIO CLI_ is much easier and predictable when installing it together with the _Atom IDE_ ([download _deb_ file provided as _Atom for PlatformIO_ here](http://platformio.org/platformio-ide)).

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

Note that for the execution of these tests on a CI platform (such as **Travis CI**) you will need to
create an account in PlatformIO Plus by doing: 

```
platformio account register
platformio account login
platformio account token
```

Retrieve the token and use it (hidden) in your CI platform by exporting: 

```
PLATFORMIO_AUTH_TOKEN=<token>
```

To **upload** the project to your embedded system connect it and launch: 

```
platformio run --target upload
```

To get **debug logs** you will have to enable the _DEBUG_ flag in [platformio.ini](platformio.ini), compile and upload the program. Keeping it connected launch the following to display the logs:

```
pio -f -c atom serialports monitor --port /dev/ttyUSB0 -b 115200
```

Note that if problems such as: 

```
Error: Can not install 'native' with version requirements '*' for your system 'linux_x86_64'
```
the workaround I found so far is to execute these targets via the _Atom IDE_ which will correctly install dependencies and update packages.

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

