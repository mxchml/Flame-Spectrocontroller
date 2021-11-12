void printDirectory(File &directory) {
  
  Serial.println("inside printDirectory");
  directory.rewindDirectory();
  File cursor;

  cursor = directory.openNextFile();

  while (cursor) {

    char this_file_name[256];
    cursor.getName(this_file_name, 256);
    Serial.println(this_file_name);
    
    cursor.close();

    cursor = directory.openNextFile();
  }
}

void readRawFile() {
  File file = sd.open("GH2count.TXT", FILE_READ);
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

word parseWordFromFile(File& file) {
    char pixel_text[10];
    int i = 0;
    
    while (file.available()){
      char c = file.read();
      if (isDigit(c) || c == '.'){
        pixel_text[i++] = c;
      }
      else if (c == ','){
        break;
      }
    }
    pixel_text[i] = '\0';
    return atof(pixel_text);
}

void loadFromFile(int fileSelect){
  
  File root = sd.open("/");
  File folder;
  File data_file;
  int file_size;
  byte byte_buf[4];

  memset(file_name, 0, sizeof file_name);
  resetPowers();
  root.rewindDirectory();

  for (int i = 0; i <= fileSelect; i++){
    folder = root.openNextFile();
  }

  folder.getName(file_name, 256);
  Serial.println(file_name);

  if (file_name[0] == 'm'){
    memset(file_path, 0, sizeof file_path);
    strcat(file_path, file_name);
    strcat(file_path, "/");

    strcat(file_path, "binary");
    strcat(file_path, ".dat");

    Serial.println(file_path);

    data_file.open(file_path, O_RDWR);

    data_file.read(byte_buf, 2);
    word read_intTime = (word) byte_buf[1] | (word) byte_buf[0] << 8;
    integration_time = read_intTime;

    for (int i = 0; i < 2048; i++){
      data_file.read(byte_buf, 2);
      word read_spectrum = (word) byte_buf[1] | (word) byte_buf[0] << 8;
      spectrum[i] = read_spectrum;
      if (i == 888){
        Serial.print("Spectrum (888): ");
        Serial.println(spectrum[i]);
      }
    }

    for (int i = 0; i < 2048; i++){
      data_file.read(byte_buf, 2);
      word read_spectrum = (word) byte_buf[1] | (word) byte_buf[0] << 8;
      corrected_spectrum[i] = read_spectrum;
      if (i == 888){
        Serial.print("Corrected Spectrum (888): ");
        Serial.println(corrected_spectrum[i]);
      }
    }
    calWattsfromCounts();

    root.close();
    folder.close();
    data_file.close();

    outputMeasurement();
  }

  else if (file_name[0] == 's'){ 
    memset(file_path, 0, sizeof file_path);
    strcat(file_path, file_name);
    strcat(file_path, "/");
    strcat(file_path, "summary");
    strcat(file_path, ".dat");

    Serial.println(file_path);

    data_file.open(file_path, O_RDWR);
    
    data_file.read(byte_buf, 4);
    memcpy(&uv_energy, byte_buf, sizeof(unsigned long));
    data_file.read(byte_buf, 4);
    memcpy(&blue_energy, byte_buf, sizeof(unsigned long));
    data_file.read(byte_buf, 4);
    memcpy(&green_energy, byte_buf, sizeof(unsigned long));
    data_file.read(byte_buf, 4);
    memcpy(&red_energy, byte_buf, sizeof(unsigned long));
  
    root.close();
    folder.close();
    data_file.close();

    outputSession();
  }

  else {
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);

    myTFT.text("IMPROPER FILE:", 5, 47);
    myTFT.text("NAME:", 5, 57);
    myTFT.text(file_name, 5, 67);
  }
}
