// Configuration options for GlobalCovfefe

#pragma once

// Define to support learning with the get_IRN command, using non-demodulating
// receiver. Preferred over RECEIVE.
#define CAPTURE

// Define to support learning with the get_IRN command, using DEMODULATING
// receiver (TSOP- etc). Use only if a non-demoodulating receiver is
// not available.
//#define RECEIVE

// LEDs can be configured to signal the operating condition.
// Due to the single-session nature, at least
// COMMAND_LED is highly recommended,
// use pin 13 if you do not have anything else.
#define COMMAND_LED     22 //16
#define LEARN_LED       24 // 17
#define TRANSMIT_LED    32 // 30

// Issues debug messages on the serial port
#define SERIAL_DEBUG

#define SDCARD_ON_ETHERSHIELD_PIN 4 // there is a pin4 to be selected low

// Define to use DHCP instead of explicitly entering IP parameters.
// Consumes rather much memory
#define DHCP

// Invoke the AMX style beacon, as per https://github.com/bengtmartensson/ABeacon
// Disable if you want/must to save memory; otherwise you probably want this.
#define BEACON

#if defined(BEACON) && ! defined(SERIAL_DEBUG)
#error BEACON without SERIAL_DEBUG does not work; I do not know why :-/
#endif

// Hardware configuration
#define MACADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED

#ifndef DHCP
#define IPADDRESS  192,168,1,29
#define GATEWAY    192,168,1,254
#define DNSSERVER  192,168,1,1
#define SUBNETMASK 255,255,255,0
#endif // ! DHCP

// Port to take commands from, standard GC port.
#define PORT       GlobalCovfefe::tcpPort

#ifdef RECEIVE
#define RECEIVE_PIN 47
// This quantity is added to all gaps and subtracted from all marks when receiving.
#define MARK_EXCESS 50
#endif

#ifdef CAPTURE
// This quantity is added to all gaps and subtracted from all marks when capturing.
#define MARK_EXCESS -10
#endif

#define CAPTURESIZE 300U // must be even
#define BEGINTIMEOUT 10000UL // milliseconds
#define ENDINGTIMEOUT 100L // milliseconds
#define PULLUP false

#if defined(SERIAL_DEBUG)
#define serialBaud 115200
#define serialTimeout 10000L
#endif // !defined(ETHERNET) | defined(SERIAL_DEBUG)
