void clearSDCard() {
  File root = sd.open("/");
  Serial.println(root.ls());
  clearDirectory(root);
}

void clearDirectory(File &directory){
  
  Serial.println("inside clearDirectory");
  directory.rewindDirectory();
  File cursor;

  cursor = directory.openNextFile();

  while(cursor) {
    cursor.getName(file_path, 256);
    
    if (cursor.isFile() && !cursor.isHidden()){
      cursor.close();
      directory.remove(file_path);
      Serial.println("This file was removed: ");
      Serial.println(file_path);
    } else {
      clearDirectory(cursor);
      cursor.close();

      File to_delete_directory = sd.open(file_path);
      to_delete_directory.rmdir();
      to_delete_directory.close();

      Serial.println("This folder was removed: ");
      Serial.println(file_path);
    }
    cursor = directory.openNextFile();
  }
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