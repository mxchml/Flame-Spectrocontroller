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

//******************************************************************************************************************************************************************************************************************************
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

//******************************************************************************************************************************************************************************************************************************
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

//******************************************************************************************************************************************************************************************************************************
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

//******************************************************************************************************************************************************************************************************************************
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

//******************************************************************************************************************************************************************************************************************************
void compress(int number) {
  if(number >= 36){
    compressor = '&';
  }
  else{
    char replacements[40] = "0123456789abcdefghijklmnopqrstuvwxyz";
    compressor = replacements[number];
  }
}

//******************************************************************************************************************************************************************************************************************************
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

//******************************************************************************************************************************************************************************************************************************
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

//******************************************************************************************************************************************************************************************************************************
void createSessionDirectory() {
  memset(directoryName, 0, sizeof directoryName);  
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
  directoryName[0] = compressor;    
  compress(Month);
  directoryName[1] = compressor;
  directoryName[2] = Year[2];
  directoryName[3] = Year[3];  
  compress(Hour); 
  directoryName[4] = compressor;
  directoryName[5] = Minute[0];
  directoryName[6] = Minute[1];
  compressSecond(Second);
  directoryName[7] = compressor;
  SD.mkdir(directoryName);
}

//******************************************************************************************************************************************************************************************************************************
void saveSpectrum() {
  Serial.println("Writing spectrum to file...");
  File file = SD.open(fileName, FILE_WRITE);
  file.print("Filename: ");
  file.print(fileName);
  file.println(",");
  file.print("Measurement Time: ");
  file.print(measurementTime);
  file.println(",");
  file.print("Integration time: ");
  file.print(integrationTime);
  file.println(",");
  file.print("UV Power (W/cm2): ");
  file.print(uvPower);
  file.println(",");
  file.print("Blue Power (W/cm2): ");
  file.print(bluePower);
  file.println(",");  
  file.print("Green Power (W/cm2): ");
  file.print(greenPower);
  file.println(",");
  file.print("Red Power (W/cm2): ");
  file.print(redPower);
  file.println(",");
  file.println("(Wavelength, Raw Count, Corrected Count, W/cm2)");
  for (int i = 0; i < dataCount; i++) {
    file.print(wavelengths[i]);
    file.print(",");
    file.print(spectrum[i]);
    file.print(",");
    file.print(correctedSpectrum[i]);
    file.print(",");    
    file.print(spectrumPixelPower[i], 10);
    file.println(",");

    Serial.print(wavelengths[i]);
    Serial.print(", ");
    Serial.print(spectrum[i]);
    Serial.print(", ");
    Serial.print(correctedSpectrum[i]);
    Serial.print(", ");
    Serial.println(spectrumPixelPower[i], 10);
  }
  file.close();
  Serial.println("Spectrum saved.");
}

//******************************************************************************************************************************************************************************************************************************
void saveSessionSpectrum() {
  Serial.println("Writing spectrum to file...");
  char filePath[45] = "";
  strcat(filePath, directoryName);
  strcat(filePath, "/");
  strcat(filePath, fileName);
  File file = SD.open(filePath, FILE_WRITE);
  Serial.println(filePath);
  file.print("Filename: ");
  file.print(fileName);
  file.println(",");
  file.print("Measurement Time: ");
  file.print(measurementTime);
  file.println(",");
  file.print("Integration time: ");
  file.print(integrationTime);
  file.println(",");
  file.print("UV Power (W/cm2): ");
  file.print(uvPower);
  file.println(",");
  file.print("Blue Power (W/cm2): ");
  file.print(bluePower);
  file.println(",");  
  file.print("Green Power (W/cm2): ");
  file.print(greenPower);
  file.println(",");
  file.print("Red Power (W/cm2): ");
  file.print(redPower);
  file.println(",");
  file.println("(Wavelength, Raw Count, Corrected Count, W/cm2)");
  for (int i = 0; i < dataCount; i++) {
    file.print(wavelengths[i]);
    file.print(",");
    file.print(spectrum[i]);
    file.print(",");
    file.print(correctedSpectrum[i]);
    file.print(",");
    file.print(spectrumPixelPower[i]);
    file.println(",");

    Serial.print(wavelengths[i]);
    Serial.print(", ");
    Serial.print(spectrum[i]);
    Serial.print(", ");
    Serial.print(correctedSpectrum[i]);
    Serial.print(", ");
    Serial.println(spectrumPixelPower[i]);
  }
  file.close();
  Serial.println("Spectrum saved.");
}

//******************************************************************************************************************************************************************************************************************************
void saveRawSpectrum(){
    Serial.println("Writing spectrum to file...");
    setFileName();
    File file = SD.open(fileName, FILE_WRITE);
    for (int i = 0; i < dataCount; i++) {
      file.print(wavelengths[i]);
      file.print(",");
      file.print(spectrum[i]);
      file.print(",");
      file.println();
  }
  file.close();
  Serial.println("Raw spectrum measurement done.");
}