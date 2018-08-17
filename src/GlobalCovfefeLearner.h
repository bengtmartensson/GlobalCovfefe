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

#include <GlobalCovfefe.h>
#include <IrReader.h>

class GlobalCovfefeLearner : public GlobalCovfefe {
public:
    GlobalCovfefeLearner(IrSender *irSender, IrReader *irReader,
            int commandLed = invalidPin, int learnLed = invalidPin, int transmitLed = invalidPin);
    GlobalCovfefeLearner(const GlobalCovfefeLearner& orig);
    virtual ~GlobalCovfefeLearner();

    virtual void processCommand(Stream &stream, char* buf);
    virtual void blink(unsigned int count = blinkCount, milliseconds_t delay = blinkDelay);

    void getIRL(Stream &stream);

private:
    IrReader *irReader;
    int learnLed;
};
