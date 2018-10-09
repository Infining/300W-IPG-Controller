String inputString = "";  // a string to hold incoming data

void setup() {
  
  Serial.begin(115200);   // Serial console debugger
  Serial1.begin(115200);  // PIN 0->RX and 1->TX
  Serial2.begin(115200);  // PIN 2->RX and 3->TX
  
}

void loop() {
  
  // put your main code here, to run repeatedly:
  Serial.println("Transmitting from Serial1 to Serial2");
  Serial1.println("Intel"); // This will tranmitt using PIN1
  inputString = "";

  delay(5); // delay to ensure message delivery

  // Serial2 will wait for something
  while (Serial2.available()) {
    // get the new byte
    char inChar = (char)Serial2.read(); // receiving by Serial2 on pin 2

    // add it to the inputString
    inputString += inChar;
  }

  // Serial2 receive the word "Intel"
  // Let's send the word Galileo back to Serial1

  Serial.print("Message received from Serial1: ");
  Serial.println(inputString);
  inputString = "";

  // transmitting another word to Serial2
  Serial.println("Transmitting from Serial2 to Serial1");
  Serial2.println("Galileo"); // transmitting by Serial2 using pin 3

  delay(5); // delay to ensure message delivery

  // Serial1 will wait for something
  while (Serial1.available()) {
    // get the new byte
    char inChar = (char)Serial1.read(); // receiving by Serial1 using pin 0

    // add it to the inputString
    inputString += inChar;
  }

  Serial.print("Message received from Serial2: ");
  Serial.println(inputString);
  inputString = "";

  Serial.println("-------------------------------------");

  delay(2000);
  
}
