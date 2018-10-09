#include <string>

unsigned long t1;
unsigned long t2;

std::string command;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
}

void loop() {

  t1 = micros();
  command = serialRead(&Serial,'\r');
  t2 = micros();
  if(!command.empty()) {
    Serial.println(t2-t1);
    Serial.println(command.c_str());
  }

}

std::string serialRead(Stream *serialX, char endChar) {
  std::string serialString;
  char buff = '\0';
  
  for(int i = 0; serialX->available() > 0 && buff != endChar; i++) {
    buff = serialX->read();
    serialString += buff;
  }

  return serialString;
}
