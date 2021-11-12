void calWattsfromCounts() {
  double integration_time_in_sec = static_cast<double>(integration_time);
  integration_time_in_sec = integration_time_in_sec/1000;
  
  int i=59;
  
  for(i; i < 604; i++){
    double pixel_number = static_cast<double>(corrected_spectrum[i]);
    pixel_power[i] = computePixelPower(pixel_number, integration_time_in_sec, i);
    uv_power += pixel_power[i];
  }
  
  for(i; i < 889; i++){
    double pixel_number = static_cast<double>(corrected_spectrum[i]);
    pixel_power[i] = computePixelPower(pixel_number, integration_time_in_sec, i);
    blue_power += pixel_power[i];
  }
  
  for(i; i < 1183; i++){
    double pixel_number = static_cast<double>(corrected_spectrum[i]);
    pixel_power[i] = computePixelPower(pixel_number, integration_time_in_sec, i);
    green_power += pixel_power[i];
  }
  
  for(i; i < 1489; i++){
    double pixel_number = static_cast<double>(corrected_spectrum[i]);
    pixel_power[i] = computePixelPower(pixel_number, integration_time_in_sec, i);
    red_power += pixel_power[i];
  }
}

void resetPowers(){
  uv_power = 0;
  blue_power = 0;
  green_power = 0;
  red_power = 0;
}

void resetSession() {
  uv_average = 0;
  blue_average = 0;
  green_average = 0;
  red_average = 0;
  
  uv_energy = 0;
  blue_energy = 0;
  green_energy = 0;
  red_energy = 0;
  
  session_measurement_count = 0;
  memset(file_name, 0, sizeof file_name);
}

void resetMeasurement() {
  memset(file_name, 0, sizeof file_name);  
  memset(measurement_time, 0, sizeof measurement_time);  
  memset(spectrum, 0, sizeof spectrum);
  memset(corrected_spectrum, 0, sizeof corrected_spectrum);
  memset(pixel_power, 0, sizeof pixel_power);
  
  integration_time = 10;
  uv_power = 0;
  blue_power = 0;
  green_power = 0;
  red_power = 0;
}

void identifySignal() {
  unsigned long count_sum = 0;
  int loop_execution_count = 107;
  double threshold_limit;
  int i=59;
  
  for(i; i < 166; i++) {
    count_sum += corrected_spectrum[i];
  }

  double background = static_cast<double>(count_sum) / static_cast<double>(loop_execution_count);

  i = 0;
  for(i; i < 369; i++) {
    
    if(abs(static_cast<double>(corrected_spectrum[i]) - background) > background/1.5) {
      corrected_spectrum[i] = corrected_spectrum[i] - background;
    }
    
    else {
      corrected_spectrum[i] = 0;
    }
  }
  
  for(i; i < 604; i++) {
    if(abs(static_cast<double>(corrected_spectrum[i]) - background) > background/2.0) {
      corrected_spectrum[i] = corrected_spectrum[i] - background;
    }
    
    else {
      corrected_spectrum[i] = 0;
    }
  }
  for(i; i < 2048; i++) {
    if(abs(static_cast<double>(corrected_spectrum[i]) - background) > background/10.0) {
      corrected_spectrum[i] = corrected_spectrum[i] - background;
    }
    
    else {
      corrected_spectrum[i] = 0;
    }
  }
}

void identifySignalElectricDark() {
  unsigned long count_sum = 0;
  int loop_execution_count = 18;
  double threshold_limit;
  int i = 3;
  
  for(i; i < 21; i++) {
    count_sum += corrected_spectrum[i];
  }

  double background = static_cast<double>(count_sum) / static_cast<double>(loop_execution_count);

  i = 0;  
    for(i; i < 369; i++) {
      if(abs(static_cast<double>(corrected_spectrum[i]) - background) > background/1.5) {
        corrected_spectrum[i] = corrected_spectrum[i] - background;
      }
      else {
        corrected_spectrum[i] = 0;
      }
  }
  for(i; i < 604; i++) {
    if(abs(static_cast<double>(corrected_spectrum[i]) - background) > background/2.0) {
      corrected_spectrum[i] = corrected_spectrum[i] - background;
    } 
    else {
      corrected_spectrum[i] = 0;
    }
  }
  for(i; i < 2048; i++) {
    if(abs(static_cast<double>(corrected_spectrum[i]) - background) > background/10.0) {
      corrected_spectrum[i] = corrected_spectrum[i] - background;
    }
    else {
      corrected_spectrum[i] = 0;
    }
  }
}

double computePixelPower(double pixel_count, double integration_time_in_sec, int i) {
    double pixel_power = pixel_count;
    pixel_power *= calibration_factors_hard[i];
    pixel_power /= 0.119460; 
    pixel_power /= integration_time_in_sec;
    pixel_power /= wavelength_delta_hard[i];
    return pixel_power;
}

void calAverageWatts() {
  uv_average = (uv_average * session_measurement_count + uv_power) / (session_measurement_count+1);
  blue_average = (blue_average * session_measurement_count + blue_power) / (session_measurement_count+1);
  green_average = (green_average * session_measurement_count + green_power) / (session_measurement_count+1);
  red_average = (red_average * session_measurement_count + red_power) / (session_measurement_count+1);
  
  session_measurement_count++;
}

void calSessionEnergy() {
  session_duration = stop_time - start_time;
  session_duration = session_duration/1000.0;
  
  uv_energy = session_duration * uv_average;
  blue_energy = session_duration * blue_average;
  green_energy = session_duration * green_average;
  red_energy = session_duration * red_average;
}

void writeDoubleAsBytes(unsigned long long_data, File &this_file){
  byte* long_as_bytes = (byte*)&long_data;

  Serial.println("writing double");

  for (int i = 0; i < 4; i++){
    this_file.write(long_as_bytes[i]);
    
    Serial.print("long_as_bytes: ");
    Serial.println(long_as_bytes[i]);
  }
}