char * getFolderName(int selection_index) {   //get the name of the folder identified by the microcontroller cursor(selection_index)
  File root;
  File cursor;

  root.open("/");
  
  for (int i = 0; i <= selection_index; i++) {    //cycle to the file indicated by the cursor
    cursor = root.openNextFile();
  }
  cursor.getName(current_folder, 100);    //extract the name of the folder at the cursor and save the name as characters to the "current_folder_ array"

  root.close();
  cursor.close();

  return current_folder;
}  
//_________________________________________________

int checkFileCompatability() {    //determine if the folder has a structure compatable with the platform
  memset(char_buffer, 0, sizeof char_buffer);
  int position = strcspn(current_folder, "_");    //search the string for the position of the "_" character and return its position
  
  strncpy(char_buffer, current_folder, position);   //copy the name of the folder without the root directory to the char_buffer array

  if (!strcmp(char_buffer, "measurement")) {    //check to see if the folder contains single measurement data
    Serial.println("return1");
    return 1;
  }
  else if (!strcmp(char_buffer, "session")) {    //check to see if the folder contains monitoring session data
    Serial.println("return2");
    return 2;
  }
  else {    //display that the file is not readable on the display
    myTFT.setTextSize(2);
    myTFT.setCursor(9,150);

    myTFT.print("FILE NOT READABLE");
    return 0;
  }
}
//_________________________________________________

double skipDescription(File& file) {    //have the text cursor (NOT THE FILE CURSOR) inside a file skip the description of the line and only return the data in the line
  char text_cursor = file.read();

  while (text_cursor != ':') {    //skip the line description
      text_cursor = file.read();
    }
    memset(char_buffer, 0, sizeof char_buffer);
  while (text_cursor != '\n') {   //place the data of the line into the "char_buffer" array
    text_cursor = file.read();
    if (text_cursor != '\n') {
      strncat(char_buffer, &text_cursor, 1);
    }
  }
  return atof(char_buffer);
}
//_________________________________________________

void loadFromFile(int file_type){   //load the UV, blue, green, and red irradiance data to the microcontroller variables tasked with holding this data
  char text_cursor;

  if (file_type == 1) {   //if the cursor selects a folder containing single measurement data, do the following
    memset(char_buffer, 0, sizeof char_buffer);   //load a character array with the path to the expected location of the file containing the data
    strcat(char_buffer, current_folder);
    strcat(char_buffer, "/arduino_preview_data.txt");

    File file;    //open the file
    file.open(char_buffer, O_READ);

    resetPowers();    //reset the global variable arrays that will take the data from the file
    resetSession();
    resetMeasurement();
    
    text_cursor = file.read();    //load the text cursor with the first byte in the file in order to ensure "text_cursor" does not contain jibberish

    while (text_cursor != ':') {    //move the text cursor to the end of the data description
      text_cursor = file.read();
      Serial.print("text_cursor = ");
      Serial.println(text_cursor);      
    }
    text_cursor = file.read();

    // measurement time
    memset(char_buffer, 0, sizeof char_buffer);
    while (text_cursor != '\n') {   //load measurement time listed in the file into the "char_buffer" array
      text_cursor = file.read();
      Serial.print("text_cursor = ");
      Serial.println(text_cursor);   

      if (text_cursor != '\n') {
        strncat(char_buffer, &text_cursor, 1);
      }
    }
    Serial.println("char_buffer = ");
    Serial.println(char_buffer);
    
    memset(measurement_name, 0, sizeof measurement_name);   //clear the "measurement_name" buffer and load the char_buffer contents to that variable
    strcat(measurement_name, char_buffer);

    memset(char_buffer, 0, sizeof char_buffer);   //clear char_buffer

    uv_power = skipDescription(file);   //load the irradiance measured for all pixels between 200 to 400 nm
    blue_power = skipDescription(file);   //load the irradiance measured for all pixels between 400 to 500 nm
    green_power = skipDescription(file);    //load the irradiance measured for all pixels between 500 to 600 nm
    red_power = skipDescription(file);    //load the irradiance measured for all pixels between 600 to 700 nm

    file.close();

    outputMeasurement();    //cause the LCD to display the values loaded to the global variables
  }
  
  else if (file_type == 2){   //if the cursor selects a folder containing monitoring session data, do the following
    memset(char_buffer, 0, sizeof char_buffer);   //load a character array with the path to the expected location of the file containing the data
    strcat(char_buffer, current_folder);
    strcat(char_buffer, "/session_summary.txt");

    File file;
    file.open(char_buffer, O_RDWR);    //open the file

    resetPowers();    //reset the global variable arrays that will take the data from the file
    resetSession();
    resetMeasurement();

    // measurement time
    text_cursor = file.read();      //load measurement time listed in the file into the "char_buffer" array
    while (text_cursor != ':') {
      text_cursor = file.read();
    }
    memset(char_buffer, 0, sizeof char_buffer);
    while (text_cursor != '\n') {
      text_cursor = file.read();
      if (text_cursor != '\n') {
        strncat(char_buffer, &text_cursor, 1);
      }
    }
    memset(measurement_name, 0, sizeof measurement_name);   //clear the "measurement_name" buffer and load the char_buffer contents to that variable
    strcat(measurement_name, char_buffer);

    memset(char_buffer, 0, sizeof char_buffer);   //clear char_buffer

    uv_energy = skipDescription(file);   //load the accumulated energy measured for all pixels between 200 to 400 nm
    blue_energy = skipDescription(file);   //load the accumulated energy measured for all pixels between 400 to 500 nm
    green_energy = skipDescription(file);   //load the accumulated energy measured for all pixels between 500 to 600 nm
    red_energy = skipDescription(file);   //load the accumulated energy measured for all pixels between 600 to 700 nm

    Serial.print("UV Energy = ");
    Serial.println(uv_energy);

    file.close();

    outputSession();    //cause the LCD to display the values loaded to the global variables
  }
  else {
    myTFT.setTextSize(2);
    myTFT.setCursor(9, 88);
    myTFT.print("ID = ");
    myTFT.print(current_folder);
  }
}
//_________________________________________________

int countSDFolders(){   //count the total number of filers and folders in the root directory of the SD card of the platform
  int file_counter = 0;
  uint64_t file_size = 0;

  File root;
  File cursor;
  File content_check;

  root.open("/");   //open the root directory
  cursor = root.openNextFile();   //open the first file or folder of the root directory
  content_check = cursor.openNextFile();    //attempt to open a file or folder in the sub-directory

  while (cursor){   //check to see if the folder of the root directory contains any files, if not, the instance is not added to the file counter
    if (content_check){
      file_counter++;
    }
    cursor = root.openNextFile();
    content_check = cursor.openNextFile();
  }

  content_check.close();
  cursor.close();
  root.close();

  return file_counter;
}