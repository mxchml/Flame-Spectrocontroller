void createDirectory(char root[], char folder_name[]){    //create a folder named "folder_name" in the "root" directory
  char this_char_buffer[256];

  memset(this_char_buffer, 0, sizeof this_char_buffer);   //build the folder path as a string
  strcat(this_char_buffer, root);
  strcat(this_char_buffer, "/");
  strcat(this_char_buffer, folder_name);

  sd.mkdir(this_char_buffer);   //make the folder
}
//_________________________________________________

void saveMeasurementAsText(char folder[], char file_name[]) {   //save a spectrum as a .txt file named "file_name[]" in the "folder[]" directory
  char this_char_buffer[256] = "";
  Serial.println("Writing spectrum to file...");

  memset(this_char_buffer, 0, sizeof this_char_buffer);   //build the file path as a string
  strcat(this_char_buffer, folder);
  strcat(this_char_buffer, "/");
  strcat(this_char_buffer, file_name);
  strcat(this_char_buffer, ".txt");

  Serial.println(this_char_buffer);

  File file = sd.open(this_char_buffer, FILE_WRITE);    //create the file
  
  file.print("Measurement Time: ");   //save the measurement time to the new file
  file.print(file_name);
  file.println(" ");
  file.print("Integration time: ");   //save the duration of the integration time to the new file
  file.print(integration_time);
  file.println(" ");
  
  file.print("UV Power (W/cm2): ");   //save the power collectively measured by the pixels between 200 to 400 nm
  file.print(uv_power);
  file.println(" ");
  file.print("Blue Power (W/cm2): ");   //save the power collectively measured by the pixels between 400 to 500 nm
  file.print(blue_power);
  file.println(" ");  
  file.print("Green Power (W/cm2): ");   //save the power collectively measured by the pixels between 500 to 600 nm
  file.print(green_power);
  file.println(" ");
  file.print("Red Power (W/cm2): ");   //save the power collectively measured by the pixels between 600 to 700 nm
  file.print(red_power);
  file.println(" ");
  
  file.println("(Wavelength) (Raw Count) (Corrected Count) (uW/cm2*nm)");
  
  for (int i = 0; i < data_count; i++) {    //save the spectrum 
    file.print(wavelengths[i]);   // save the wavelength corresponding to the pixel
    file.print("\t");
    file.print(spectrum[i]);   // save the raw signal corresponding to the pixel
    file.print("\t");
    file.print(corrected_spectrum[i]);   // save the stray light and thresholded signal corresponding to the pixel
    file.print("\t");    
    file.print(pixel_power[i], 10);   // save the irradiance in uW/(cm2*nm) corresponding to the pixel
    file.println("\t");
  }
  file.close();
} 
//_________________________________________________

void saveMeasurementPreview(char folder[], char file_name[]) {
  Serial.println("Writing spectrum binary to file...");

  char this_char_buffer[100] = "";

  memset(char_buffer, 0, sizeof this_char_buffer);
  strcat(this_char_buffer, folder);
  strcat(this_char_buffer, "/");
  strcat(this_char_buffer, file_name);
  strcat(this_char_buffer, ".txt");

  if (sd.exists(this_char_buffer)) {
    sd.remove(this_char_buffer);
  }

  File file = sd.open(this_char_buffer, FILE_WRITE);

  file.print("Measurement ID: ");
  file.println(measurement_name);
  file.println();
  file.print("UV Power (uW/cm2): ");
  file.println(uv_power);
  file.print("Blue Power (uW/cm2): ");
  file.println(blue_power);
  file.print("Green Power (uW/cm2): ");
  file.println(green_power);
  file.print("Red Power (uW/cm2): ");
  file.println(red_power);

  file.close(); 
}
//_________________________________________________

void writeDoubleAsBytes(unsigned long long_data, File &this_file){
  byte* long_as_bytes = (byte*)&long_data;

  for (int i = 0; i < 4; i++){
    this_file.write(long_as_bytes[i]);
  }
}
//_________________________________________________

void saveSessionSummary(char folder[]){
  char this_char_buffer[256] = "";

  memset(this_char_buffer, 0, sizeof this_char_buffer);
  strcat(this_char_buffer, folder);
  strcat(this_char_buffer, "/");
  strcat(this_char_buffer, "session_summary");
  strcat(this_char_buffer, ".txt");

  Serial.print("this_char_buffer = ");
  Serial.println(this_char_buffer);

  if (sd.exists(this_char_buffer)) {
    sd.remove(this_char_buffer);
  }

  File file = sd.open(this_char_buffer, FILE_WRITE);

  file.print("Session ID: ");
  file.println(session_name);
  file.println();
  file.print("UV Energy (uJ/cm2): ");
  file.println(uv_energy);
  file.print("Blue Energy (uJ/cm2): ");
  file.println(blue_energy);
  file.print("Green Energy (uJ/cm2): ");
  file.println(green_energy);
  file.print("Red Energy (uJ/cm2): ");
  file.println(red_energy);

  file.close();
}
//_________________________________________________

void createTimeSeries(char folder[], char this_session_name[]) {
  char this_char_buffer[256] = "";

  memset(this_char_buffer, 0, sizeof this_char_buffer);
  strcat(this_char_buffer, folder);
  strcat(this_char_buffer, "/");
  strcat(this_char_buffer, "timeseries_");
  strcat(this_char_buffer, this_session_name);
  strcat(this_char_buffer, ".txt");

  Serial.println("File_name");
  Serial.println(this_char_buffer);

  File file = sd.open(this_char_buffer, FILE_WRITE);
  file.print("Measurement time, Integration time (ms), Pixel Irradiance (uW/cm2*nm) ");
  for (int i; i < 2048; i++) {
    file.print(wavelengths[i]);
    file.print(' ');
  }
  file.println();
  file.close();
}
//_________________________________________________

void appendTimeSeries(char file_name[]) {
  File file = sd.open(file_name, FILE_WRITE);
  
  unsigned long file_end = file.size();

  Serial.print("FILESIZE: ");
  Serial.println(file_end);

  file.seek(file_end);

  file.print(session_name);
  file.print(" ");
  file.print(integration_time);
  file.print(" ");
  
  for (int i; i < 2048; i++) {
    file.print(pixel_power[i]);
    file.print(' ');
  }

  file.println();

  file.close();
}