void startSDCard(){   //start communication between the SD card and microcontroller

  //Start the SD card
  Serial.println("Initializing SD card...");
  
  if (!sd.begin(SD_CS, SPI_HALF_SPEED)) {   //start communication between the SD card and microcontroller
    Serial.println("Initialization failed!");   //should the communication between the SD card and arduino fail, this code stops the whole program and causes the screen to display an error message.
      
      
    myTFT.fillScreen(BLACK);

    myTFT.setCursor(26,90);
    myTFT.print("SD card");

    myTFT.setCursor(26,110);
    myTFT.print("Initialization failed");
    
    exit(1);
  }
  else {
    Serial.println("Initialization success");
  }
}
//_________________________________________________

void setMeasurementName() {   //updates the global variable that holds the spectrum name to a new name generated using the current time pulled from the RTC
  memset(measurement_name, 0, sizeof measurement_name);

  DateTime dt = rtc.now(); 
  sprintf(measurement_name, "%02d-%02d-%02d__%4d-%02d-%02d", dt.hour(), dt.minute(), dt.second(), dt.year(), dt.month(), dt.day());
}
//_________________________________________________

void calWattsfromCounts() {   //convert the spectrum data sent by the spectroradiometer from raw signal to uW/(cm2*nm)
  double integration_time_in_sec = static_cast<double>(integration_time);   //convert the integration time from milliseconds to seconds
  integration_time_in_sec = integration_time_in_sec/1000;
  
  int i=59;   //skip the first fifty nine pixels the manufacturer has not calibrated them
  
  for(i; i < 604; i++){   //convert the raw signal measured by the pixels capturing 200 to 400nm wavelength irradiance
    double pixel_number = static_cast<double>(corrected_spectrum[i]);
    pixel_power[i] = computePixelPower(pixel_number, integration_time_in_sec, i);
  }
  
  for(i; i < 889; i++){   //convert the raw signal measured by the pixels capturing 400 to 500nm wavelength irradiance
    double pixel_number = static_cast<double>(corrected_spectrum[i]);
    pixel_power[i] = computePixelPower(pixel_number, integration_time_in_sec, i);
  }
  
  for(i; i < 1183; i++){   //convert the raw signal measured by the pixels capturing 500 to 600nm wavelength irradiance
    double pixel_number = static_cast<double>(corrected_spectrum[i]);
    pixel_power[i] = computePixelPower(pixel_number, integration_time_in_sec, i);
  }
  
  for(i; i < 1489; i++){   //convert the raw signal measured by the pixels capturing 600 to 700nm wavelength irradiance
    double pixel_number = static_cast<double>(corrected_spectrum[i]);
    pixel_power[i] = computePixelPower(pixel_number, integration_time_in_sec, i);
  }
}
//_________________________________________________

void computePowerBounds() {   //The units of pixel power are uW/(cm2*nm). This code multiplies the 2048 values in pixel power by the wavelength delta to get uW/cm2
  uv_power = 0;
  blue_power = 0;
  green_power = 0;
  red_power = 0;

  int i=59;
  for(i; i < 604; i++){   //sum the power of the 200 to 400nm irradiance
    uv_power += pixel_power[i] * wavelength_delta_hard[i];
  }

  for(i; i < 889; i++){    //sum the power of the 400 to 500nm irradiance
    blue_power += pixel_power[i] * wavelength_delta_hard[i];
  }
  
  for(i; i < 1183; i++){    //sum the power of the 500 to 600nm irradiance
    green_power += pixel_power[i] * wavelength_delta_hard[i];
  }
  
  for(i; i < 1489; i++){    //sum the power of the 600 to 700nm irradiance
    red_power += pixel_power[i] * wavelength_delta_hard[i];
  }
}
//_________________________________________________

void clearDirectory(File &directory){   //clear all files and folders in a file specified by the file path passed to the function
  Serial.println("inside clearDirectory");
  directory.rewindDirectory();
  File cursor;

  cursor = directory.openNextFile();

  while(cursor) {
    cursor.getName(current_folder, 256);
    
    if (cursor.isFile() && !cursor.isHidden()){
      cursor.close();
      directory.remove(current_folder);
      Serial.println("This file was removed: ");
      Serial.println(current_folder);
    } else {
      clearDirectory(cursor);
      cursor.close();

      File to_delete_directory = sd.open(current_folder);
      to_delete_directory.rmdir();
      to_delete_directory.close();

      Serial.println("This folder was removed: ");
      Serial.println(current_folder);
    }
    cursor = directory.openNextFile();
  }
}
//_________________________________________________

void initializeBuffers(){   //return all global variables that hold spectrum data to zero
  for (int i; i < 2100; i++) {
    spectrum[i] = 0;
    corrected_spectrum[i] = 0;
    pixel_power[i] = 0;
    background_pixel_power[i] = 0;
  }
}
//_________________________________________________

void updateBackground(){    //store the array held in pixel power to the background_pixel_power array
  for (int i; i < 2100; i++){
    background_pixel_power[i] = pixel_power[i];
  }

  strcpy(background_measurement_name, measurement_name);
}
//_________________________________________________

void resetPowers(){     //set the global variables that hold the summation of the irradiance values to zero
  uv_power = 0;
  blue_power = 0;
  green_power = 0;
  red_power = 0;
}
//_________________________________________________

void resetSession() {     //set the global variables that hold the summation of the session values to zero
  uv_energy = 0;
  blue_energy = 0;
  green_energy = 0;
  red_energy = 0;
  session_measurement_count = 0;
}
//_________________________________________________

void resetMeasurement() {     //reset the global variables that store spectrum data to zero
  memset(measurement_name, 0, sizeof measurement_name);  
  memset(spectrum, 0, sizeof spectrum);
  memset(corrected_spectrum, 0, sizeof corrected_spectrum);
  memset(pixel_power, 0, sizeof pixel_power);
  
  integration_time = 10;
  uv_power = 0;
  blue_power = 0;
  green_power = 0;
  red_power = 0;

  subtract_background = 0;
}
//_________________________________________________

void identifySignal() {   //stray light correction and signal thresholding
  unsigned long count_sum = 0;
  int loop_execution_count = 107;
  double threshold_limit;
  int i=59;
  
  for(i; i < 166; i++) {   //sum the counts of the pixels measuring between 200 and 240 nm
    count_sum += corrected_spectrum[i];
  }

  double background = static_cast<double>(count_sum) / static_cast<double>(loop_execution_count);   //calculate the average signal measured by a pixel between 200 to 240 nm

  i = 0;
  for(i; i < 369; i++) {    //between 200 and 315 nm: set the signal measured at the pixel to zero if the value of the signal minus background is within 66% of noise
    
    if(abs(static_cast<double>(corrected_spectrum[i]) - background) > background/1.5) {
      corrected_spectrum[i] = corrected_spectrum[i] - background;
    }
    
    else {
      corrected_spectrum[i] = 0;
    }
  }
  
  for(i; i < 604; i++) {    //between 315 and 400nm: set the signal measured at the pixel to zero if the value of the signal minus background is within 50% of noise
    if(abs(static_cast<double>(corrected_spectrum[i]) - background) > background/2.0) {
      corrected_spectrum[i] = corrected_spectrum[i] - background;
    }
    
    else {
      corrected_spectrum[i] = 0;
    }
  }
  for(i; i < 2048; i++) {    //between 400 and 871nm: set the signal measured at the pixel to zero if the value of the signal minus background is within 10% of noise
    if(abs(static_cast<double>(corrected_spectrum[i]) - background) > background/10.0) {
      corrected_spectrum[i] = corrected_spectrum[i] - background;
    }
    
    else {
      corrected_spectrum[i] = 0;
    }
  }
}
//_________________________________________________

void identifySignalElectricDark() {   //electric dark light correction and signal thresholding
  unsigned long count_sum = 0;
  int loop_execution_count = 18;
  double threshold_limit;
  int i = 3;
  
  for(i; i < 18; i++) {   //sum the counts of the 'dark' pixels of the device
    count_sum += corrected_spectrum[i];
  }

  double background = static_cast<double>(count_sum) / static_cast<double>(loop_execution_count);   //calculate the average signal measured by a dark pixel

  i = 0;  
    for(i; i < 369; i++) {    //between 200 and 315 nm: set the signal measured at the pixel to zero if the value of the signal minus background is within 66% of noise
      if(abs(static_cast<double>(corrected_spectrum[i]) - background) > background/1.5) {
        corrected_spectrum[i] = corrected_spectrum[i] - background;
      }
      else {
        corrected_spectrum[i] = 0;
      }
  }
  for(i; i < 604; i++) {    //between 200 and 315 nm: set the signal measured at the pixel to zero if the value of the signal minus background is within 66% of noise
    if(abs(static_cast<double>(corrected_spectrum[i]) - background) > background/2.0) {
      corrected_spectrum[i] = corrected_spectrum[i] - background;
    } 
    else {
      corrected_spectrum[i] = 0;
    }
  }
  for(i; i < 2048; i++) {    //between 200 and 315 nm: set the signal measured at the pixel to zero if the value of the signal minus background is within 66% of noise
    if(abs(static_cast<double>(corrected_spectrum[i]) - background) > background/10.0) {
      corrected_spectrum[i] = corrected_spectrum[i] - background;
    }
    else {
      corrected_spectrum[i] = 0;
    }
  }
}
//_________________________________________________

double computePixelPower(double pixel_count, double integration_time_in_sec, int i) {   //function to convert signal to irradiance in uW/(cm2*nm)
    double this_pixel_power = pixel_count;
    this_pixel_power *= calibration_factors_hard[i];    //multiply by calibration factors
    this_pixel_power /= 0.119460;   //divide by the area of the optical inlet
    this_pixel_power /= integration_time_in_sec;    //divide by the integration time in seconds
    this_pixel_power /= wavelength_delta_hard[i];   //divide by the wavelength range of the pixel
    return this_pixel_power;
}
//_________________________________________________

void calAverageWatts() {    //calculate the average power measured during a monitoring session
  uv_average = (uv_average * session_measurement_count + uv_power) / (session_measurement_count+1);
  blue_average = (blue_average * session_measurement_count + blue_power) / (session_measurement_count+1);
  green_average = (green_average * session_measurement_count + green_power) / (session_measurement_count+1);
  red_average = (red_average * session_measurement_count + red_power) / (session_measurement_count+1);
  
  session_measurement_count++;
}
//_________________________________________________

void calSessionEnergy() {   //calculate the energy accumulated over a monitoring session 
  session_duration = stop_time - start_time;
  session_duration = session_duration/1000.0;
  
  uv_energy = session_duration * uv_average;
  blue_energy = session_duration * blue_average;
  green_energy = session_duration * green_average;
  red_energy = session_duration * red_average;
}