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

// this may not work
void changeBaud(int dataword) {
  switch(dataword) {
    case 0:
      sendCharDataword("K",0);
      Serial1.begin(2400);
      delay(1000);
      sendCharDataword("K",0);
      break;
    case 1:
      sendCharDataword("K",1);
      Serial1.begin(4800);
      delay(1000);
      sendCharDataword("K",1);
      break;
    case 2:
      sendCharDataword("K",2);
      Serial1.begin(9600);
      delay(1000);
      sendCharDataword("K",2);
      break;
    case 3:
      sendCharDataword("K",3);
      Serial1.begin(19200);
      sendCharDataword("K",3);
      delay(1000);
      break;
    case 4:
      sendCharDataword("K",4);
      Serial1.begin(38400);
      delay(1000);
      sendCharDataword("K",4);
      break;
    case 6:
      sendCharDataword("K",6);
      Serial1.begin(115200);
      delay(1000);
      sendCharDataword("K",6);
      break;
  }
}

void send_str(const char* cmd) {
  Serial1.print(cmd);
  Serial1.print('\r');
}

void sendCharDataword(const char* cmd, word dataword) {
  byte LSB = dataword & 0xFF;
  byte MSB = dataword >> 8;
  Serial1.print(cmd);
  Serial1.write(MSB);
  Serial1.write(LSB);
  Serial1.print('\r');
}

void setIntegrationTime() {
  integrationTime = 10;
  sendCharDataword("I",integrationTime);
  readSpectrum();
  double multiplier = 50000.0/static_cast<double>(maximum(spectrum));
  integrationTime = static_cast<double>(integrationTime)*multiplier;
  Serial.print("Multiplier: ");
  Serial.println(multiplier);
  sendCharDataword("I",integrationTime);
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

