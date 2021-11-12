void startSDCard(){
  //Start the SD card
  Serial.println("Initializing SD card...");
  if (!sd.begin(SD_SPECTRO_CS)) {
    Serial.println("Initialization failed!");
      
    myTFT.background(0,0,0);
    myTFT.fill(255,255,255);
    myTFT.stroke(255,255,255);
    myTFT.text("SD card",14,45);
    myTFT.text("Initialization failed",14, 55);
    
    exit(1);
  }
  else {
    Serial.println("Initialization success");
  }
}

void setFileName() {
  memset(file_name, 0, sizeof file_name);
  memset(measurement_time, 0, sizeof measurement_time);

  DateTime dt = rtc.now(); 
  sprintf(measurement_time, "%4d_%02d_%02d_%02d_%02d_%02d", dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second());
  sprintf(file_name, "%4d%02d%02d%02d%02d%02d", dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second());
}

int countSDFolders(){
  int file_counter = 0;
  
  SdFile root;
  SdFile cursor;

  root.open("/");
  cursor.openNext(&root);

  while (cursor){
    file_counter++;
    cursor.openNext(&root);
  }
  
  Serial.print("Number of Files: ");
  Serial.println(file_counter);

  cursor.close();
  root.close();

  Serial.println("file counter");
  Serial.println(file_counter);

  return file_counter;
}

