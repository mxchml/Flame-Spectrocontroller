void clearSDCard() {
  File root = sd.open("/");
  Serial.println(root.ls());
  clearDirectory(root);
}

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
  File file = sd.open(current_folder, FILE_READ);
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

word parseWordFromFile(File& file) {    //extract a dataword from a file
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

void sendChar32BitDataword(const char* cmd, long data_word) {    //function that formats commands that contain parameters to the format expected by the spectroradiometer
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
//_________________________________________________

void ASCIIMode(){
  Serial1.print("aA");
}
//_________________________________________________

void binaryMode(){
  Serial1.print("bB");
}
//_________________________________________________

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
//_________________________________________________

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
//_________________________________________________