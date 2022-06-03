void singleMeasure1(){   //controls the first screen displayed in the "Take Single Measurement" menu
  if (push_red == 1){
    screen_select = 7;
    push_red = 0;
  }
  else if (push_yellow == 1) {
    screen_select = 7;
    subtract_background = 1;    
    push_yellow = 0;
  }
  else if (push_green == 1) {
    push_green = 0;
    screen_select = 3;
  }
  else if (push_blue == 1) {
    push_blue = 0;
    screen_select = 2;
  }  
  else if(flicker_control == 0){
    screenTemplate(2);
    
    myTFT.setTextSize(2);
    myTFT.setTextColor(WHITE);

    myTFT.setCursor(8,87);
    myTFT.print("> Push RED for irradiance");
    myTFT.setCursor(8,104);
    myTFT.print("  measurement");

    myTFT.setTextSize(2);
    myTFT.setCursor(8,131);
    myTFT.print("> Push YELLOW to measure");
    myTFT.setCursor(8,148);
    myTFT.print("  and subtract background");

    myTFT.setCursor(8,175);
    myTFT.print("> Push GREEN to measure");
    myTFT.setCursor(8,192);
    myTFT.print("  new background");

    myTFT.setCursor(8,218);
    myTFT.print("> Push BLUE for main menu");

    flicker_control = 1;
  }
  timeTextTFT();
}
//_________________________________________________

void singleMeasure2(){   //controls the second screen displayed in the "Take Single Measurement" menu
  if(exit_condition == 1){
    screen_select = 8;
    flicker_control = 0;
    exit_condition = 0;
  }
  else if(push_green == 1 | push_blue == 1 | push_yellow == 1 | push_red == 1){
    push_blue = 0;
    push_green = 0;
    push_yellow = 0;
    push_red = 0;
    
    flicker_control = 1;
  }
  else if(flicker_control == 0){
    screenTemplate(2);
    
    checkSpectrometerConnection();

    myTFT.setTextSize(2);
    myTFT.setTextColor(WHITE);

    if (subtract_background == 1) {
      myTFT.setCursor(9,100);
      myTFT.print("Measuring with");
      myTFT.setCursor(9,120);
      myTFT.print("background subtracted...");
    }
    else {
      myTFT.setCursor(9,100);
      myTFT.print("Keep inlet fixed");
      myTFT.setCursor(9,120);
      myTFT.print("Taking measurement...");
    }
  
    setMeasurementName();   //set the measurement name using the time taken from the RTC
    setIntegrationTime();   //set the integration time by taking a preceeding measurement and adjusting based on the peak signal
    readSpectrum();   //take a spectroradiometric measurement
    linearityCorrection();    //correct the raw signal for linearity
    identifySignal();   //perform the stray light and thresholding corrections
    calWattsfromCounts();   //convert the raw signal to counts
    computePowerBounds();   //compute the total power measured in four wavelength bounds

    memset(current_folder, 0, sizeof current_folder);
    strcat(current_folder, "measurement_");
    strcat(current_folder, measurement_name);

    createDirectory("", current_folder);

    memset(char_buffer, 0, sizeof char_buffer);
    strcat(char_buffer, "background_included_");
    strcat(char_buffer, measurement_name);

    saveMeasurementAsText(current_folder, char_buffer);   //save the full measurement in .txt file
    saveMeasurementPreview(current_folder, "arduino_preview_data");   //save only the values computed in the "computePowerBounds" function in .txt file

    if (subtract_background == 1) {   //if the user wants to automatically subtract the background from the spectrum, the following occurs
      for (int i; i < 2100; i++) {    //background gets subtracted from the measurement
        pixel_power[i] = pixel_power[i] - background_pixel_power[i];
      }
      computePowerBounds();      //compute the total power measured in four wavelength bounds

      memset(current_folder, 0, sizeof current_folder);   //generate file names for the files that will store the data from these measurements on the SD card
      strcat(current_folder, "measurement_");
      strcat(current_folder, measurement_name);
      memset(char_buffer, 0, sizeof char_buffer);
      strcat(char_buffer, "background_subtracted_");
      strcat(char_buffer, measurement_name);

      saveMeasurementAsText(current_folder, char_buffer);   //save the full measurement in .txt file
      saveMeasurementPreview(current_folder, "arduino_preview_data");   //save only the values computed in the "computePowerBounds" function in .txt file
    }

    exit_condition = 1; 
    flicker_control = 1;   
  }
  timeTextTFT();
}
//_________________________________________________

void singleMeasure3(){   //controls the third screen displayed in the "Take Single Measurement" menu
  if (push_blue == 1){
    screen_select = 2;
    push_blue = 0;
  }
  else if (push_red == 1 | push_yellow == 1 | push_green == 1){
    push_red = 0;
    push_yellow = 0;
    flicker_control = 1;
  }
  else if(flicker_control == 0){
    flicker_control = 1;
    
    screenTemplate(2);
    
    myTFT.setCursor(8,218);
    myTFT.setTextSize(2);
    myTFT.setTextColor(WHITE);
    myTFT.print("> Push BLUE for main menu");

    outputMeasurement();  
    
    if (subtract_background == 1) {   //save the background measurement subtracted from the spectrum to the SD card itself
      //Save background measurement itself
        for (int i; i < 2100; i++) {
          pixel_power[i] = background_pixel_power[i];
        }
        computePowerBounds();

        memset(current_folder, 0, sizeof current_folder);
        strcat(current_folder, "measurement_");
        strcat(current_folder, measurement_name);

        memset(char_buffer, 0, sizeof char_buffer);
        strcat(char_buffer, "background_");
        strcat(char_buffer, measurement_name);

        saveMeasurementAsText(current_folder, char_buffer);  
    }
    resetMeasurement();
  }
  timeTextTFT();
}