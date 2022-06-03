void initializeScreen(){    //start the communication between the microcontroller and the LCD screen
    myTFT.begin();
    myTFT.fillScreen(BLACK);
    myTFT.setRotation(3);

    pinMode(LCD_LIGHT, OUTPUT);
    digitalWrite(LCD_LIGHT, HIGH);
}
//_________________________________________________

void screenControl(){   //the main switch that controls which screen gets displayed on the LCD
  switch(screen_select){
    case 1: introScreen();
      break;
    case 2: mainMenu();
      break;
    case 3: backgroundMeasurement1();
      break;
    case 4: backgroundMeasurement2();
      break;
    case 5: backgroundMeasurement3();
      break;
    case 6: singleMeasure1();
      break;
    case 7: singleMeasure2();
      break;      
    case 8: singleMeasure3();
      break;
    case 9: sampleSession1();
      break;
    case 10: sampleSession2();
      break;
    case 11: sampleSession3();
      break;
    case 12: storedData();
      break;
  }
}
//_________________________________________________

void introScreen(){   //the screen that draws the University of Washington logo upon startup
  myTFT.fillScreen(0x0000);
  myTFT.fillTriangle(9,28,111,28,9,66,YELLOW);
  myTFT.fillTriangle(111,28,111,66,9,66,YELLOW);
  myTFT.fillTriangle(216,28,291,28,216,66,YELLOW);
  myTFT.fillTriangle(291,28,291,66,216,66,YELLOW);
  myTFT.fillTriangle(30,66,69,232,84,66,YELLOW);
  myTFT.fillTriangle(84,66,124,232,69,232,YELLOW);
  myTFT.fillTriangle(111,168,124,232,150,130,YELLOW);
  myTFT.fillTriangle(111,168,150,130,144,28,YELLOW);
  myTFT.fillTriangle(144,28,150,130,182,28,YELLOW);
  myTFT.fillTriangle(150,130,182,28,173,232,YELLOW);
  myTFT.fillTriangle(173,232,182,28,212,168,YELLOW);
  myTFT.fillTriangle(173,232,212,168,229,232,YELLOW);
  myTFT.fillTriangle(229,232,212,168,270,66,YELLOW);
  myTFT.fillTriangle(212,168,238,66,270,66,YELLOW);

  screen_select = 2;
}
//_________________________________________________

void mainMenu(){    //the screen that displays the main menu
    if(push_red == 1){
      screen_select = 6;
      push_red = 0;
    }
    else if(push_green == 1){
      screen_select = 3;
      push_green = 0;
    }
    else if(push_yellow == 1){
      screen_select = 9;
      push_yellow = 0;
    }
    else if(push_blue == 1){
      screen_select = 12;
      push_blue = 0;
    }
    
    else if(flicker_control == 0){
      screenTemplate(0);
      
      //Text Settings
      myTFT.setTextColor(WHITE);
      myTFT.setTextSize(2);

      //red
      myTFT.fillRect(0, 80, 160, 80, RED);
      myTFT.fillRect(5, 85, 150, 70, BLACK);
      myTFT.setCursor(15,100);
      myTFT.print("Take Single");
      myTFT.setCursor(15,120);
      myTFT.print("Measurement");

      //yellow
      myTFT.fillRect(160, 80, 160, 80, YELLOW);
      myTFT.fillRect(165, 85, 150, 70, BLACK);
      myTFT.setCursor(193,100);
      myTFT.print("Sampling");
      myTFT.setCursor(198,120);
      myTFT.print("Session");

      //green
      myTFT.fillRect(0, 160, 160, 80, GREEN);
      myTFT.fillRect(5, 165, 150, 70, BLACK);

      myTFT.setCursor(15,180);
      myTFT.print("Background");
      myTFT.setCursor(15,200);
      myTFT.print("Measurement");

      //blue
      myTFT.fillRect(160, 160, 160, 80, BLUE);
      myTFT.fillRect(165, 165, 150, 70, BLACK);

      myTFT.setCursor(203,180);
      myTFT.print("Stored");
      myTFT.setCursor(216,200);
      myTFT.print("Data");

      flicker_control = 1;
      timeTextTFT();
    }
}

//_________________________________________________

void storedData(){    //screen that controls the functionality of accessing data stored on the SD card
  if(push_green == 1){    //increment the file cursor up
    if (countSDFolders() != 0){
      file_selection++;
      if (file_selection == countSDFolders()){
        file_selection = 0;
      }
    }
    else {    //display a NO FILES error on the screen if there are no files
      Serial.println("NO FILES");
      file_selection = 0;
    }
      push_green = 0;
      screen_select = 12;
  }
  
  else if(push_blue == 1){    //increment the file cursor down
    if (countSDFolders() != 0){
      file_selection--;
      if (file_selection == -1){
        file_selection = countSDFolders() - 1;
      }
    }
    else {
      Serial.println("NO FILES");    //display a NO FILES error on the screen if there are no files
      file_selection = 0;
    }
    push_blue = 0;
    screen_select = 12;
  }
  
  else if (push_yellow == 1){   //exit the data preview mode
    screen_select = 2;
    push_yellow = 0;
  }
  else if (push_red == 1){
    screen_select = 12;
    push_red = 0;
  }
  
  else if (flicker_control == 0){   //display the data contained in the file
    screenTemplate(4);
    myTFT.setTextSize(2);
    myTFT.setTextColor(WHITE);

    if (countSDFolders() == 0){
      Serial.println("NO FILES");

      myTFT.setCursor(9,110);
      myTFT.print("NO FILES");
    }
    else {
      getFolderName(file_selection);    //get the name of the folder that the cursor is currently on
      
      myTFT.setTextSize(2);
      myTFT.setCursor(9,110);
      myTFT.print("File:");
      memset(char_buffer, 0, sizeof char_buffer);
      itoa(file_selection, char_buffer, 10);
      myTFT.print(char_buffer);   //print the folder name on screen
      memset(char_buffer, 0, sizeof char_buffer);
      itoa(countSDFolders(), char_buffer, 10);    //display the total number of files on screen
      myTFT.print(" of ");
      myTFT.print(char_buffer);

      myTFT.fillRect(5, 126, 310, 1, WHITE);

      myTFT.setCursor(8,218);
      myTFT.print("> YELLOW for main menu");

      loadFromFile(checkFileCompatability());   //check the file that the cursor is currently on for compatability and if yes then load the file onto the display
    }
    flicker_control = 1;
  }
  timeTextTFT();
}

//_________________________________________________
void screenTemplate(int screen){    //controls the title and color of the items that display on the screen
  char title[20];
  int16_t color;
  
  switch(screen){
    case 0:
      strcpy(title, "Main");
      break;
    case 1:
      strcpy(title,"Background");
      color = GREEN;
      break;
    case 2:
      strcpy(title, "Single M");
      color = RED;
      break;
    case 3: 
      strcpy(title, "Session");
      color = YELLOW;
      break;
    case 4:
      strcpy(title, "Stored Data");
      color = BLUE;
      break;
  }
  myTFT.fillScreen(BLACK);
  myTFT.setTextColor(WHITE);//text
  myTFT.setTextSize(3);

  myTFT.setCursor(10,20);
  myTFT.print(title);
  
  //text
  myTFT.fillRect(0,80,320,160, color);
  myTFT.fillRect(5,85,310,150, BLACK);
  myTFT.setTextSize(2);

  timeTextTFT();
}
//_________________________________________________

void outputMeasurement() {    //controls how the platform displays single measurement data on the screen
    myTFT.setTextSize(2);
    myTFT.setTextColor(WHITE);

    myTFT.setCursor(9,88);
    myTFT.print("ID: ");
    myTFT.print(measurement_name);

    myTFT.fillRect(5, 126, 310, 1, WHITE);

    myTFT.setCursor(9,130);
    myTFT.print("Measurement   uW/cm2");
    
    myTFT.setCursor(9,149);
    myTFT.print("200 - 400nm: ");

    myTFT.setCursor(9,165);
    myTFT.print("400 - 500nm: ");

    myTFT.setCursor(9,181);
    myTFT.print("500 - 600nm: ");
    
    myTFT.setCursor(9,197);
    myTFT.print("600 - 700nm: ");
    
    memset(char_buffer, 0, sizeof char_buffer);
    dtostrf(uv_power, sizeof(char_buffer), 2, char_buffer);   //convert the numerical data to characters for display on the screen.
    myTFT.setCursor(177,149);
    myTFT.print(char_buffer);

    memset(char_buffer, 0, sizeof char_buffer);
    dtostrf(blue_power, sizeof(char_buffer), 2, char_buffer);   //convert the numerical data to characters for display on the screen.
    myTFT.setCursor(177,165);
    myTFT.print(char_buffer);

    memset(char_buffer, 0, sizeof char_buffer);
    dtostrf(green_power, sizeof(char_buffer), 2, char_buffer);   //convert the numerical data to characters for display on the screen.
    myTFT.setCursor(177,181);
    myTFT.print(char_buffer);

    memset(char_buffer, 0, sizeof char_buffer);
    dtostrf(red_power, sizeof(char_buffer), 2, char_buffer);   //convert the numerical data to characters for display on the screen.
    myTFT.setCursor(177,197);    
    myTFT.print(char_buffer);
}
//_________________________________________________

void outputSession() {    //controls how the platform displays monitoring session data on the screen
    
    myTFT.setTextSize(2);
    myTFT.setTextColor(WHITE);

    myTFT.setCursor(9,88);
    myTFT.print("ID: ");
    myTFT.print(measurement_name);

    myTFT.fillRect(5, 126, 310, 1, WHITE);

    myTFT.setCursor(9,130);
    myTFT.print("Accumulated     uJ/cm2");
    myTFT.setCursor(9,149);
    myTFT.print("200 - 400nm: ");
    myTFT.setCursor(9,165);
    myTFT.print("400 - 500nm: ");
    myTFT.setCursor(9,181);
    myTFT.print("500 - 600nm: ");
    myTFT.setCursor(9,197);
    myTFT.print("600 - 700nm: ");

    memset(char_buffer, 0, sizeof char_buffer);
    dtostrf(uv_energy, sizeof(char_buffer), 0, char_buffer);
    myTFT.setCursor(201,149);
    myTFT.print(char_buffer);

    memset(char_buffer, 0, sizeof char_buffer);
    dtostrf(blue_energy, sizeof(char_buffer), 0, char_buffer);
    myTFT.setCursor(201,165);
    myTFT.print(char_buffer);
    
    memset(char_buffer, 0, sizeof char_buffer);
    dtostrf(green_energy, sizeof(char_buffer), 0, char_buffer);
    myTFT.setCursor(201,181);
    myTFT.print(char_buffer);

    memset(char_buffer, 0, sizeof char_buffer);
    dtostrf(red_energy, sizeof(char_buffer), 0, char_buffer);
    myTFT.setCursor(201,197);
    myTFT.print(char_buffer);
}
//_________________________________________________

void timeTextTFT(){   //controls how the platform loads the time from the RTC and loads that time on the display
  DateTime dt = rtc.now();
  
  char char_date[20];
  char TFT_time_displayed[20];
  char rtc_year[5];

  char year_displayed[3];
  char month_displayed[3];
  char day_displayed[3];
  char hour_displayed[3];
  char minute_displayed[3];

  itoa(dt.year(), rtc_year, 10);
  year_displayed[2] = rtc_year[4];
  year_displayed[1] = rtc_year[3];
  year_displayed[0] = rtc_year[2];

  itoa(dt.month(), month_displayed, 10);
  itoa(dt.day(), day_displayed, 10);
  itoa(dt.hour(), hour_displayed, 10);
  itoa(dt.minute(), minute_displayed, 10);

  int hour_len = strlen(hour_displayed);
  if(hour_len == 1){
    hour_displayed[2] = hour_displayed[1];
    hour_displayed[1] = hour_displayed[0];
    hour_displayed[0] = '0';
  }

  int minute_len = strlen(minute_displayed);
  if(minute_len == 1){
    minute_displayed[2] = minute_displayed[1];
    minute_displayed[1] = minute_displayed[0];
    minute_displayed[0] = '0';
  }

  strcpy(char_date,month_displayed);
  strcat(char_date,"/");
  strcat(char_date,day_displayed);
  strcat(char_date,"/");  
  strcat(char_date,year_displayed);

  strcpy(TFT_time_displayed,hour_displayed);
  strcat(TFT_time_displayed,":");
  strcat(TFT_time_displayed,minute_displayed);

  myTFT.setTextSize(2);
  myTFT.setTextColor(WHITE);

  myTFT.setCursor(231,20);
  myTFT.print(char_date);

  if(strcmp(TFT_time_displayed, now_time)){
    strcpy(now_time, TFT_time_displayed);
    myTFT.fillRect(225,40,75,20, BLACK);
  }
  myTFT.setCursor(255,40);
  myTFT.print(now_time);

  backgroundActiveCheck();
}
//_________________________________________________

void backgroundActiveCheck(){   //checks to see if a background has been taken and if so loads the timestamp of that measurement on screen
  myTFT.setTextSize(1);
  memset(char_buffer, 0, sizeof char_buffer);
  if (!strcmp(background_measurement_name, "    Background: ZERO")) {
      myTFT.drawRect(221, 61, 300, 13, WHITE);
      myTFT.setCursor(200, 64);
      myTFT.setTextColor(WHITE);
      myTFT.print(background_measurement_name);

  }
  else {
      myTFT.drawRect(171, 61, 300, 13, WHITE);
      memset(char_buffer, 0, sizeof char_buffer);
      strcat(char_buffer, "BG: ");
      strcat(char_buffer, background_measurement_name);
      
      myTFT.setTextColor(WHITE);
      myTFT.setCursor(174, 64);
      myTFT.print(char_buffer);
  }
}