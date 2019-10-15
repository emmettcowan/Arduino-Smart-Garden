//Pump.cpp
//EMMETT COWAN

#include <Arduino.h>
#include "pump.h"
#include "pins.h"
#include "Soilmoist.h"
#include <LiquidCrystal.h>

LiquidCrystal lcdPump(12, 11, 5, 4, 3, 2);
uint32_t pumpTimer=0;
int pumprun=1;
int moistcalabration=100;

void waterplant(){
  if(readsoil() < moistcalabration){
  digitalWrite(Pump_Pin,1);
  delay(1500);
  digitalWrite(Pump_Pin,0);
  }
}
