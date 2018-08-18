# GlobalCovfefe -- Emulation of a GlobalCache networked IR device

The present program/library emulates a TCP commands of a
[GlobalCaché](https://www.globalcache.com)
networked IR sender, like the [Flex](https://www.globalcache.com/products/flex/).
The emulation is not complete. The device emulated has one IR sender,
and optionally a learner. Sensor inputs and relay closures are planned,
but not yet implemented.

Hardware requirements are a "sufficiently large" Arduino with
a supported Ethernet shield.
The program has been developed on an Arduino Mega 2560 with an Ethernet shield
from the first generation (with W5100 chip). It is believed, that, possibly after
some stripping, it should run on an Uno-class board. However, this has
not yet been verified.

The program/library is built on top of my IR library
[Infrared4Arduino](https://github.com/bengtmartensson/Infrared4Arduino).
Optionally, the [ABeacon](https://github.com/bengtmartensson/ABeacon)
library is used to advertise the device on the local network.
Other that that, only official Arduino libraries are used.

The TCP API is described
[here](https://www.globalcache.com/files/releases/flex-16/API-Flex_TCP_1.6.pdf).

The program/library was written from scratch by myself. It is licensed under the
GNU Public License version 2 or later.

## Software architecture
There is a class `GlobalCovfefe` and a derived class `GlobalCovfefeLearner`
contained in the `src` directory. These classes operates
on a [Stream](https://www.arduino.cc/reference/en/language/functions/communication/stream/).
This is typically, but not necessarily,
an [`EthernetClient`](https://www.arduino.cc/en/Reference/ClientConstructor) instance.
The "example" `GlobalCovfefe.ino` contains the "application". It controls the
Ethernet connections, and delivers it, as a `Stream` to an instance of the class `GlobalCovfefe`.

### Single-threaded operation
Most important difference from the GlobalCache devices
is that the current program is single-threaded.
That is, it accepts one command, executes it, and only when it is finished
(or times out), the program listens for new commands.
Thereby, commands like `stopir` are not meaningful, and not implemented.

#### Only one concurrent TCP session
For this reason, only one connection at a time is possible.
If one connection is active, the device will not respond to further connection
attempts until the first connection is terminated. (This makes a LED indicating
open connection () quite useful.)

## Commands implemented
* `getversion`: Returns the version of the current program, identical to the version in the library manager.
* `getdevices`: Returns the configured devices in a machine parseable  way.
* `blink`: Blinks the configured LEDs.
* `sendir`: sends a (raw) IR command. Compressed commands are not implemented ([yet](https://github.com/bengtmartensson/GlobalCovfefe/issues/7)).
* `get_IRL`: reads an IR signal and returns it (Only if a learner has been configured.)

Expected responses are produced.
Error handling and sensibility checking of the provided arguments
are essentially non-existent.

## Differences to the GlobalCaché Flex
* Only a subset of the TCP commands are implemented.
* The HTTP API, including file storage, is not supported.
* Only one sender is supported.
* Configuration is during compilation only; the GlobalCaché products are higly
configurable, for example with an embedded HTTP server.
* Only one session, single-threaded. (Thus no commands like `stopir` would be sensible.)

## Dependencies
The program/library depends on [Infrared4Arduino](https://github.com/bengtmartensson/Infrared4Arduino)
version 1.0.4 or later,
and (optionally) on [ABeacon](https://github.com/bengtmartensson/ABeacon).

## Configuration
The GlobalCache hardware is extensively configurable, both using the TCP commands,
as well as using the built-in HTTP-server. This program is instead configured
during compilation, using CPP defines in the file `config.h`. These are described in that file.

For the configuration of the IrSender and, optionally, the learner, see the
documenation of Infrared4Arduino.

## Modules

GlobalCache devices advertise their configuration with the `getdevices` command.
This is used here too.

### IR module
An IR module is present as module 1. It has one port.

### IR module as sensor

Sensor input is [planned but not yet implemented](https://github.com/bengtmartensson/GlobalCovfefe/issues/3).

### Relay module

Relay module is [planned but not yet implemented](https://github.com/bengtmartensson/GlobalCovfefe/issues/4).

### Serial module
Due to the single-threaded architecture, implementing
a serial module, as in the original hardware, is not possible.

## Porting
GlobalCovfefe as such is a portable program, not a program that needs porting.
There are no other "dependencies" than a certain resource consumption.
However, Infrared4Arduino (which is the low-level library that this one builds upon)
only runs
on some hardware. Porting it to more hardware platforms is desirable and planned.
Help is solicited.

## API documentation
API documentation for the classes in the library are generated by
[Doxygen](http://www.doxygen.org). This available online [here](https://bengtmartensson.github.io/GlobalCovfefe/).
The "application" in the example is not documented in this way.

## Use with IrScrutinizer
Unfortunately, the restriction of only one session can cause somewhat unexpected
behavior in [IrScrutinizer](https://github.com/bengtmartensson/harctoolbox).
If opened for sending (using the "Sending hw" pane), the connection has to be
closed before opening for capturing (the "Capturing hw" pane) --
and similarly for going from capturing to transmitting.

## Support and feedback
Bug reports, improvements requests etc. are solicited and can be directed to
[GitHub issues](https://github.com/bengtmartensson/GlobalCovfefe/issues).
General discussion etc can be directed to the
[Arduino Home Automation section and Networked Objects](https://forum.arduino.cc/index.php?board=16.0)
forum.