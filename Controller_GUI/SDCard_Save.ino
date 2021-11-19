void createMeasurementDirectory(){  
  setFileName();
  memset(file_path, 0, sizeof file_path);

  strcat(file_path, "m");
  strcat(file_path, file_name);

  sd.mkdir(file_path);
}

void createSessionFolder(){  
  memset(file_path, 0, sizeof file_path);
  setFileName();
  
  strcat(file_path, "s");
  strcat(file_path, file_name);

  sd.mkdir(file_path);
  
  strcpy(session_folder, file_path);
  Serial.println("session_folder");
  Serial.println(session_folder);
}

void saveMeasurementAsText() {
  Serial.println("Writing spectrum to file...");

  memset(file_path, 0, sizeof file_path);  
  strcat(file_path, "m");
  strcat(file_path, file_name);
  strcat(file_path, "/");
  strcat(file_path, "measurement");
  strcat(file_path, ".txt");

  Serial.println(file_path);

  File file = sd.open(file_path, FILE_WRITE);

  file.print("file_name: ");
  file.print(file_name);
  file.println(",");
  
  file.print("Measurement Time: ");
  file.print(measurement_time);
  file.println(",");
  file.print("Integration time: ");
  file.print(integration_time);
  file.println(",");
  
  file.print("UV Power (W/cm2): ");
  file.print(uv_power);
  file.println(",");
  file.print("Blue Power (W/cm2): ");
  file.print(blue_power);
  file.println(",");  
  file.print("Green Power (W/cm2): ");
  file.print(green_power);
  file.println(",");
  file.print("Red Power (W/cm2): ");
  file.print(red_power);
  file.println(",");
  
  file.println("(Wavelength, Raw Count, Corrected Count, W/cm2)");
  
  for (int i = 0; i < data_count; i++) {
    file.print(wavelengths[i]);
    file.print(",");
    file.print(spectrum[i]);
    file.print(",");
    file.print(corrected_spectrum[i]);
    file.print(",");    
    file.print(pixel_power[i], 10);
    file.println(",");
  }
  file.close();
}

void saveMeasurementAsBinary() {
  Serial.println("Writing spectrum binary to file...");

  memset(file_path, 0, sizeof file_path);  
  strcat(file_path, "m");
  strcat(file_path, file_name);
  strcat(file_path, "/");
  strcat(file_path, "binary");
  strcat(file_path, ".dat");

  Serial.println(file_path);

  File file = sd.open(file_path, FILE_WRITE);

  // write integration time
  byte byte_buf[2];
  byte_buf[1] = (byte) (integration_time >> 8) & 0x7f;
  byte_buf[0] = (byte) integration_time;
  file.write(byte_buf[1]);
  file.write(byte_buf[0]);

  for (int i = 0; i < data_count; i++) {
    byte byte_buf[2];
    byte_buf[1] = (byte) (spectrum[i] >> 8) & 0x7f;
    byte_buf[0] = (byte) spectrum[i];
    file.write(byte_buf[1]);
    file.write(byte_buf[0]);
  }

  for (int i = 0; i < data_count; i++) {  
    byte byte_buf[2];
    byte_buf[1] = (byte) (corrected_spectrum[i] >> 8) & 0x7f;
    byte_buf[0] = (byte) corrected_spectrum[i];
    file.write(byte_buf[1]);
    file.write(byte_buf[0]);
  }
  file.close(); 
}

void generateSessionBinarySummary(){
  memset(file_path, 0, sizeof file_path);
  strcat(file_path, session_folder);
  strcat(file_path, "/");
  strcat(file_path, "summary.dat");

  File file = sd.open(file_path, FILE_WRITE);

  writeDoubleAsBytes(uv_energy, file);
  writeDoubleAsBytes(blue_energy, file);
  writeDoubleAsBytes(green_energy, file);
  writeDoubleAsBytes(red_energy, file);

  file.close();
}

void generateSessionTextSummary(){
  memset(file_path, 0, sizeof file_path);
  strcat(file_path, session_folder);
  strcat(file_path, "/");
  strcat(file_path, "summary.txt");

  File file = sd.open(file_path, FILE_WRITE);

  file.print("UV Energy: ");
  file.println(uv_energy);
  file.print("Blue Energy: ");
  file.println(blue_energy);
  file.print("Green Energy: ");
  file.println(green_energy);
  file.print("Red Energy: ");
  file.print(red_energy);

  file.close();
}

void saveSessionSpectrum() {
  char char_session_measurement_count[10];
  itoa(session_measurement_count, char_session_measurement_count, 10);

  memset(file_path, 0, sizeof file_path);
  strcat(file_path, session_folder);
  strcat(file_path, "/");
  strcat(file_path, "m");
  strcat(file_path, char_session_measurement_count);
  strcat(file_path, ".txt");

  File file = sd.open(file_path, FILE_WRITE);

  file.print("file_name: ");
  file.print(file_name);
  file.println(",");
  
  file.print("Measurement Time: ");
  file.print(measurement_time);
  file.println(",");
  file.print("Integration time: ");
  file.print(integration_time);
  file.println(",");
  
  file.print("UV Power (W/cm2): ");
  file.print(uv_power);
  file.println(",");
  file.print("Blue Power (W/cm2): ");
  file.print(blue_power);
  file.println(",");  
  file.print("Green Power (W/cm2): ");
  file.print(green_power);
  file.println(",");
  file.print("Red Power (W/cm2): ");
  file.print(red_power);
  file.println(",");
  
  file.println("(Wavelength, Raw Count, Corrected Count, W/cm2)");
  
  for (int i = 0; i < data_count; i++) {
    file.print(wavelengths[i]);
    file.print(",");
    file.print(spectrum[i]);
    file.print(",");
    file.print(corrected_spectrum[i]);
    file.print(",");
    file.print(pixel_power[i], 10);
    file.println(",");

  }
  file.close();

  Serial.println("Spectrum saved.");
}

void appendTimeSeries() {
  memset(file_path, 0, sizeof file_path);
  strcat(file_path, session_folder);
  strcat(file_path, "/");
  strcat(file_path, "time_series_(W/cm2)");
  strcat(file_path, ".txt");

  File file = sd.open(file_path, FILE_WRITE);

  uint64_t file_end = file.size();

  file.seek(file_end);

  file.print(measurement_time);
  file.print(",");  
  file.print(uv_power);
  file.print(",");
  file.print(blue_power);
  file.print(",");
  file.print(green_power);
  file.print(",");
  file.print(red_power);
  file.println(",");

  file.close();

}