void readHeader() {
  word header = readWord();
  while (header != 0xFFFF) {
    header = readWord();
  }
  readWord();  // data size flag
  scansAccumulated = readWord();
  integrationTime = readWord();
  readWord();  // fpga baseline MSW
  readWord();  // fpga baseline LSW
  readWord();  // pixel mode
}
word readWord() {
  byte byte_buf[2];
  Serial1.readBytes(byte_buf, 2);
  return (word) byte_buf[1] | (word) byte_buf[0] << 8;
}
byte readByte() {
  byte byte_buf[1];
  Serial1.readBytes(byte_buf, 1);
  return byte_buf[1];
}
void readSpectrum() {
  Serial.println("Starting Spectrum read...");
  send_str("S");
  dataCount = 0;
  readHeader();
  word dataLine = readWord();
  while (dataLine != 0xFFFD) {
    spectrum[dataCount++] = dataLine; 
    dataLine = readWord();
  }
  Serial.println("End of spectrum");
}
void readBackSpectrum() {
  Serial.println("Starting Spectrum read...");
  send_str("S");
  dataCount = 0;
  readHeader();
  word dataLine = readWord();
  while (dataLine != 0xFFFD) {
    backSpectrum[dataCount++] = dataLine; 
    dataLine = readWord();
  }
  Serial.println("End of spectrum");
}
void send_str(const char* cmd) {
  Serial1.print(cmd);
  Serial1.print('\r');
}
void sendChar16BitDataword(const char* cmd, word dataword) {
  byte LSB = dataword & 0xFF;
  byte MSB = dataword >> 8;
  Serial1.print(cmd);
  Serial1.write(MSB);
  Serial1.write(LSB);
  Serial1.print('\r');
}
void ASCIIMode(){
  Serial1.print("aA");
}
void binaryMode(){
  Serial1.print("bB");
}
void changeBaud(word dataword) {
  byte LSB = dataword & 0xFF;
  byte MSB = dataword >> 8;

  Serial1.print("K");
  Serial1.write(MSB);
  Serial1.write(LSB);
  Serial1.print('\r');

  while (true) {
    byte resp = Serial1.read();
    if (resp == 0x6) {
      Serial.println("ACK received, confirming");
      break;
    } else if (resp == 0x21) {
      Serial.println("NAK received");
      return;
    }
  }

  // byte resp_buf[20];
  // int size = Serial1.readBytesUntil(0x6, resp_buf, 20);
  // if (size == 0) {
  //   Serial.println("No data received");
  //   return;
  // }
  // if (resp_buf[size - 1] != 0x6) {
  //   Serial.println("ACK not found:");
  //   for (int i = 0; i < size; i++) {
  //     Serial.print("resp line: ");
  //     Serial.println(resp_buf[i]);
  //   }
  //   return;
  // }

  Serial1.begin(115200);
  delay(100);

  Serial.println("After delay");
  
  while (Serial1.available()) {
    Serial.println("flushing the pipe");
    Serial.println(Serial1.read());
  }

  Serial1.print("K");
  Serial1.write(MSB);
  Serial1.write(LSB);
  Serial1.print('\r');

  while (Serial1.available()) {
    Serial.println("flushing the pipe");
    Serial.println(Serial1.read());
  }

  // while (true) {
  //   byte resp = Serial1.read();
  //   if (resp == 0x6) {
  //     Serial.println("Baud rate changed");
  //     break;
  //   } else if (resp == 0x21) {
  //     Serial.println("Confirmation failed");
  //     Serial1.begin(9600);
  //     return;
  //   }
  // }
  Serial.println("After while");
}

void setIntegrationTime() {
  integrationTime = 100;
  sendChar16BitDataword("I",integrationTime);
  readBackSpectrum();
  double multiplier = 50000.0/static_cast<double>(maximum(spectrum));
  integrationTime = static_cast<double>(integrationTime)*multiplier;
  Serial.print("Background multiplier: ");
  Serial.println(multiplier);
  sendChar16BitDataword("I",integrationTime);
}
void setBackgroundIntegrationTime() {
  backgroundIntegrationTime = 100;
  sendChar16BitDataword("I",backgroundIntegrationTime);
  readBackSpectrum();
  double multiplier = 50000.0/static_cast<double>(maximum(backSpectrum));
  backgroundIntegrationTime = static_cast<double>(backgroundIntegrationTime)*multiplier;
  Serial.print("Multiplier: ");
  Serial.println(multiplier);
  sendChar16BitDataword("I",backgroundIntegrationTime);
}
word maximum(word spectrum[]){
  word maxValue = 0;
  for (int i = 0; i < dataCount; i++) {
    if(spectrum[i] > maxValue){
      maxValue = spectrum[i];
    }
  }
  return maxValue;
}
double pixelPower(double pixelNumber, double backgroundNumber, double integrationTimeInSec, int i) {
    double pixelPower = pixelNumber - backgroundNumber;
    pixelPower *= calibrationFactorsHard[i];
    pixelPower /= 0.119460; 
    pixelPower /= integrationTimeInSec;
    pixelPower /= wavelengthDeltaHard[i];
    return pixelPower;
}
void calWattsfromCounts() {
  Serial.print("Start:\t");
  Serial.println(millis());
  double integrationTimeInSec = static_cast<double>(integrationTime);
  integrationTimeInSec = integrationTimeInSec/1000;
  int i=56;
  for(i; i < 602; i++){
    double pixelNumber = static_cast<double>(spectrum[i]);
    double backgroundNumber = static_cast<double>(backSpectrum[i]);
    spectrumPixelPower[i] = pixelPower(pixelNumber, backgroundNumber, integrationTimeInSec, i);
    uvPower += spectrumPixelPower[i];
  }
  for(i; i < 887; i++){
    double pixelNumber = static_cast<double>(spectrum[i]);
    double backgroundNumber = static_cast<double>(backSpectrum[i]);
    spectrumPixelPower[i] = pixelPower(pixelNumber, backgroundNumber, integrationTimeInSec, i);
    bluePower += spectrumPixelPower[i];
  }
  for(i; i < 1182; i++){
    double pixelNumber = static_cast<double>(spectrum[i]);
    double backgroundNumber = static_cast<double>(backSpectrum[i]);
    spectrumPixelPower[i] = pixelPower(pixelNumber, backgroundNumber, integrationTimeInSec, i);
    greenPower += spectrumPixelPower[i];
  }
  for(i; i < 1488; i++){
    double pixelNumber = static_cast<double>(spectrum[i]);
    double backgroundNumber = static_cast<double>(backSpectrum[i]);
    spectrumPixelPower[i] = pixelPower(pixelNumber, backgroundNumber, integrationTimeInSec, i);
    redPower += spectrumPixelPower[i];
  }
  Serial.print("End:\t");
  Serial.println(millis());
}