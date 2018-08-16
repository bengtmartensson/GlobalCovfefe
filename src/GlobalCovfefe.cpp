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

#include "GlobalCovfefe.h"

/* Global Cache error codes:

 * General errors:
ERR 001 Invalid request. Command not found.
ERR 002 Bad request syntax used with a known command.
ERR 003 Invalid or missing module and/or connector address.
ERR 004 No carriage return found.
ERR 005 Command not supported by current Flex Link Port setting.
ERR 006 Settings are locked.

 * IR errors:
ERR IR001 Invalid ID value.
ERR IR002 Invalid frequency.
ERR IR003 Invalid repeat.
ERR IR004 Invalid offset.
ERR IR005 Invalid IR pulse value.
ERR IR006 Odd amount of IR pulse values (must be even).
ERR IR007 Maximum pulse pair limit exceeded.

 * Serial Errors Explanation (not presently implemented)
ERR SL001 Invalid baud rate.
ERR SL002 Invalid flow control or duplex setting.
ERR SL003 Invalid parity setting.
ERR SL004 Invalid stop bits setting.

 * Relay & Sensor Errors Explanation (not presently implemented)
ERR RS001 Invalid logical relay type.
ERR RS002 Invalid logical relay state.
ERR RS003 Unsupported operation.
ERR RS004 Logical relay disabled or not available.
ERR RS008 Invalid sensor notify port value.
ERR RS009 Invalid sensor notify timer value.
 */

const char *GlobalCovfefe::version = "0.0.1";

// Ugly
#define IRMODULE "1"
#define IRPORT "1"

GlobalCovfefe::GlobalCovfefe(IrSender *irSender_, int commandLed_, int transmitLed_)
        : irSender(irSender_), commandLed(commandLed_), transmitLed(transmitLed_) {
    irSender->mute();
    initLed(commandLed);
    initLed(transmitLed);
}

GlobalCovfefe::GlobalCovfefe(const GlobalCovfefe& orig) : irSender(orig.irSender) {
}

GlobalCovfefe::~GlobalCovfefe() {
}

void GlobalCovfefe::initLed(int pin) {
    if (pin != invalidPin)
        pinMode(pin, OUTPUT);
}

void GlobalCovfefe::turnOnLed(int pin) {
    if (pin != invalidPin)
        digitalWrite(pin, HIGH);
}

void GlobalCovfefe::turnOffLed(int pin) {
    if (pin != invalidPin)
        digitalWrite(pin, LOW);
}

void GlobalCovfefe::readProcessCommand(Stream& stream) {
    char buf[bufSize];
    turnOnLed(commandLed);
    unsigned int n = stream.readBytesUntil(eolChar, buf, bufSize);
    turnOffLed(commandLed);
    char *b = buf;
    while (!isalpha(b[0]) && n > 0) {
        b++;
        n--;
    }
    if (n < bufSize)
        b[n] = '\0';
    if (n > 0)
        processCommand(stream, b);
    stream.flush();
}

void GlobalCovfefe::processCommand(Stream &stream, char* buf) {
    if (strncmp(buf, "getdevices", 10) == 0)
        getdevices(stream);
    else if (strncmp(buf, "getversion", 10) == 0)
        getversion(stream);
    else if (strncmp(buf, "sendir", 6) == 0)
        sendir(stream, buf);
    else // Command unrecognized
        stream.println(F("ERR 001"));
}

static unsigned int noCommas(const char *buf) {
    unsigned int n = 0;
    for (const char *p = buf; *p != '\0'; p++) {
        if (*p == ',')
            n++;
    }
    return n;
}

static void grok(microseconds_t *buf, size_t length, frequency_t freq) {
    for (unsigned int i = 0; i < length; i++) {
        int duration = atoi(strtok(NULL, ","));
        buf[i] = (microseconds_t) (1000000UL * duration / freq);
    }
}

void GlobalCovfefe::sendir(Stream &stream, char *buf) {
    unsigned int commas = noCommas(buf);
    strtok(buf, ",");
    strtok(NULL, ","); // connector, ignore nor now
    uint16_t id = atoi(strtok(NULL, ","));
    frequency_t freq = atoi(strtok(NULL, ","));
    uint16_t repeat = atoi(strtok(NULL, ","));
    uint16_t offset = atoi(strtok(NULL, ","));
    size_t lengthIntro = offset - 1;
    microseconds_t intro[lengthIntro];
    grok(intro, lengthIntro, freq);
    size_t lengthRepetition = commas - offset - 4;
    microseconds_t reps[lengthRepetition];
    grok(reps, lengthRepetition, freq);

    IrSignal irSignal(intro, lengthIntro, reps, lengthRepetition, NULL, 0U, freq);
    turnOnLed(transmitLed);
    irSender->sendIrSignal(irSignal, repeat);
    turnOffLed(transmitLed);
    stream.print(F("completeir," IRMODULE ":" IRPORT ","));
    stream.println(id);
}

void GlobalCovfefe::getdevices(Stream &stream) {
    stream.println(F("device,0,0 ETHERNET"));
    if (irSender != NULL)
        stream.println(F("device," IRMODULE "," IRPORT " IR"));
    stream.println(F("endlistdevices"));
}

void GlobalCovfefe::getversion(Stream &stream) {
    stream.println(version);
}
