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

void readSpectrum() {
  Serial.println("Starting Spectrum read...");
  send_str("S");
  dataCount = 0;
  readHeader();
  word dataLine = readWord();
  while (dataLine != 0xFFFD) {
    if(escapeGlobal == 1){
      break;
    }
    spectrum[dataCount++] = dataLine; 
    dataLine = readWord();
  }
  Serial.println("End of spectrum");
  escapeGlobal = 0;
}

word readWord() {
  byte byte_buf[2];
  Serial1.readBytes(byte_buf, 2);
  return (word) byte_buf[1] | (word) byte_buf[0] << 8;
}

// this may not work
void changeBaud(const int dataword) {
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
  maximum();
  if(maxValue > 55000){
    integrationTime = integrationTime / 3;
  }
  else if(maxValue < 10000){
    integrationTime = integrationTime * 2;
  }
  if(integrationTime < 2){
    integrationTime = 1;
  }
  else if(integrationTime >= 5000){
    integrationTime = 5000;
  }
  sendCharDataword("I",integrationTime);
  delay(100);
}


