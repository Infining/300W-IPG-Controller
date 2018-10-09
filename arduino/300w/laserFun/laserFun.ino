#include <math.h>

unsigned long bootTime;
unsigned long currentTime;

void setup() {
  // put your setup code here, to run once:
  bootTime = micros();
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = micros() - bootTime;
  Serial.print(0); // min y plot
  Serial.print(" ");
  Serial.print(110); // max y plot
  Serial.print(" ");
  Serial.println(laserFun(5.0, // delaySec
                          1.0, // riseTSec
                          2.0, // onDurSec
                          3.0, // fallTSec
                          4.0, // offDurSec
                          10.0, // minP
                          100.0, // maxP
                          0.0,  // phaseShiftSec
                          currentTime, // time in microseconds
                          -1.0 // endTSec
                          ));
  delay(1);
}

unsigned long secondsToMicros(double seconds) {
  unsigned long microS = seconds * 1.0e6;
  return microS;
}

double laserFun(double delaySec, double riseTSec, double onDurSec, double fallTSec, double offDurSec, double minP, double maxP, double phaseShiftSec, unsigned long t, double endTSec) {
  
  double val = 0.0; // output value

  signed long tnew = t;

  double periodSec = riseTSec + onDurSec + fallTSec + offDurSec; // period of signal
  double r = fmod(phaseShiftSec / periodSec, 1.0); // remainder of shift accounting for period

  tnew += secondsToMicros(r*periodSec); // add shift to time
  tnew -= secondsToMicros(delaySec); // add delay to time

  if (t >= secondsToMicros(endTSec) && endTSec >= 0) {
    val = 0.0;
  } else {
    if (t < secondsToMicros(delaySec)) { // laser hasn't turned on yet
      val = 0.0;
    } else {

      tnew = tnew % secondsToMicros(periodSec); // account for the periodic nature of the function
      
      if (tnew < secondsToMicros(riseTSec)) {
        // power rising
        val = (((maxP-minP) / secondsToMicros(riseTSec)) * tnew) + minP;
      } else if (tnew >= secondsToMicros(riseTSec) && tnew < secondsToMicros(riseTSec + onDurSec)) {
        // max power
        val = maxP;
      } else if (tnew >= secondsToMicros(riseTSec + onDurSec) && tnew < secondsToMicros(riseTSec + onDurSec + fallTSec)) {
        // power falling
        val = (((minP - maxP) / secondsToMicros(fallTSec)) * (tnew - secondsToMicros(riseTSec + onDurSec))) + maxP;
      } else if (tnew >= secondsToMicros(riseTSec + onDurSec + fallTSec) && tnew < secondsToMicros(riseTSec + onDurSec + fallTSec + offDurSec)) {
        // minumum power
        val = minP;
      }
    }
  }

  return val;
}
