#include <math.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(1+fmod(-24.1,1.0));
  Serial.println(1-fmod(24.1,1.0));
}

void loop() {
  // put your main code here, to run repeatedly:
}
