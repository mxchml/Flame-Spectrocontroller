void backgroundMeasurement1() {   //controls the first screen displayed in the "Background Measurement" menu
  if (push_red == 1){
    push_red = 0;
    flicker_control = 1;
  }
  else if (push_yellow == 1){
    push_yellow = 0;
    flicker_control = 1;
  }
  else if (push_green == 1){
    screen_select = 4;
    push_green = 0;
  }
  else if (push_blue == 1){
    screen_select = 2;
    push_blue = 0;
  }  
  else if(flicker_control == 0){
    screenTemplate(1);
    
    myTFT.setTextSize(2);
    myTFT.setTextColor(WHITE);

    myTFT.setCursor(8,110);
    myTFT.print("Cover optical inlet");
    myTFT.setCursor(8,150);
    
    myTFT.print("> Push GREEN to measure");
    myTFT.setCursor(8,170);
    myTFT.print("  new background");    
    
    myTFT.setCursor(8,218);
    myTFT.print("> Push BLUE for Main Menu");

    flicker_control = 1;
  }
  timeTextTFT();
}
//_________________________________________________
 
void backgroundMeasurement2() {   //controls the second screen displayed in the "Background Measurement" menu
    if(exit_condition == 1){
    screen_select = 5;
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
    screenTemplate(1);
    
    checkSpectrometerConnection();    //confirm the spectroradiometer is connected to the platform

    myTFT.setTextSize(2);
    myTFT.setTextColor(WHITE);

    myTFT.setCursor(9,100);
    myTFT.print("Keep inlet fixed");
    
    myTFT.setCursor(9,120);
    myTFT.print("Taking measurement...");
  
    setMeasurementName();   //set the measurement name using the time taken from the RTC
    setIntegrationTime();   //set the integration time by taking a preceeding measurement and adjusting based on the peak signal
    readSpectrum();   //take a spectroradiometric measurement
    linearityCorrection();    //correct the raw signal for linearity
    identifySignal();   //perform the stray light and thresholding corrections
    calWattsfromCounts();   //convert the raw signal to counts
    computePowerBounds();   //compute the total power measured in four wavelength bounds
    updateBackground();   //update the global array with the data of the just taken spectrum
    
    exit_condition = 1; 
    flicker_control = 1;
  }
  timeTextTFT();
}
//_________________________________________________

void backgroundMeasurement3() {   //controls the third screen displayed in the "Background Measurement" menu
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
    screenTemplate(1);
    
    myTFT.setCursor(8,218);
    myTFT.setTextSize(2);
    myTFT.setTextColor(WHITE);
    myTFT.print("> Push BLUE for main menu");

    outputMeasurement();  
    resetMeasurement();
  }
  timeTextTFT();
}