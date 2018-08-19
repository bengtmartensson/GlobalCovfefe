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

/**
 * This class is derived from GlobalCovfefe, and adds the get_IRL command,
 * invoking a learner.
 */
class GlobalCovfefeLearner : public GlobalCovfefe {
public:
    /**
     * Main constructor.
     * @param irSender Pointer to an IrSender from Infrared4Arduino.
     * @param irReader Pointer to an IrReader from Infrared4Arduino.
     * @param commandLed Pin number of command LED, if present.
     * @param learnLed Pin number of learn LED, if present.
     * @param transmitLed Pin number of transmit LED, if present.
     */
    GlobalCovfefeLearner(IrSender *irSender, IrReader *irReader,
            int commandLed = invalidPin, int learnLed = invalidPin, int transmitLed = invalidPin);

    /**
     * Copy constructor
     */
    GlobalCovfefeLearner(const GlobalCovfefeLearner& orig);
    virtual ~GlobalCovfefeLearner();

    virtual void processCommand(Stream &stream, char* buf) const;
    virtual void blink(unsigned int count = blinkCount, milliseconds_t delay = blinkDelay) const;

    /**
     * Invoke the configured learner, and deliver the result in sendir format.
     * @param stream where the output (in sendir format) is written.
     */
    void getIRL(Stream &stream) const;

private:
    IrReader *irReader;
    int learnLed;
};
