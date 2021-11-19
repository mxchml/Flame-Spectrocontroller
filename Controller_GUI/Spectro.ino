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

void readHeader() {
  word header = readWord();
  unsigned long escape_start = millis();
  unsigned long escape_now;
  
  while (header != 0xFFFF) {
    header = readWord();
    
    escape_now = millis();
    if ((escape_now - escape_start) > 15000) {
      myTFT.background(0,0,0);
      myTFT.fill(255,255,255);
      myTFT.stroke(255,255,255);
      myTFT.text("Spectroradiometer",14,45);
      myTFT.text("response timeout", 14, 55);

      exit(3);
    }

  }
  word data_size_flag = readWord();  // data size flag
  scans_accumulated = readWord();
  integration_time = readWord();
  word FPGA_MSW = readWord();  // fpga baseline MSW
  word FPGA_LSW = readWord();  // fpga baseline LSW
  word pixel_mode = readWord();  // pixel mode
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
  data_count = 0;
  readHeader();
  word data_line = readWord();
  for (int escape = 0; data_line != 0xFFFD && escape < 2100; escape++) {
    spectrum[data_count++] = data_line; 
    data_line = readWord();
  }
  Serial.println("End of spectrum");

  //correction for stuck pixels
  spectrum[310] = (spectrum[309] + spectrum[311]) / 2;
  spectrum[345] = (spectrum[344] + spectrum[346]) / 2;
  spectrum[1011] = (spectrum[1010] + spectrum[1012]) / 2;  
}

void send_str(const char* cmd) {
  Serial1.print(cmd);
  Serial1.print('\r');
}

void sendChar16BitDataword(const char* cmd, word data_word) {
  byte LSB = data_word & 0xFF;
  byte MSB = data_word >> 8;
  
  Serial1.print(cmd);
  Serial1.write(MSB);
  Serial1.write(LSB);
  Serial1.print('\r');
}

void sendChar32BitDataword(const char* cmd, long data_word) {
  byte smallest = data_word & 0xFF;
  byte mid_small = data_word >> 8;
  byte mid_large = data_word >> 16;
  byte largest = data_word >> 32;

  Serial1.print(cmd);
  Serial1.write(largest);
  Serial1.write(mid_large);
  Serial1.write(mid_small);
  Serial1.write(smallest);
  Serial1.print('\r');
}

void ASCIIMode(){
  Serial1.print("aA");
}

void binaryMode(){
  Serial1.print("bB");
}

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

void changeBaud() {
  word data_word = 4;
  byte LSB = data_word & 0xFF;
  byte MSB = data_word >> 8;

  Serial.print("Changing baud rate: ");
  Serial.println(data_word);

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

void setIntegrationTime() {
  if(spectrum[889] == 0){
    sendChar16BitDataword("I",integration_time);
    readSpectrum();
  }
  
  double multiplier = 21363.6/static_cast<double>(maximum(spectrum));
  integration_time = static_cast<double>(integration_time)*multiplier;
  
  if (integration_time > 5000) {
    integration_time = 5000;
  }
  
  sendChar16BitDataword("I",integration_time);
  
  Serial.print("IntegrationTime: ");
  Serial.println(integration_time);
}

word maximum(word spectrum[]){
  word max_value = 0;
  
  for (int i = 0; i < data_count; i++) {
    
    if(spectrum[i] > max_value){
      max_value = spectrum[i];
    }
  }
  return max_value;
}

double linearityCorrection() {
  int i = 0;
  for(i; i < 2048; i++) {
    double pixel_count = static_cast<double>(spectrum[i]);  
    pixel_count = (65535.0 / 28000.0) * pixel_count;
    double factor = linearityCorrectionFactor(pixel_count);
    pixel_count = pixel_count * factor;
    corrected_spectrum[i] = static_cast<word>(pixel_count);
    if (i % 500 == 0) {
      Serial.println(corrected_spectrum[i]);
    }
  }
}

double linearityCorrectionFactor(double pixel_count) { 
    double pixelCount1 = pixel_count;
    double pixelCount2 = pixel_count * pixel_count;
    double pixelCount3 = pixel_count * pixel_count * pixel_count;
    double pixelCount4 = pixel_count * pixel_count * pixel_count * pixel_count;
    double pixelCount5 = pixel_count * pixel_count * pixel_count * pixel_count * pixel_count;
    double pixelCount6 = pixel_count * pixel_count * pixel_count * pixel_count * pixel_count * pixel_count;
    double pixelCount7 = pixel_count * pixel_count * pixel_count * pixel_count * pixel_count * pixel_count * pixel_count;
    double factor = 0.884115 + 1.00742e-5 * pixelCount1 - 5.39901e-10 * pixelCount2 + 2.78301e-14 * pixelCount3 - 1.10510e-18 * pixelCount4 + 2.59529e-23 * pixelCount5 - 3.05861e-28 * pixelCount6 + 1.34544e-33 * pixelCount7;
    factor = 1.0*factor;
    return factor;
}
