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

#include "GlobalCovfefeLearner.h"

GlobalCovfefeLearner::GlobalCovfefeLearner(IrSender *irSender_, IrReader *irReader_,
        int commandLed_, int learnLed_, int transmitLed_)
: GlobalCovfefe(irSender_, commandLed_, transmitLed_),irReader(irReader_),learnLed(learnLed_) {
    irReader->reset();
    initLed(learnLed);
}

GlobalCovfefeLearner::GlobalCovfefeLearner(const GlobalCovfefeLearner& orig) : GlobalCovfefe(orig),irReader(orig.irReader) {
}

GlobalCovfefeLearner::~GlobalCovfefeLearner() {
}

void GlobalCovfefeLearner::blink(unsigned int count, milliseconds_t ms) const {
    for (unsigned int i = 0; i < count; i++) {
        turnOnOffLed(learnLed, ms);
        GlobalCovfefe::blink(1, ms);
    }
}

void GlobalCovfefeLearner::processCommand(Stream &stream, char* buf) const {
    if (strncmp(buf, "get_IRL", 7) == 0)
        getIRL(stream);
    else
        GlobalCovfefe::processCommand(stream, buf);
}

void GlobalCovfefeLearner::getIRL(Stream &stream) const {
    stream.println(F("IR Learner Enabled"));
    turnOnLed(learnLed);

    irReader->receive();
    turnOffLed(learnLed);
    if (!irReader->isEmpty()) {
        stream.print(F("sendir,1:1,1,"));
        frequency_t freq = irReader->getFrequency();
        stream.print(freq);
        stream.print(",1,1");
        for (unsigned int i = 0; i < irReader->getDataLength(); i++) {
            stream.print(",");
            stream.print(((uint32_t) irReader->getDuration(i) * freq)/1000000UL);
        }
        stream.println();
    }
}
