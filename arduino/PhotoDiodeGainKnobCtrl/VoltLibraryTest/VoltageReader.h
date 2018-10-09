/*
VoltageReader.h - Library for reading the voltage across two analog pins.
 pin1 is assumed to be the higher voltage reading pin
pin2 is assumed to be the lower voltage pin (ie if you had a risitor 
hooked up to a battery 'pin1' would be the analog pin attached to the positive 
side of the resistor (closest to the + battery terminal) and 'pin2' would be hooked
 up to the negative side of the resistor. This library would output the voltage drop 
 of the resistor
 Created 7 May 2017
 by Chris McGahan
*/

#ifndef VoltageReader_h
#define VoltageReader_h
#include "Arduino.h"
class VoltageReader
{
  public:
  VoltageReader(int pin1, int pin2, int avgPts);

  float Volts();
  float VoltsLow;  
  float VoltsHigh;
  float Potential;   
  private:  
  int _avgPts;
  int _pin1;
  int _pin2;
  int _aa = 1;//counter
  float _hiVal;
  float _loVal;
};

#endif

