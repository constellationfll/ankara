
void sendData(int x) {
  BtSerial.write(x);
  delay(100);
}

boolean isReady() {

  if (!a) {
    BtSerial.write("a");
    delay(100);
  }
  if (Serial.available() > 0) {

    char y = BtSerial.read();
    Serial.println(y);
    if (y == 'b') {
      a = true;
    }
  }

  return a;
}