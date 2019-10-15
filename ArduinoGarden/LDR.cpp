//LDR .cpp
//EMMETT COWAN

#include <Arduino.h>
#include "LDR.h"
#include "pins.h"
#include <LiquidCrystal.h>

LiquidCrystal lcdLDR(12, 11, 5, 4, 3, 2);

int LDRRaw = 0;
int LDRLevel = 0;

  
void LDRLcd()
{
   LDRRaw = analogRead(LDR_pin);
   LDRLevel = map(LDRRaw, 0,1024,0,100);
   lcdLDR.begin(16,2);
   lcdLDR.clear();
   lcdLDR.print("Light level ");
   lcdLDR.setCursor(0,1);
   lcdLDR.print("(0-100)=");
   lcdLDR.println(LDRLevel);
}

int readLight()
{
  LDRRaw = analogRead(LDR_pin);
  LDRLevel = map(LDRRaw, 0,1024,0,100);
  return LDRLevel;
}
