// Configuration options for SerialCovfefe

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

#define serialBaud 115200
#define serialTimeout 10000L
