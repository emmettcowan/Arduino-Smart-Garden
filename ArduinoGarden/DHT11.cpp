//dht11.cpp
//EMMETT COWAN

#include <Arduino.h>
#include "DHT11.h"
#include "Pins.h"
#include <LiquidCrystal.h>


dht DHT;
LiquidCrystal lcd_dht11(12, 11, 5, 4, 3, 2);

void lcd_Print_Dht(){
  lcd_dht11.begin(16,2);
  DHT.read11(DHT11_PIN);
  lcd_dht11.clear();
  lcd_dht11.print("Air temp= ");
  lcd_dht11.print(DHT.temperature);
  lcd_dht11.print("C");
  lcd_dht11.setCursor(0,1);
  lcd_dht11.print("humidity =");
  lcd_dht11.print(DHT.humidity);
  lcd_dht11.print("%");
}

int returnDHT11Temp(){
  DHT.read11(DHT11_PIN);
  int temp=DHT.temperature;
  return temp;
}

int returnDHT11Humidity(){
  DHT.read11(DHT11_PIN);
  int hum=DHT.humidity;
  return hum;
}
