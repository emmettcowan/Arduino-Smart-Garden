//soil mostiure .cpp
//EMMETT COWAN

#include <Arduino.h>
#include "Soilmoist.h"
#include <LiquidCrystal.h>

LiquidCrystal lcdsoil(12, 11, 5, 4, 3, 2);

void lcd_print_moist(){
  lcdsoil.begin(16,2);
  lcdsoil.clear();
  lcdsoil.print("soil moisture ");
  lcdsoil.setCursor(0,1);
  lcdsoil.print("(0-500)=");
  lcdsoil.println(readsoil());
}


int readsoil(){
  int soilpower = 8;
  int soilpin = A0;
  pinMode(soilpower, OUTPUT);
  digitalWrite(soilpower, 1);
  delay(10);
  int val = analogRead(soilpin);
  digitalWrite(soilpower , 0);
  int moistlevel;
  moistlevel = map(val , 0 ,1023,0,500);
  return moistlevel ;
  
}
