

#include "Arduino.h"
#include "VoltageReader.h"

VoltageReader::VoltageReader(int pin1, int pin2, int avgPts)
{
  _pin1 = pin1; _pin2 = pin2;
  _avgPts = avgPts;
}

float VoltageReader::Volts()
{
  for (_aa = 1; _aa <= _avgPts;_aa++)
  {
   _hiVal = analogRead(_pin1);  
   _loVal = analogRead(_pin2);
   VoltsLow = (_loVal)/1023.0*5.0 +VoltsLow;
   VoltsHigh = (_hiVal) / 1023.0 * 5.0 +VoltsHigh;   
   }
    VoltsLow = VoltsLow/100;
    VoltsHigh = VoltsHigh/100;
    Potential = VoltsHigh-VoltsLow;  
    return Potential;
}
  
