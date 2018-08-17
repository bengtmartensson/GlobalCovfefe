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

#include "config.h"

#ifdef CAPTURE
#include <GlobalCovfefeCapturer.h>
#else
#include <GlobalCovfefe.h>
#endif

#if defined(ETHERNET_REVISION) & (ETHERNET_REVISION == 2)
#include <Ethernet2.h>
#else
#include <Ethernet.h>
#endif

#include <IPAddress.h>

#include <IrSenderPwm.h>

#ifdef CAPTURE
#ifdef RECEIVE
#error Only one of RECEIVE and CAPTURE may be defined
#endif
#include <IrWidgetAggregating.h>
#endif // CAPTURE

#ifdef RECEIVE
#error RECEIVE is not yet implemented.
#include <IrReceiverSampler.h>
#endif

#ifdef BEACON
#include <Beacon.h>
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

EthernetServer server(PORT);
EthernetClient client;

#ifndef PROGNAME
#define PROGNAME "GlobalCovfefe"
#endif

GlobalCovfefe *globalCovfefe;

static void tcpSession() {
    client.setTimeout(10000);
#ifdef SERIAL_DEBUG
    Serial.println(F("Connection!"));
#endif

    while (client.connected()) {
#ifdef BEACON
        Beacon::checkSend();
#endif
        globalCovfefe->readProcessCommand(client);
    }

#ifdef SERIAL_DEBUG
    Serial.println(F("Connection closed!"));
#endif
    client.stop();
}

void loop() {
#ifdef BEACON
    Beacon::checkSend();
#endif

    client = server.available();
    if (client)
        tcpSession();
}

void setup() {
    IrSender *irSender = IrSenderPwm::getInstance(true);

#ifdef CAPTURE
    IrWidget *irWidget = IrWidgetAggregating::newIrWidgetAggregating(CAPTURESIZE,
            CAPTURE_PULLUP, IRSENSOR_MARK_EXCESS,
            CAPTURE_BEGINTIMEOUT, CAPTURE_ENDINGTIMEOUT);

    globalCovfefe = new GlobalCovfefeCapturer(irSender, irWidget, commandLed, learnLed, transmitLed);
#else
    globalCovfefe = new GlobalCovfefe(irSender, commandLed, transmitLed);
#endif

#ifdef SDCARD_ON_ETHERSHIELD_PIN
    // disable the SD card, as recommended in the doc
    pinMode(SDCARD_ON_ETHERSHIELD_PIN, OUTPUT);
    digitalWrite(SDCARD_ON_ETHERSHIELD_PIN, LOW);
#endif
    byte mac[] = { MACADDRESS };
#ifdef DHCP
    Ethernet.begin(mac);
#else // !DHCP
    Ethernet.begin(mac, IPAddress(IPADDRESS), IPAddress(DNSSERVER), IPAddress(GATEWAY), IPAddress(SUBNETMASK));
#endif // !DHCP

    server.begin();

#ifdef BEACON
    Beacon::setup(PROGNAME, "DE-AD-BE-EF-FE-ED", "Utility", "GlobalCache",
            "iTachFlexEthernet", GlobalCovfefe::version, "none", "http://arduino/nosuchfile.html");
#endif

#if defined(ARDUINO) & defined(SERIAL_DEBUG)
    Serial.begin(serialBaud);
#if defined(ARDUINO_AVR_LEONARDO) | defined(ARDUINO_AVR_MICRO)
    while (!Serial)
        ; // wait for serial port to connect. "Needed for Leonardo only"
#endif
    Serial.print(F(PROGNAME " "));
    Serial.println(GlobalCovfefe::version);
    Serial.setTimeout(serialTimeout);

    Serial.println(Ethernet.localIP());
#endif // defined(ARDUINO) & !defined(ETHERNET) | defined(SERIAL_DEBUG)

    globalCovfefe->blink();
}