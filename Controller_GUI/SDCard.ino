void startSDCard(){
  //Start the SD card
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_SPECTRO_CS)) {
    Serial.println("Initialization failed!");
  }
  else {
    Serial.println("Initialization success");
  }
  root = SD.open("/");
}
double parseFromFile(File& file) {
    char pixelText[10];
    int i = 0;
    
    while (file.available()){
      char c = file.read();
      if (isDigit(c) || c == '.'){
        pixelText[i++] = c;
      }
      else if (c == ','){
        break;
      }
    }
    pixelText[i] = '\0';
    return atof(pixelText);
}
void readRawFile() {
  File file = SD.open("GH2count.TXT", FILE_READ);
  int counter = 0;
  while (file.available()) {
    char c = file.read();
    Serial.print(c);
    counter++;
    if(counter > 500){
      break;
    }
  }
}
void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
word parseWordFromFile(File& file) {
    char pixelText[10];
    int i = 0;
    
    while (file.available()){
      char c = file.read();
      if (isDigit(c) || c == '.'){
        pixelText[i++] = c;
      }
      else if (c == ','){
        break;
      }
    }
    pixelText[i] = '\0';
    return atof(pixelText);
}
void compress(int number) {
  if(number >= 36){
    compressor = '&';
  }
  else{
    char replacements[40] = "0123456789abcdefghijklmnopqrstuvwxyz";
    compressor = replacements[number];
  }
}
void compressSecond(int number) {
  if (number & 0x01) {
    number = number + 1;
  }
  number = number/2;
  if(number >= 31){
    compressor = '&';
  }
  else{
    char replacements[40] = "0123456789abcdefghijklmnopqrstuvwxyz";
    compressor = replacements[number];
  }
}
void setFileName() {
  memset(fileName, 0, sizeof fileName);  
  DateTime dt = rtc.now();  
  char Year[5];
  itoa(dt.year(), Year, 10);
  int Month = dt.month();
  int Day = dt.day();  
  int Hour = dt.hour();
  char Minute[3];
  if(dt.minute() <=9){
    itoa(dt.minute(), Minute, 10);
    Minute[1] = Minute[0];
    Minute[0] = '0';
  }
  else{
    itoa(dt.minute(), Minute, 10);
  }
  int Second = dt.second();
  compress(Day);
  fileName[0] = compressor;    
  compress(Month);
  fileName[1] = compressor;
  fileName[2] = Year[2];
  fileName[3] = Year[3];  
  compress(Hour); 
  fileName[4] = compressor;
  fileName[5] = Minute[0];
  fileName[6] = Minute[1];
  compressSecond(Second);
  fileName[7] = compressor;
  fileName[8] = '.';
  fileName[9] = 't';
  fileName[10] = 'x';
  fileName[11] = 't';
  sprintf(measurementTime, "%02d-%02d-%4d %02d:%02d:%02d", dt.month(), dt.day(), dt.year(), dt.hour(), dt.minute(), dt.second());
}
void setBackgroundFileName() {
  memset(backFileName, 0, sizeof backFileName);  
  DateTime dt = rtc.now();  
  char Year[5];
  itoa(dt.year(), Year, 10);
  int Month = dt.month();
  int Day = dt.day();  
  int Hour = dt.hour();
  char Minute[3];
  if(dt.minute() <=9){
    itoa(dt.minute(), Minute, 10);
    Minute[1] = Minute[0];
    Minute[0] = '0';
  }
  else{
    itoa(dt.minute(), Minute, 10);
  }
  int Second = dt.second();
  compress(Day);
  backFileName[0] = compressor;    
  compress(Month);
  backFileName[1] = compressor;
  backFileName[2] = Year[2];
  backFileName[3] = Year[3];  
  compress(Hour); 
  backFileName[4] = compressor;
  backFileName[5] = Minute[0];
  backFileName[6] = Minute[1];
  compressSecond(Second);
  backFileName[7] = compressor;
  backFileName[8] = '.';
  backFileName[9] = 't';
  backFileName[10] = 'x';
  backFileName[11] = 't';
  sprintf(backMeasurementTime, "M%02d-D%02d-Y%4d H%02d:M%02d:S%02d", dt.month(), dt.day(), dt.year(), dt.hour(), dt.minute(), dt.second());
}
void spectrumAnalysis(File& file) {
  File spectrumFile = SD.open(fileName, FILE_READ);    // get integration time from spectrum file
  File backgroundFile = SD.open("backG.TXT", FILE_READ);

  
}
void saveRawSpectrum() {
  Serial.println("Writing spectrum to file...");
  File file = SD.open(fileName, FILE_WRITE);
  file.println(fileName);
  file.println();
  file.println("Integration Time: ");
  file.println(integrationTime);
  file.println();

//((((((((((((((()))))))))))))))

  for (int i = 0; i < dataCount; i++) {
    file.print(spectrum[i]);
    file.print(",");
    file.println();
  }
  file.close();
  Serial.println("Raw spectrum measurement done.");
}
void saveSpectrum(){
    Serial.println("Writing spectrum to file...");
    setFileName();
    File file = SD.open(fileName, FILE_WRITE);
    for (int i = 0; i < dataCount; i++) {
      file.print(spectrumPixelPower[i]);
      file.print(",");
      file.println();
  }
  file.close();
  Serial.println("Raw spectrum measurement done.");
}
void saveBackgroundSpectrum() {
  Serial.println("Writing background spectrum to file...");
  setBackgroundFileName();
  File file = SD.open(backFileName, FILE_WRITE);
  for (int i = 0; i < dataCount; i++) {
    file.print(backSpectrum[i]);
    file.print(",");
    file.println();
  }
  file.close();
  Serial.println("Raw spectrum measurement done.");
}
