void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println("Ready to Write");
}

void loop() {
  Serial1.write("Test\n");
  delay(5000);
}
