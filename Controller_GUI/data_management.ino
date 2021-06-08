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

void maximum(){
  maxValue = 0;
  for (int i = 0; i < dataCount; i++) {
    if(spectrum[i] > maxValue){
      maxValue = spectrum[i];
    }
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
}

void spectrumAnalysis(File& file) {
  File spectrumFile = SD.open(fileName, FILE_READ);    // get integration time from spectrum file
  File backgroundFile = SD.open("backG.TXT", FILE_READ);

  
}

void calculateWattsfromCounts() {

  Serial.print("Start:\t");
  Serial.println(millis());
  
  File spectrumFile = SD.open(fileName, FILE_READ);    // get integration time from spectrum file
  File backgroundFile = SD.open("backG.TXT", FILE_READ);

  double integrationTimeFloat = parseFromFile(spectrumFile);
  integrationTimeFloat = integrationTimeFloat/1000; 
  int i=0;
  
  for(i; i < 56; i++){
    parseFromFile(spectrumFile);
    parseFromFile(backgroundFile);
  }
  
  for(i; i < 602; i++){
    double pixelNumber = parseFromFile(spectrumFile);
    double backgroundNumber = parseFromFile(backgroundFile);
    uvPower += pixelPower(pixelNumber, backgroundNumber, integrationTimeFloat, i);
  }

  for(i; i < 887; i++){
    double pixelNumber = parseFromFile(spectrumFile);
    double backgroundNumber = parseFromFile(backgroundFile);
    bluePower += pixelPower(pixelNumber, backgroundNumber, integrationTimeFloat, i);
  }

  for(i; i < 1182; i++){
    double pixelNumber = parseFromFile(spectrumFile);
    double backgroundNumber = parseFromFile(backgroundFile);
    greenPower += pixelPower(pixelNumber, backgroundNumber, integrationTimeFloat, i);
  }

  for(i; i < 1488; i++){
    double pixelNumber = parseFromFile(spectrumFile);
    double backgroundNumber = parseFromFile(backgroundFile);
    redPower += pixelPower(pixelNumber, backgroundNumber, integrationTimeFloat, i);
  }
  
  Serial.println(uvPower, 10);
  Serial.println(bluePower, 10);
  Serial.println(greenPower, 10);
  Serial.println(redPower, 10);
           
  Serial.print("End:\t");
  Serial.println(millis());
}

void saveRawSpectrum() {
  Serial.println("Writing spectrum to file...");
  File file = SD.open(fileName, FILE_WRITE);
  for (int i = 0; i < dataCount; i++) {
    file.print(spectrum[i]);
    file.print(",");
    file.println();
  }
  file.close();
  Serial.println("Raw spectrum measurement done.");
}

void saveBackgroundSpectrum() {
  Serial.println("Writing background spectrum to file...");
  File file = SD.open("backG.txt", FILE_WRITE);
  for (int i = 0; i < dataCount; i++) {
    file.print(spectrum[i]);
    file.print(",");
    file.println();
  }
  file.close();
  Serial.println("Raw spectrum measurement done.");
}
