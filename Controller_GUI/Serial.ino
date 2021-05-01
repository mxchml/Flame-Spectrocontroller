void initializeSerial(){
  // Open regular serial connections for RX/TX and the software serial pins
  Serial.begin(9600);
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.println("The Serial Port is active");
}
