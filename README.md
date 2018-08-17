# GlobalCovfefe

The program has been developed on an Arduino Mega 2560 with an Ethernet shield
from the first generation (with W5100 chip). It is believed, that, possibly after
some stripping, it should run on an Uno-class board. However, this has
not been tested.

## Software architecture
There is a few classes, (GlobalCovfefe and a few derived classes)
contained in the `src` directory. These classes operates
on a `Stream` (in the Arduino sense). This is typically, but not necessarily,
an `EthernetClient` instance.

The "example" `GlobalCovfefe.ino` contains the "application". It controls the
Ethernet connections, and delivers it, as a `Stream` to the class `GlobalCovfefe`.

Most important difference from the GlobalCache devices
is that the current program is single-threaded.
That is, it accepts one command, executes it, and only when it is finished
(or times out), the program listens for new commands.
Thereby, commands like `stopir` are not meaningful, and not implemented.

For this reason, only one connection at a time is possible.

## Dependencies
GlobalCovfefe depends on [Infrared4Arduino](https://github.com/bengtmartensson/Infrared4Arduino)
version 1.0.4 or later,
and (optionally) on [ABeacon](https://github.com/bengtmartensson/ABeacon).


## Configuration
The GlobalCache hardware is extensively configurable, both using the TCP commands,
as well as using the built-in HTTP-server. This program is instead configured
with CPP defines in the file `config.h`.

### IR module

### IR module as sensor

### Relay module

### Serial module
Due to the single-threaded architecture, implementing
an emulation of a serial module, as in the original hardware, is not possible.

## Porting
GlobalCovfefe as such is a portable program, not a program that needs porting.
There are no other "dependencies" than "at least *X* of *Y*".
However, Infrared4Arduino only runs
on some hardware. Porting it to more hardware platforms is desirable and planned.

## API documentation

## Use with IrScrutinizer
