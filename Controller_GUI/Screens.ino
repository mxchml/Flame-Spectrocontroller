void initializeScreen(){
    myTFT.begin();
}

void screenControl(){
  switch(screen_select){
    case 1: introScreen();
      break;
    case 2: mainMenu();
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

void introScreen(){
  myTFT.background(0,0,0);
  myTFT.fill(255,255,255);
  myTFT.stroke(255,255,255);
  myTFT.triangle(5,14,59,14,5,33);
  myTFT.triangle(59,14,59,33,5,33);
  myTFT.triangle(115,14,155,14,115,33);
  myTFT.triangle(155,14,155,33,115,33);
  myTFT.triangle(16,33,37,116,45,33);
  myTFT.triangle(45,33,66,116,37,116);
  myTFT.triangle(59,84,66,116,80,65);
  myTFT.triangle(59,84,80,65,77,14);
  myTFT.triangle(77,14,80,65,97,14);
  myTFT.triangle(80,65,97,14,92,116);
  myTFT.triangle(92,116,97,14,113,84);
  myTFT.triangle(92,116,113,84,122,116);
  myTFT.triangle(122,116,113,84,144,33);
  myTFT.triangle(113,84,127,33,144,33);
  
  screen_select = 2;
}

void mainMenu(){
    if(green_button == 1){
      screen_select = 6;
      green_button = 0;
    }
    
    else if(yellow_button == 1){
      screen_select = 9;
      yellow_button = 0;
    }
    
    else if(blue_button == 1){
      screen_select = 12;
      blue_button = 0;
    }
    
    else if(flicker_control == 0){
      screenTemplate(0);
      
      myTFT.fill(0,255,0);        //green
      myTFT.rect(0,32,80,47);
      myTFT.fill(0,0,0);          
      myTFT.rect(3,35,74,41);
      myTFT.stroke(255,255,255);
      myTFT.text("Take Single",8,45);
      myTFT.text("Measure",8,55);

      myTFT.fill(0, 0, 255);      //blue
      myTFT.rect(80,32,80,47);
      myTFT.stroke(255,255,255);
      myTFT.fill(0,0,0);
      myTFT.rect(83,35,74,41);
      myTFT.text("Stored",100,45);
      myTFT.text("Data",100,55);
      
      myTFT.fill(255,255,0);      //yellow
      myTFT.rect(0,79,160,47);
      myTFT.fill(0,0,0);
      myTFT.rect(3,82,155,41);
      myTFT.stroke(255,255,255); 
      myTFT.text("Begin Session",43,98);

      flicker_control = 1;
      timeTextTFT();
    }
}

void singleMeasure1(){
  if(blue_button == 1){
    screen_select = 2;
    blue_button = 0;
  }  
  else if(green_button == 1){
    screen_select = 7;
    green_button = 0;
  }
  else if(yellow_button == 1){
    yellow_button = 0;
    flicker_control = 1;
  }
  else if(flicker_control == 0){
    screenTemplate(2);
    
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);
    myTFT.text("> Push GREEN to measure", 4, 105);
    myTFT.text("> Push BLUE for Main Menu", 4, 115);

    flicker_control = 1;
  }
  timeTextTFT();
}

void singleMeasure2(){
  if(exit_condition == 1){
    screen_select = 8;
    flicker_control = 0;
    exit_condition = 0;
  }
  else if(green_button == 1 | blue_button == 1 | yellow_button == 1){
    blue_button = 0;
    green_button = 0;
    yellow_button = 0;
    
    flicker_control = 1;
  }
  else if(flicker_control == 0){
    screenTemplate(2);
    
    checkSpectrometerConnection();

    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);
    myTFT.text("Keep inlet fixed",5,37);
    myTFT.text("Taking measurement...", 5, 47);
    
    setFileName();
    setIntegrationTime();
    readSpectrum();
    linearityCorrection();
    //identifySignal();
    identifySignalElectricDark();
    calWattsfromCounts();
    
    createMeasurementDirectory();
    saveMeasurementAsText();
    saveMeasurementAsBinary();
    
    exit_condition = 1; 
    flicker_control = 1;
  }
  timeTextTFT();
}

void singleMeasure3(){
  if(blue_button == 1){
    screen_select = 2;
    blue_button = 0;
  }
  else if(green_button == 1 | yellow_button == 1){
    green_button = 0;
    yellow_button = 0;
    flicker_control = 1;
  }
  else if(flicker_control == 0){
    flicker_control = 1;
    
    screenTemplate(2);
    myTFT.text("> Push BLUE for main menu", 4, 115);

    outputMeasurement();  
    resetMeasurement();
  }
  timeTextTFT();
}

//******************************************************************************************************************************************************************************************************************************
void sampleSession1(){
  if(yellow_button == 1){
    screen_select = 10;
    yellow_button = 0;

    checkSpectrometerConnection();

    start_time = millis();
    createSessionFolder();
  }
  else if(blue_button == 1){
    screen_select = 2;
    blue_button = 0;
  }
  else if(green_button == 1){
    green_button = 0;
    flicker_control = 1;
  }
  else if(flicker_control == 0){
    screenTemplate(3);
    
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);
    myTFT.text("> Push BLUE for M Menu", 4, 105);
    myTFT.text("> Push YELLOW to start", 4, 115);
    
    flicker_control = 1;
  }
  timeTextTFT();
}

//******************************************************************************************************************************************************************************************************************************
void sampleSession2(){
  if(yellow_button == 1){
    screen_select = 11;
    yellow_button = 0;
    
    stop_time = millis();
    calSessionEnergy();
  }
  else if(green_button == 1 | blue_button == 1){
    blue_button = 0;
    green_button = 0;
  }
  else if(flicker_control == 0){
    screenTemplate(3);
    
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);
    myTFT.text("Sample session active",5,37);
    
    outputMeasurement();
    resetPowers();
    
    myTFT.text("> HOLD YELLOW to stop", 5, 115);
    setFileName();
    setIntegrationTime();
    
    readSpectrum();
    linearityCorrection();
    //identifySignal();
    identifySignalElectricDark();
    calWattsfromCounts();
    saveSessionSpectrum();
    calAverageWatts();
    appendTimeSeries();

    delay(100);
  }
  timeTextTFT();
}

void sampleSession3(){
  if(blue_button == 1){
    screen_select = 2;
    blue_button = 0;
    yellow_button = 0;
    
    resetMeasurement();
    resetSession();
  }

  else if(flicker_control == 0){
    screenTemplate(3);
    myTFT.text("> Push BLUE for main menu", 4, 115);

    generateSessionBinarySummary();
    generateSessionTextSummary();
    
    outputSession();
    
    flicker_control = 1;
  }

  else if(green_button == 1){
    green_button = 0;
    flicker_control = 1;
  }
  
  timeTextTFT();
}

void storedData(){
  if(green_button == 1){
    if (countSDFolders() != 0){
      file_selection++;
      if (file_selection == countSDFolders()){
        file_selection = 0;
      }
    }
    else {
      Serial.println("NO FILES");
      file_selection = 0;
    }
      green_button = 0;
      screen_select = 12;
  }
  
  else if(blue_button == 1){
    if (countSDFolders() != 0){
      file_selection--;
      if (file_selection == -1){
        file_selection = countSDFolders() - 1;
      }
    }
    else {
      Serial.println("NO FILES");
      file_selection = 0;
    }
    blue_button = 0;
    screen_select = 12;
  }
  
  else if (yellow_button == 1){
    screen_select = 2;
    yellow_button = 0;
  }
  
  else if (flicker_control == 0){
    char output_char[20];
    screenTemplate(4);
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);

    if (countSDFolders() == 0){
      Serial.println("NO FILES");
      myTFT.text("NO FILES", 5, 37);
    }
    else {
      myTFT.text("File ID: ", 5, 37);
      itoa(file_selection, output_char, 10);
      myTFT.text(output_char, 70, 37);

      myTFT.text("COUNT:", 110, 37);
      itoa(countSDFolders(), output_char, 10);
      myTFT.text(output_char, 150, 37);

      loadFromFile(file_selection);
    }

    myTFT.text("> YELLOW for main menu", 4, 115);

    flicker_control = 1;
  }
  timeTextTFT();
}

//******************************************************************************************************************************************************************************************************************************
void screenTemplate(int screen){
  char title[20];
  int red = 0;
  int green = 0;
  int blue = 0;
  
  switch(screen){
    case 0:
      strcpy(title, "Main");
      break;
    case 1:
      strcpy(title,"Back S");
      blue = 255;
      break;
    case 2:
      strcpy(title, "Single M");
      green = 255;
      break;
    case 3: 
      strcpy(title, "Session");
      red = 255;
      green = 255;
      break;
    case 4:
      strcpy(title, "Stored D");
      blue = 255;
      break;
  }
  myTFT.background(0,0,0);
  myTFT.stroke(255,255,255);  //text
  myTFT.setTextSize(2);
  myTFT.text(title, 5, 10);
  
  myTFT.setTextSize(1);
  myTFT.fill(red, green, blue);
  myTFT.stroke(red, green, blue);  //text
  myTFT.rect(0,32,160,96);
  myTFT.fill(0,0,0);  //text
  myTFT.stroke(0,0,0);  //text
  myTFT.rect(3,35,154,90);
  myTFT.setTextSize(1);

  timeTextTFT();
}

void outputMeasurement() {    
    char output_char[20];
    
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);
    myTFT.text("File ID: ", 5, 47);
    myTFT.text(file_name, 60, 47);
    myTFT.text("AVERAGES    (uW/cm2)", 5,67);
    
    myTFT.text("200-400nm: ", 5, 77);
    dtostrf(uv_power, sizeof(output_char), 2, output_char);
    myTFT.text(output_char,5,77);
    Serial.println(uv_power);
    Serial.println(output_char);
    
    myTFT.text("400-500nm: ", 5, 87);
    dtostrf(blue_power, sizeof(output_char), 2, output_char);
    myTFT.text(output_char,5,87);
    Serial.println(blue_power);
    Serial.println(output_char);
    
    myTFT.text("500-600nm: ", 5, 97);
    dtostrf(green_power, sizeof(output_char), 2, output_char);
    myTFT.text(output_char,5,97);
    Serial.println(green_power);
    Serial.println(output_char);
    
    myTFT.text("600-700nm: ", 5, 107);
    dtostrf(red_power, sizeof(output_char), 2, output_char);
    myTFT.text(output_char,5,107);
    Serial.println(red_power);
    Serial.println(output_char);
}

void outputSession() {
    char output_char[20];
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);

    myTFT.text("S_ID:", 5, 47);
    myTFT.text(file_name, 60, 47);

    myTFT.text("Session Energy (mJ/cm2)", 5,67);
    myTFT.text("200-400nm: ", 5, 77);
    dtostrf(uv_energy, sizeof(output_char), 0, output_char);
    myTFT.text(output_char,5,77);
    Serial.println(uv_energy);
    Serial.println(output_char);
    
    myTFT.text("400-500nm: ", 5, 87);
    dtostrf(blue_energy, sizeof(output_char), 0, output_char);
    myTFT.text(output_char,5,87);
    Serial.println(blue_energy);
    Serial.println(output_char);
    
    myTFT.text("500-600nm: ", 5, 97);
    dtostrf(green_energy, sizeof(output_char), 0, output_char);
    myTFT.text(output_char,5,97);
    Serial.println(green_energy);
    Serial.println(output_char);
    
    myTFT.text("600-700nm: ", 5, 107);
    dtostrf(red_energy, sizeof(output_char), 0, output_char);
    myTFT.text(output_char,5,107);
    Serial.println(red_energy);
    Serial.println(output_char);
}

void timeTextTFT(){
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

  myTFT.setTextSize(1);
  myTFT.text(char_date,110,10);

  if(strcmp(TFT_time_displayed, now_time)){
    strcpy(now_time, TFT_time_displayed);
    myTFT.stroke(0,0,0);
    myTFT.fill(0,0,0);
    myTFT.rect(120,20,40,10);
  }
  myTFT.stroke(255,255,255);
  myTFT.text(now_time,120,20);
  myTFT.setTextSize(text_size);
}