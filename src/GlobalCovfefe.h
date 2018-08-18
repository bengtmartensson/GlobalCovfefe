/*
Copyright (C) 2018 Bengt Martensson.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.
*/

#pragma once

#include <IrSender.h>

/**
 * This class emulates the IR sending of a GlobalCache device. It does not
 * know about Ethernet, only reads from (and writes to) a Stream,
 * given as argument to a few of the member functions.
 */
class GlobalCovfefe {
public:
    /**
     * Main constructor.
     * @param irSender Pointer to an IrSender from Infrared4Arduino.
     * @param commandLed Pin number of command LED, if present.
     * @param transmitLed Pin number of transmit LED, if present.
     */
    GlobalCovfefe(IrSender *irSender, int commandLed = invalidPin, int transmitLed = invalidPin);

    /**
     * Copy constructor
     * @param orig
     */
    GlobalCovfefe(const GlobalCovfefe& orig);
    virtual ~GlobalCovfefe();

    /**
     * Reads and processes one command from the supplied Stream argument.
     * @param stream where the output is written.
     */
    void readProcessCommand(Stream &stream) const;

    /**
     * Implements the getdevices command.
     * @param stream where the output is written.
     */
    void getdevices(Stream &stream) const;

    /**
     * Implements the getversion command.
     * @param stream where the output is written.
     */
    void getversion(Stream &stream) const;

    /**
     * Implements the sendir command. The command line, by writing on the supplied argument.
     * @param stream where the output (completeir) is written.
     * @param buf Supposed to hold the complete command line. Is destroyed, thereby not const.
     */
    void sendir(Stream &stream, char *buf) const;

    /**
     * Implements the blink command (found in GC-100)
     * @param count Number of times to flash.
     * @param delay Delay in milli seconds
     */
    virtual void blink(unsigned int count = blinkCount, milliseconds_t delay = blinkDelay) const;

    /**
     * Version of the present program.
     */
    static const char *version;

    /**
     * The GlobalCache standard TCP command port.
     */
    static const uint16_t tcpPort = 4998U;

    /**
     * GlobalCache uses CR as line terminator. Period.
     */
    static const char eolChar = '\r';

    static const int invalidPin = -1;

    /**
     * Max length on input line.
     */
    static const size_t bufSize = 300U;

    static const unsigned int blinkCount = 3;

    static const milliseconds_t blinkDelay = 200;

private:
    IrSender *irSender;
    int commandLed;
    int transmitLed;

protected:
    /**
     * Processes one command from the supplied Stream argument.
     * To be overridden in derived classes that are implementing more commands.
     * @param stream where possible output is written.
     * @param buf Command line, to be interpreted.
     */
    virtual void processCommand(Stream &stream, char *buf) const;

    void initLed(int pin) const;
    void turnOnLed(int pin) const;
    void turnOffLed(int pin) const;

    /**
     * Turns on the pin as the first argument, waits delay milli seconds, then turns off the pin.
     * @param pin
     * @param delay
     */
    void turnOnOffLed(int pin, milliseconds_t delay) const;
};
