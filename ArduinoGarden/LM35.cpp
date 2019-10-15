// SerialComs.cpp


#include <Arduino.h>
#include "LM35.h"
#include "pins.h"

//constant variables for conversions
const int ADC_LEVELS=1024;
const int MV_RANGE=5000;
const int MV_PRE_DEGREE=10;

//function to get temp in F

#include <LiquidCrystal.h>



LiquidCrystal lcd_lm35(12, 11, 5, 4, 3, 2);

void lcd_soiltemp()
{
  lcd_lm35.begin(16,2);
  lcd_lm35.clear();
  lcd_lm35.print("soil Temp in C ");
  lcd_lm35.setCursor(0,1);
  lcd_lm35.print("     ");
  lcd_lm35.println(gettempC(LM35_Pin));
}

float gettempF(int pin) {

  float val;
  float val_mv, tempC,tempF;

  val = analogRead(pin);
  val_mv = (val / ADC_LEVELS) * MV_RANGE;

  tempC = val_mv / MV_PRE_DEGREE ;

  tempF = ((tempC * 9) / 5) + 32;

  return tempF;

}

//function to get temp in C
float gettempC(int pin) {

  float val;
  float val_mv, tempC;

  val = analogRead(pin);
  val_mv = (val / ADC_LEVELS) * MV_RANGE;

  tempC = val_mv / MV_PRE_DEGREE ;

  return tempC;

}
