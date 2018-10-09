
#include <VoltageReader.h>
VoltageReader V(0,3,100);
int pin = 12;
float P;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
P = V.Volts();

Serial.println(P,9);
if (0.045<abs(P) & abs(P) < 0.15 )
{
  
  digitalWrite(pin, HIGH);
}
else
{
  digitalWrite(pin, LOW);
}

delay(50);
}
