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
