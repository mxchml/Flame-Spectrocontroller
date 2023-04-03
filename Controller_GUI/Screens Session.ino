void sampleSession1(){   //controls the first screen displayed in the "Sampling Session" menu
  if(push_yellow == 1){
    screen_select = 10;
    push_yellow = 0;

    resetPowers();    //reset the global variables that store the spectrum data
    resetSession();

    checkSpectrometerConnection();    //ensure that the spectroradiometer is connected to the platform

    start_time = millis();

    setMeasurementName();   //set the measurement name using the time pulled from the RTC 

    memset(session_name, 0, sizeof session_name);
    strcat(session_name, measurement_name);

    memset(char_buffer, 0, sizeof char_buffer);
    strcat(char_buffer, "session_");
    strcat(char_buffer, measurement_name);    

    createDirectory("", char_buffer);   //create the folders on the SD card that will store the spectrum data
    createDirectory(char_buffer, "measurements");
    createTimeSeries(char_buffer, session_name);
  }
  else if(push_blue == 1){
    screen_select = 2;
    push_blue = 0;
  }
  else if(push_red == 1){
    push_red = 0;
    flicker_control = 1;
  }
  else if(push_green == 1){
    push_green = 0;
    flicker_control = 1;
  }
  else if(flicker_control == 0){
    screenTemplate(3);
    
    myTFT.setTextSize(2);
    myTFT.setTextColor(WHITE);
    
    myTFT.setCursor(8,180);
    myTFT.print("> Push YELLOW to start");

    myTFT.setCursor(8,218);
    myTFT.print("> Push BLUE for main menu");

    flicker_control = 1;
  }
  timeTextTFT();
}
//_________________________________________________

void sampleSession2(){   //controls the first screen displayed in the "Sampling Session" menu
  if(push_yellow == 1){
    screen_select = 11;
    push_yellow = 0;
    
    stop_time = millis();
    calSessionEnergy();

  }
  else if(push_red == 1 | push_green == 1 | push_blue == 1){
    push_blue = 0;
    push_green = 0;
    push_red = 0;
  }
  else if(flicker_control == 0){
    screenTemplate(3);
        
    myTFT.setTextSize(2);
    myTFT.setTextColor(WHITE);
    myTFT.setCursor(8,218);
    myTFT.print("> Hold YELLOW to stop");

    myTFT.setTextColor(BLACK);
    myTFT.fillRect(6, 108, 260, 18, WHITE);
    myTFT.setCursor(8,110);
    myTFT.print("Sampling in Progress");

    outputMeasurement();    //display a preview of the measurement on the screen
    resetPowers();  

    setMeasurementName();   //set the measurement name using the time taken from the RTC
    setIntegrationTime();   //set the integration time by taking a preceeding measurement and adjusting based on the peak signal
    readSpectrum();   //take a spectroradiometric measurement
    linearityCorrection();    //correct the raw signal for linearity
    identifySignal();   //perform the stray light and thresholding corrections
    calWattsfromCounts();   //convert the raw signal to counts
    computePowerBounds();   //compute the total power measured in four wavelength bounds

    memset(char_buffer, 0, sizeof char_buffer);   //build the name of the folder that will store the monitoring session data on the SD card
    strcat(char_buffer, "session_");
    strcat(char_buffer, session_name);
    strcat(char_buffer, "/");
    strcat(char_buffer, "measurements");
    saveMeasurementAsText(char_buffer, measurement_name);
    
    calAverageWatts();    //average all measurements taken during this session up to this point

    memset(char_buffer, 0, sizeof char_buffer);   //build the file path to the file that contains the time series of this monitoring session
    strcat(char_buffer, "session_");
    strcat(char_buffer, session_name);
    strcat(char_buffer, "/");
    strcat(char_buffer, "timeseries_");
    strcat(char_buffer, session_name);
    strcat(char_buffer, ".txt");
    appendTimeSeries(char_buffer);

    memset(char_buffer, 0, sizeof char_buffer);
    strcat(char_buffer, "session_");
    strcat(char_buffer, session_name);
    saveSessionSummary(char_buffer);

    delay(100);
  }
  timeTextTFT();
}
//_________________________________________________

void sampleSession3(){   //controls the first screen displayed in the "Sampling Session" menu
  if(push_blue == 1){
    screen_select = 2;
    push_blue = 0;
    push_yellow = 0;
    
    resetMeasurement();
    resetSession();
  }
  else if(push_red == 1){
    push_red = 0;
    flicker_control = 1;
  }
  else if(push_green == 1){
    push_green = 0;
    flicker_control = 1;
  }
  else if(flicker_control == 0){
    screenTemplate(3);

    myTFT.setTextSize(2);
    myTFT.setTextColor(WHITE);
    myTFT.setCursor(8,218);
    myTFT.print("> Push BLUE for main menu");

    memset(char_buffer, 0, sizeof char_buffer);
    strcat(char_buffer, "session_");
    strcat(char_buffer, session_name);
    saveSessionSummary(char_buffer);

    outputSession();
    
    flicker_control = 1;
  }
  timeTextTFT();
}