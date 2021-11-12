<<<<<<< Updated upstream
=======
void checkSpectrometerConnection() {
  send_str("v");
  
  word connection_check_1 = readWord();
  word connection_check_2 = readWord();

  if (connection_check_1 != 1551 && connection_check_2 != 43541) {
    Serial.println(connection_check_1);
    Serial.println(connection_check_2);

    myTFT.background(0,0,0);
    myTFT.fill(255,255,255);
    myTFT.stroke(255,255,255);
    myTFT.text("Spectroradiometer",14,45);
    myTFT.text("connection failure", 14, 55);

    exit(5);
  }

}

>>>>>>> Stashed changes
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

//******************************************************************************************************************************************************************************************************************************
word readWord() {
  byte byte_buf[2];
  Serial1.readBytes(byte_buf, 2);
  return (word) byte_buf[1] | (word) byte_buf[0] << 8;
}

//******************************************************************************************************************************************************************************************************************************
byte readByte() {
  byte byte_buf[1];
  Serial1.readBytes(byte_buf, 1);
  return byte_buf[1];
}

//******************************************************************************************************************************************************************************************************************************
void readSpectrum() {
  Serial.println("Starting Spectrum read...");
  send_str("S");
  dataCount = 0;
  readHeader();
  word dataLine = readWord();
  for (int escape = 0; dataLine != 0xFFFD && escape < 2100; escape++) {
    spectrum[dataCount++] = dataLine; 
    dataLine = readWord();
  }
  Serial.println("End of spectrum");

  //correction for stuck pixels
  spectrum[310] = (spectrum[309] + spectrum[311]) / 2;
  spectrum[345] = (spectrum[344] + spectrum[346]) / 2;
  spectrum[1011] = (spectrum[1010] + spectrum[1012]) / 2;  
}

//******************************************************************************************************************************************************************************************************************************
void send_str(const char* cmd) {
  Serial1.print(cmd);
  Serial1.print('\r');
}

//******************************************************************************************************************************************************************************************************************************
void sendChar16BitDataword(const char* cmd, word dataword) {
  byte LSB = dataword & 0xFF;
  byte MSB = dataword >> 8;
  Serial1.print(cmd);
  Serial1.write(MSB);
  Serial1.write(LSB);
  Serial1.print('\r');
}

//******************************************************************************************************************************************************************************************************************************
void ASCIIMode(){
  Serial1.print("aA");
}

//******************************************************************************************************************************************************************************************************************************
void binaryMode(){
  Serial1.print("bB");
}

//******************************************************************************************************************************************************************************************************************************
bool ack() {
  while (true) {
    byte resp = Serial1.read();
    if (resp == 0x6) {
      Serial.println("ACK received, confirming");
      return true;
    } else if (resp == 0x21) {
      Serial.println("NAK received");
      return false;
    }
  }
}

//******************************************************************************************************************************************************************************************************************************
void changeBaud() {
  word dataword = 4;
  byte LSB = dataword & 0xFF;
  byte MSB = dataword >> 8;

  Serial.print("Changing baud rate: ");
  Serial.println(dataword);

  Serial1.print("K");
  Serial1.write(MSB);
  Serial1.write(LSB);

  delay(100);

  Serial1.flush();
  Serial1.begin(38400);

  Serial1.print("K");
  Serial1.write(MSB);
  Serial1.write(LSB);
  Serial1.print('\r');
}

//******************************************************************************************************************************************************************************************************************************
void setIntegrationTime() {
  if(spectrum[889] == 0){
    sendChar16BitDataword("I",integrationTime);
    readSpectrum();
  }
  double multiplier = 21363.6/static_cast<double>(maximum(spectrum));
  integrationTime = static_cast<double>(integrationTime)*multiplier;
  if (integrationTime > 5000) {
    integrationTime = 5000;
  }
  sendChar16BitDataword("I",integrationTime);
  Serial.print("IntegrationTime: ");
  Serial.println(integrationTime);
}

//******************************************************************************************************************************************************************************************************************************
word maximum(word spectrum[]){
  word maxValue = 0;
  for (int i = 0; i < dataCount; i++) {
    if(spectrum[i] > maxValue){
      maxValue = spectrum[i];
    }
  }
  return maxValue;
}

//******************************************************************************************************************************************************************************************************************************
double linearityCorrection() {
  int i = 0;
  for(i; i < 2048; i++) {
    double pixelCount = static_cast<double>(spectrum[i]);
    double factor = linearityCorrectionFactor(pixelCount);
    pixelCount = pixelCount * factor;
    correctedSpectrum[i] = static_cast<word>(pixelCount);
  }
}

//******************************************************************************************************************************************************************************************************************************
double linearityCorrectionFactor(double pixelCount) { 
    pixelCount = pixelCount * (65535.0/28000.0);
    double pixelCount1 = pixelCount;
    double pixelCount2 = pixelCount * pixelCount;
    double pixelCount3 = pixelCount * pixelCount * pixelCount;
    double pixelCount4 = pixelCount * pixelCount * pixelCount * pixelCount;
    double pixelCount5 = pixelCount * pixelCount * pixelCount * pixelCount * pixelCount;
    double pixelCount6 = pixelCount * pixelCount * pixelCount * pixelCount * pixelCount * pixelCount;
    double pixelCount7 = pixelCount * pixelCount * pixelCount * pixelCount * pixelCount * pixelCount * pixelCount;
    double factor = 0.884115 + 1.00742e-5 * pixelCount1 + -5.39901e-10 * pixelCount2 + 2.78301e-14 * pixelCount3 + -1.10510e-18 * pixelCount4 + 2.59529e-23 * pixelCount5 + -3.05861e-28 * pixelCount6 + 1.34544e-33 * pixelCount7;
    factor = 1.0/factor;
    return factor;
}

//******************************************************************************************************************************************************************************************************************************
double pixelPower(double pixelCount, double integrationTimeInSec, int i) {
    double pixelPower = pixelCount;
    pixelPower *= calibrationFactorsHard[i];
    pixelPower /= 0.119460; 
    pixelPower /= integrationTimeInSec;
    pixelPower /= wavelengthDeltaHard[i];
    return pixelPower;
}

//******************************************************************************************************************************************************************************************************************************
void identifySignal() {
  unsigned long countSum = 0;
  int loopExecuteCount = 107;
  double thresholdLimit;
  int i=59;
  for(i; i < 166; i++) {
    countSum += correctedSpectrum[i];
  }
  double background = static_cast<double>(countSum) / static_cast<double>(loopExecuteCount);
  i = 0;  
  for(i; i < 369; i++) {
    if(abs(static_cast<double>(correctedSpectrum[i]) - background) > background/1.5) {
      correctedSpectrum[i] = correctedSpectrum[i] - background;
    }
    else {
      correctedSpectrum[i] = 0;
    }
  }
  for(i; i < 604; i++) {
    if(abs(static_cast<double>(correctedSpectrum[i]) - background) > background/2.0) {
      correctedSpectrum[i] = correctedSpectrum[i] - background;
    }
    else {
      correctedSpectrum[i] = 0;
    }
  }
  for(i; i < 2048; i++) {
    if(abs(static_cast<double>(correctedSpectrum[i]) - background) > background/10.0) {
      correctedSpectrum[i] = correctedSpectrum[i] - background;
    }
    else {
      correctedSpectrum[i] = 0;
    }
  }
}

//******************************************************************************************************************************************************************************************************************************
void calWattsfromCounts() {
  double integrationTimeInSec = static_cast<double>(integrationTime);
  integrationTimeInSec = integrationTimeInSec/1000;
  int i=59;
  for(i; i < 604; i++){
    double pixelNumber = static_cast<double>(correctedSpectrum[i]);
    spectrumPixelPower[i] = pixelPower(pixelNumber, integrationTimeInSec, i);
    uvPower += spectrumPixelPower[i];
  }
  for(i; i < 889; i++){
    double pixelNumber = static_cast<double>(correctedSpectrum[i]);
    spectrumPixelPower[i] = pixelPower(pixelNumber, integrationTimeInSec, i);
    bluePower += spectrumPixelPower[i];
  }
  for(i; i < 1183; i++){
    double pixelNumber = static_cast<double>(correctedSpectrum[i]);
    spectrumPixelPower[i] = pixelPower(pixelNumber, integrationTimeInSec, i);
    greenPower += spectrumPixelPower[i];
  }
  for(i; i < 1489; i++){
    double pixelNumber = static_cast<double>(correctedSpectrum[i]);
    spectrumPixelPower[i] = pixelPower(pixelNumber, integrationTimeInSec, i);
    redPower += spectrumPixelPower[i];
  }
  Serial.print("End:\t");
  Serial.println(millis());
}

//******************************************************************************************************************************************************************************************************************************
void calAverageWatts() {
  uvAverage = (uvAverage * sessionMeasureNumber + uvPower) / (sessionMeasureNumber+1);
  blueAverage = (blueAverage * sessionMeasureNumber + bluePower) / (sessionMeasureNumber+1);
  greenAverage = (greenAverage * sessionMeasureNumber + greenPower) / (sessionMeasureNumber+1);
  redAverage = (redAverage * sessionMeasureNumber + redPower) / (sessionMeasureNumber+1);
  sessionMeasureNumber++;
}

//******************************************************************************************************************************************************************************************************************************
void calSessionEnergy() {
  unsigned long sessionDuration = stopTime - startTime;
  sessionDuration = sessionDuration/1000;
  uvEnergy = sessionDuration * uvAverage;
  blueEnergy = sessionDuration * blueAverage;
  greenEnergy = sessionDuration * greenAverage;
  redEnergy = sessionDuration * redAverage;
}

//******************************************************************************************************************************************************************************************************************************
void resetMeasurement() {
  memset(fileName, 0, sizeof fileName);  
  memset(measurementTime, 0, sizeof measurementTime);  
  memset(spectrum, 0, sizeof spectrum);
  memset(correctedSpectrum, 0, sizeof correctedSpectrum);
  memset(spectrumPixelPower, 0, sizeof spectrumPixelPower);
  integrationTime = 10;
  uvPower = 0;
  bluePower = 0;
  greenPower = 0;
  redPower = 0;
}

//******************************************************************************************************************************************************************************************************************************
void resetSession() {
  uvAverage = 0;
  blueAverage = 0;
  greenAverage = 0;
  redAverage = 0;
  uvEnergy = 0;
  blueEnergy = 0;
  greenEnergy = 0;
  redEnergy = 0;
  sessionMeasureNumber = 0;
  memset(directoryName, 0, sizeof directoryName);
}