// Pins.h

#ifndef PINS_H
#define PINS_H
#define DHT11_PIN 9
#define buttonPin 10

#include <dht.h>

#include <Arduino.h>


// Pin constants
const int TEMP_PIN(A0);
const int LDR_pin(A1);
const int LM35_Pin(A2);
const int Pump_Pin(13);


void PinsInit();

#endif
