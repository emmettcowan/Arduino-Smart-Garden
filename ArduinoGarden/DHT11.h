//DHT11.h
//EMMETT COWAN

#ifndef DHT11_H
#define DHT11_H

#include <dht.h>
#include <Arduino.h>


void lcd_Print_Dht();
int returnDHT11Temp();
int returnDHT11Humidity();

#endif
