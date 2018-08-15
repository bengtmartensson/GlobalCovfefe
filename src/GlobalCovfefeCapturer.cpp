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

#include "GlobalCovfefeCapturer.h"

GlobalCovfefeCapturer::GlobalCovfefeCapturer(IrSender *irSender_, IrWidget *irWidget_) : GlobalCovfefe(irSender_),irWidget(irWidget_) {
    irWidget->reset();
}

GlobalCovfefeCapturer::GlobalCovfefeCapturer(const GlobalCovfefeCapturer& orig) : GlobalCovfefe(orig),irWidget(orig.irWidget) {
}

GlobalCovfefeCapturer::~GlobalCovfefeCapturer() {
}

void GlobalCovfefeCapturer::processCommand(Stream &stream, char* buf) {
    if (strncmp(buf, "get_IRL", 7) == 0)
        getIRL(stream);
    else
        GlobalCovfefe::processCommand(stream, buf);
}

void GlobalCovfefeCapturer::getIRL(Stream &stream) {
    stream.println(F("IR Learner Enabled"));

    irWidget->capture();
    if (!irWidget->isEmpty()) {
        stream.print(F("sendir,1:1,1,"));
        stream.print(irWidget->getFrequency());
        stream.print(",1,1");
        for (unsigned int i = 0; i < irWidget->getDataLength(); i++) {
            stream.print(",");
            stream.print(irWidget->getDuration(i));
        }
        stream.println();
    }
}
