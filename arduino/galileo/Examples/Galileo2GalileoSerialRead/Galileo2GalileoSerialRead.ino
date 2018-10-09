String inData;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println("Ready to Read");
}

void loop() {
  while(Serial1.available() > 0) {
    char received = Serial1.read();
    inData += received;

    if (received == '\n')
    {
      Serial.print("String Received: ");
      Serial.println(inData);

      inData = "";
    }
  }
}
