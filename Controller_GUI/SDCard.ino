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

void calculateWattsfromCounts() {

  Serial.print("Start:\t");
  Serial.println(millis());
  
  File spectrumFile = SD.open(fileName, FILE_READ);    // get integration time from spectrum file
  File backgroundFile = SD.open("backG.TXT", FILE_READ);

  char integrationTimeText[10];
  int charPosition = 0;
  while (spectrumFile.available()){
    char c = spectrumFile.read();
    if(c == ','){
      break;
    }
    else if (isDigit(c)){
      integrationTimeText[charPosition] = c;
      charPosition++;
    }
  }
  
  integrationTimeText[charPosition+1] = '\0';
  double integrationTimeFloat = atoi(integrationTimeText);
  integrationTimeFloat = integrationTimeFloat/1000; 
  int i=0;
  
  for(i; i < 56; i++){
    while (spectrumFile.available()){
      char c = spectrumFile.read();      
      if (c == ','){
        break;
      }
    }
    while (backgroundFile.available()){
      char c = backgroundFile.read();
      if (c == ','){
        break;
      }      
    }
  }
  for(i; i < 602; i++){
    char pixelText[10];

    charPosition = 0;
    while (spectrumFile.available()){
      char c = spectrumFile.read();
      if (isDigit(c)){
        pixelText[charPosition] = c;
        charPosition++;
      }
      else if (c == '.'){
        pixelText[charPosition] = c;
        charPosition++;
      }
      else if (c == ','){
        break;
      }
    }
    pixelText[charPosition] = '\0';
    int pixelNumber = atoi(pixelText);
    charPosition = 0;
    char backgroundText[10];
    
    while (backgroundFile.available()) {
      char c = backgroundFile.read();
      if (isDigit(c)){
        backgroundText[charPosition] = c;
        charPosition++;
      }
      else if (c == '.'){
        backgroundText[charPosition] = c;
        charPosition++;
      }
      else if (c == ','){
        break;
      }
    }
    backgroundText[charPosition] = '\0';
    int backgroundNumber = atoi(backgroundText);

    double pixelPower = pixelNumber - backgroundNumber;
    pixelPower = calibrationFactorsHard[i] * pixelPower; 
    pixelPower = pixelPower / 0.119460; 
    pixelPower = pixelPower / integrationTimeFloat;
    pixelPower = pixelPower / wavelengthDeltaHard[i];
    uvPower = pixelPower + uvPower;
  }

  for(i; i < 887; i++){
    char pixelText[10];

    charPosition = 0;
    while (spectrumFile.available()){
      char c = spectrumFile.read();
      if (isDigit(c)){
        pixelText[charPosition] = c;
        charPosition++;
      }
      else if (c == '.'){
        pixelText[charPosition] = c;
        charPosition++;
      }
      else if (c == ','){
        break;
      }
    }
    pixelText[charPosition] = '\0';
    int pixelNumber = atoi(pixelText);
    
    charPosition = 0;
    char backgroundText[10];
    
    while (backgroundFile.available()) {
      char c = backgroundFile.read();
      if (isDigit(c)){
        backgroundText[charPosition] = c;
        charPosition++;
      }
      else if (c == '.'){
        backgroundText[charPosition] = c;
        charPosition++;
      }
      else if (c == ','){
        break;
      }
    }
    backgroundText[charPosition] = '\0';
    int backgroundNumber = atoi(backgroundText);
    
    double pixelPower = pixelNumber - backgroundNumber;
    pixelPower = calibrationFactorsHard[i] * pixelPower;
    pixelPower = pixelPower / 0.119460;
    pixelPower = pixelPower / integrationTimeFloat;
    pixelPower = pixelPower / wavelengthDeltaHard[i];
    bluePower = pixelPower + bluePower;
  }

  for(i; i < 1182; i++){
    char pixelText[10];

    charPosition = 0;
    while (spectrumFile.available()){
      char c = spectrumFile.read();
      if (isDigit(c)){
        pixelText[charPosition] = c;
        charPosition++;
      }
      else if (c == '.'){
        pixelText[charPosition] = c;
        charPosition++;
      }
      else if (c == ','){
        break;
      }
    }
    pixelText[charPosition] = '\0';
    int pixelNumber = atoi(pixelText);
    
    charPosition = 0;
    char backgroundText[10];
    
    while (backgroundFile.available()) {
      char c = backgroundFile.read();
      if (isDigit(c)){
        backgroundText[charPosition] = c;
        charPosition++;
      }
      else if (c == '.'){
        backgroundText[charPosition] = c;
        charPosition++;
      }
      else if (c == ','){
        break;
      }
    }
    backgroundText[charPosition] = '\0';
    int backgroundNumber = atoi(backgroundText);

    double pixelPower = pixelNumber - backgroundNumber;
    pixelPower = calibrationFactorsHard[i] * pixelPower;
    pixelPower = pixelPower / 0.119460;
    pixelPower = pixelPower / integrationTimeFloat;
    pixelPower = pixelPower / wavelengthDeltaHard[i];
    greenPower = pixelPower + greenPower;
  }

  for(i; i < 1488; i++){
    char pixelText[10];

    charPosition = 0;
    while (spectrumFile.available()){
      char c = spectrumFile.read();
      if (isDigit(c)){
        pixelText[charPosition] = c;
        charPosition++;
      }
      else if (c == '.'){
        pixelText[charPosition] = c;
        charPosition++;
      }
      else if (c == ','){
        break;
      }
    }
    pixelText[charPosition] = '\0';
    int pixelNumber = atoi(pixelText);
    
    charPosition = 0;
    char backgroundText[10];
    
    while (backgroundFile.available()) {
      char c = backgroundFile.read();
      if (isDigit(c)){
        backgroundText[charPosition] = c;
        charPosition++;
      }
      else if (c == '.'){
        backgroundText[charPosition] = c;
        charPosition++;
      }
      else if (c == ','){
        break;
      }
    }
    backgroundText[charPosition] = '\0';
    int backgroundNumber = atoi(backgroundText);

    double pixelPower = pixelNumber - backgroundNumber;
    pixelPower = calibrationFactorsHard[i] * pixelPower;
    pixelPower = pixelPower / 0.119460;
    pixelPower = pixelPower / integrationTimeFloat;
    pixelPower = pixelPower / wavelengthDeltaHard[i];
    redPower = pixelPower + redPower;
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

void stopSpinning() {
  bool stopThis = 1;
  while(stopThis){
    Serial.println("Emergency brake deployed");
    delay(2000);
  }
}
