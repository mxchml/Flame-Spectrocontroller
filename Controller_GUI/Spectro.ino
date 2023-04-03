void checkSpectrometerConnection() {    //check to see if the spectroradiometer is connected to the platform
  send_str("v");    //send a command that asks the spectroradiometer to return some known firmware data
  
  word connection_check_1 = readWord();
  word connection_check_2 = readWord();

  if (connection_check_1 != 1551 && connection_check_2 != 43541) {    //determine if the data returned by the spectroradiometer matches the expected values 

    myTFT.fillScreen(BLACK);

    myTFT.setCursor(26,90);
    myTFT.print("Spectroradiometer");

    myTFT.setCursor(26,110);
    myTFT.print("connection failure");

    exit(5);    //stop the platform if the spectroradiometer fails to communicate with the rest of the platform
  }
}
//_________________________________________________

void readHeader() {   //the first step the microcontroller performs during reciept of spectrum data
  word header = readWord();
  unsigned long escape_start = millis();
  unsigned long escape_now;
  
  while (header != 0xFFFF) {    //the microcontroller skips all data until the spectroradiometer starts sending spectrum data
    header = readWord();
    
    escape_now = millis();
    if ((escape_now - escape_start) > 15000) {    //if the spectroradiometer does not send data within 15 seconds of the microcontroller request, the platform stops and throws an error
      myTFT.fillScreen(BLACK);

      myTFT.setCursor(26,90);
      myTFT.print("Spectroradiometer");

      myTFT.setCursor(26,110);
      myTFT.print("response timeout");

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
//_________________________________________________

word readWord() {   //a function that converts the data sent by the spectroradiometer into a dataword format and stores it in the microcontroller RAM memory
  byte byte_buf[2];
  Serial1.readBytes(byte_buf, 2);
  return (word) byte_buf[1] | (word) byte_buf[0] << 8;
}
//_________________________________________________

byte readByte() {     //a function that allows the microcontroller to read a single byte sent by the spectroradiometer
  byte byte_buf[1];
  Serial1.readBytes(byte_buf, 1);
  return byte_buf[1];
}
//_________________________________________________

void readSpectrum() {   //a function that governs how the microcontroller asks the spectroradiometer for a spectrum measurement, and how to store that data on the microcontroller
  Serial.println("Starting Spectrum read...");
  send_str("S");    //command sent by the microcontroller that asks the spectroradiometer to take a spectrum and send spectrum data back
  data_count = 0;
  readHeader();   //a function that tells the microcontroller to discard all data sent by the spectroradiometer until the spectrum data begins
  word data_line = readWord();
  for (int escape = 0; data_line != 0xFFFD && escape < 2100; escape++) {    //the microcontroller captures spectrum data as bytes until the spectroradiometer sends the 0xFFFD code signalling the end of the spectrum
    spectrum[data_count++] = data_line;   //save the spectrum data to the spectrum global array
    data_line = readWord();
    }
  Serial.println("End of spectrum");

  //correction for stuck pixels
  // spectrum[310] = (spectrum[309] + spectrum[311]) / 2;
  // spectrum[345] = (spectrum[344] + spectrum[346]) / 2;
  // spectrum[1011] = (spectrum[1010] + spectrum[1012]) / 2;  

}
//_________________________________________________

void send_str(const char* cmd) {    //function that formats simple commands to the format expected by the spectroradiometer
  Serial1.print(cmd);
  Serial1.print('\r');
}
//_________________________________________________

void sendChar16BitDataword(const char* cmd, word data_word) {    //function that formats commands that contain parameters to the format expected by the spectroradiometer
  byte LSB = data_word & 0xFF;
  byte MSB = data_word >> 8;
  
  Serial1.print(cmd);
  Serial1.write(MSB);
  Serial1.write(LSB);
  Serial1.print('\r');
}
//_________________________________________________

void setIntegrationTime() {   //function that governs the dynamic integration time adjustment
  if(spectrum[889] == 0){   //if the integration time has not yet been dynamically adjusted, perform a spectrum measurement using a 100ms integration time
    sendChar16BitDataword("I",integration_time);    //set the integration time on the spectroradiometer to 100ms
    readSpectrum();   //command the spectroradiometer to take a spectrum
  }
  
  double multiplier = 21363.6/static_cast<double>(maximum(spectrum));   //use the peak signal of the most recent measurement to calculate the integration time that would allow the signal to reach 80% of spectroradiometer saturation
  integration_time = static_cast<double>(integration_time)*multiplier;
  
  if (integration_time > 5000) {    //if the calculated integration time exceeds five seconds, set the global variable containing the integration time to five seconds
    integration_time = 5000;
  }
  
  sendChar16BitDataword("I",integration_time);    //set the integration time on the spectroradiometer to five seconds
  
  Serial.print("IntegrationTime: ");
  Serial.println(integration_time);
}
//_________________________________________________

word maximum(word spectrum[]){    //identify the maximum signal measured within a single spectrum
  word max_value = 0;
  
  for (int i = 0; i < data_count; i++) {
    
    if(spectrum[i] > max_value){
      max_value = spectrum[i];
    }
  }
  return max_value;
}
//_________________________________________________

double linearityCorrection() {    //perform a linearity correction using the polynomial provided by the manufacturer of the spectroradiometer
  int i = 0;
  for(i; i < 2048; i++) {
    double pixel_count = static_cast<double>(spectrum[i]);
    pixel_count = (65535.0 / 28000.0) * pixel_count;    //normalize the signal sent by RS232 to that expected by USB (polynomial provided using USB saturation format)
    double factor = linearityCorrectionFactor(pixel_count);   //apply the polynomial to the raw signal to compute a conversion factor
    pixel_count = pixel_count * factor;   //multiply the factor to compute the linearity corrected signal
    corrected_spectrum[i] = static_cast<word>(pixel_count);   //save the corrected signal into the corrected_spectrum global array
  }
}
//_________________________________________________

double linearityCorrectionFactor(double pixel_count) {    //computation of the linearity polynomial provided by the manufacturer
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
