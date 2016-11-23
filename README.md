# ZINO README

This is a _very low-budget_ indoor **plant watering system** project. 

**Please star the project if you like it :)**

Some details:

- it is operational (it was fully implemented at least twice, and is right now running at home taking care of my small basil plant)

- hardware is based on the [Arduino Pro Mini](https://www.arduino.cc/en/Main/ArduinoBoardProMini) board, which costs less than 3 euros

- unit tests are provided (executed on PC rather than on the embedded system)

- hardware materials cost including for watering only 1 plant should be around 25 euros, and each additional plant increases the cost by around 6 euros

The full set of features is listed [here](USER-GUIDE.md). 

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

After compilation (see below), the file [.gcc-flags.json](.gcc-flags.json) contains the path of header _.h_ files resolved by _platformio_.

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
 
just send me an e-mail to _mauriciojostx@gmail.com_. I will be glad to share it with you!


