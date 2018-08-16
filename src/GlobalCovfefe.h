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

class GlobalCovfefe {
public:
    GlobalCovfefe(IrSender *irSender, int commandLed = invalidPin, int transmitLed = invalidPin);
    GlobalCovfefe(const GlobalCovfefe& orig);
    virtual ~GlobalCovfefe();

    void readProcessCommand(Stream &stream);

    /**
     * Version of the present program.
     */
    static const char *version;

    /**
     * GlobalCache uses CR as line terminator. Period.
     */
    static const char eolChar = '\r';

    static const int invalidPin = -1;

    /**
     * Max length on input line.
     */
    static const size_t bufSize = 300U;

private:
    IrSender *irSender;
    int commandLed;
    int transmitLed;

    void getdevices(Stream &stream);
    void getversion(Stream &stream);
    void sendir(Stream &stream, char *buf);

protected:
    virtual void processCommand(Stream &stream, char *buf);
    void initLed(int pin);
    void turnOnLed(int pin);
    void turnOffLed(int pin);
};
