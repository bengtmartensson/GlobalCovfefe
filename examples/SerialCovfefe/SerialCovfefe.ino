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

// This sketch demonstrates the GlobalCovfefe or GlobalCofefeLearner
// classes, using the Sereial stream instead of Ethernet.
#include "config.h"

#if defined(CAPTURE) || defined(RECEIVE)
#include <GlobalCovfefeLearner.h>
#else
#include <GlobalCovfefe.h>
#endif

#include <IrSenderPwm.h>

#ifdef CAPTURE
#ifdef RECEIVE
#error Only one of RECEIVE and CAPTURE may be defined
#endif
#include <IrWidgetAggregating.h>
#endif // CAPTURE

#ifdef RECEIVE
#include <IrReceiverSampler.h>
#endif

static const int commandLed =
#ifdef COMMAND_LED
        COMMAND_LED;
#else
        -1;
#endif

static const int learnLed =
#ifdef LEARN_LED
        LEARN_LED;
#else
        -1;
#endif

static const int transmitLed =
#ifdef TRANSMIT_LED
        TRANSMIT_LED;
#else
        -1;
#endif

#ifndef PROGNAME
#define PROGNAME "SerialCovfefe"
#endif

GlobalCovfefe *globalCovfefe;

void loop() {
    globalCovfefe->readProcessCommand(Serial);
}

void setup() {
    IrSender *irSender = IrSenderPwm::getInstance(true);

#if defined(CAPTURE) || defined(RECEIVE)
    IrReader *irReader =
#ifdef CAPTURE
            IrWidgetAggregating::newIrWidgetAggregating(CAPTURESIZE,
#else
            IrReceiverSampler::newIrReceiverSampler(CAPTURESIZE, RECEIVE_PIN,
#endif
            PULLUP, MARK_EXCESS, BEGINTIMEOUT, ENDINGTIMEOUT);

    globalCovfefe = new GlobalCovfefeLearner(irSender, irReader, commandLed, learnLed, transmitLed);
#else
    globalCovfefe = new GlobalCovfefe(irSender, commandLed, transmitLed);
#endif

    Serial.begin(serialBaud);
#if defined(ARDUINO_AVR_LEONARDO) | defined(ARDUINO_AVR_MICRO)
    while (!Serial)
        ; // wait for serial port to connect. "Needed for Leonardo only"
#endif

    Serial.println(F(PROGNAME));
    Serial.setTimeout(serialTimeout);
#ifdef CAPTURE
    Serial.println(F("Non-demodulating learner"));
#elif defined(RECEIVE)
    Serial.println(F("Demodulating learner"));
#else
    Serial.println(F("No learning device"));
#endif

    globalCovfefe->getversion(Serial);
    globalCovfefe->getdevices(Serial);
    globalCovfefe->blink();
}