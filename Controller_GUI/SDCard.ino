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

double pixelPower(double pixelNumber, double backgroundNumber, double integrationTimeFloat, int i) {
    double pixelPower = pixelNumber - backgroundNumber;
    pixelPower *= pgm_read_float(calibrationFactorsHard + i);
    pixelPower /= 0.119460; 
    pixelPower /= integrationTimeFloat;
    pixelPower /= pgm_read_float(wavelengthDeltaHard + i);
    return pixelPower;
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
