void initializeScreen(){
    myTFT.begin();
}

void screenControl(){
  switch(screenSelect){
    case 1: introScreen();
      break;
    case 2: mainMenu();
      break;
    case 3: darkSpectrum1();
      break;
    case 4: darkSpectrum2();
      break;
    case 5: darkSpectrum3();
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
    case 12: storedMeasures();
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
  delay(1);
  screenSelect = 2;
}

void mainMenu(){
    if(blue == 1){
      screenSelect = 3;
      blue = 0;
    }
    else if(green == 1){
      screenSelect = 6;
      green = 0;
    }
    else if(yellow == 1){
      screenSelect = 9;
      yellow = 0;
    }
    else if(red == 1){
      screenSelect = 12;
      red = 0;
    }
    else if(flickerControl == 0){
      screenTemplate(0);
      myTFT.fill(0,0,255);        //blue
      myTFT.rect(0,32,80,47);
      myTFT.fill(255,255,0);      //yellow
      myTFT.rect(0,79,80,47);
      myTFT.fill(0,255,0);        //green
      myTFT.rect(80,32,80,47);
      myTFT.fill(255,0,0);        //red
      myTFT.rect(80,79,80,47);      
      myTFT.fill(0,0,0);          //black
      myTFT.rect(83,82,74,41);
      myTFT.rect(83,35,74,41);
      myTFT.rect(3,82,74,41);
      myTFT.rect(3,35,74,41);
      myTFT.stroke(255,255,255);  //text
      myTFT.text("Take Dark",14,45);
      myTFT.text("Spectrum",14,55);
      myTFT.text("Take Single",87,45);
      myTFT.text("Measure",100,55);
      myTFT.text("Begin",14,93);
      myTFT.text("Session",14,103);
      myTFT.text("Stored",100,93);
      myTFT.text("Data",100,103);
      flickerControl = 1;
    }
}

void darkSpectrum1(){
  if(blue == 1){
    screenSelect = 4;
    blue = 0;
  }
  else if(green == 1 | red == 1 | yellow == 1){
    green = 0;
    red = 0;
    yellow = 0;
    flickerControl = 1;
  }
  else if(flickerControl == 0){
    screenTemplate(1);
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);
    myTFT.text("Put cap on optical inlet",5,37);
    myTFT.text("...", 5, 47);
    myTFT.text("> Once on, push BLUE", 4, 115);
    flickerControl = 1;
  }
  timeTextTFT();
}

void darkSpectrum2(){
  if(exitCondition == 1){
    screenSelect = 5;
    flickerControl = 0;
    exitCondition = 0;
  }
  else if(green == 1 | blue == 1 | red == 1 | yellow == 1){
    blue = 0;
    green = 0;
    red = 0;
    yellow = 0;
    flickerControl = 1;
  }
  else if(flickerControl == 0){
    screenTemplate(1);
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);
    myTFT.text("Taking dark spectrum",5,37);
    myTFT.text("...", 5, 47);
    
    delay(2000);
    exitCondition = 1; 
    flickerControl = 1;
  }
  timeTextTFT();
}

void darkSpectrum3(){
  if(blue == 1){
    screenSelect = 2;
    blue = 0;
  }
  else if(green == 1 | red == 1 | yellow == 1){
    green = 0;
    red = 0;
    yellow = 0;
    flickerControl = 1;
  }
  else if(flickerControl == 0){
    screenTemplate(1);
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);
    myTFT.text("Dark Spectrum Complete",5,37);
    myTFT.text("Filename:", 5, 47);
    myTFT.text("> Push BLUE for main menu", 4, 115);
    flickerControl = 1;
  }
  timeTextTFT();
}

void singleMeasure1(){
  if(blue == 1){
    screenSelect = 3;
    blue = 0;
  }
  else if(green == 1){
    screenSelect = 7;
    green = 0;
  }
  else if(red == 1 | yellow == 1){
    red = 0;
    yellow = 0;
    flickerControl = 1;
  }
  else if(flickerControl == 0){
    screenTemplate(2);
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);
    myTFT.text("Dark Spectrum File: ",5,37);
    myTFT.text("Taken on: ", 5, 47);
    myTFT.text("> Push BLUE for new DS", 4, 105);
    myTFT.text("> Push GREEN to measure", 4, 115);
    flickerControl = 1;
  }
  timeTextTFT();
}

void singleMeasure2(){
  if(exitCondition == 1){
    screenSelect = 8;
    flickerControl = 0;
    exitCondition = 0;
  }
  else if(green == 1 | blue == 1 | red == 1 | yellow == 1){
    blue = 0;
    green = 0;
    red = 0;
    yellow = 0;
    flickerControl = 1;
  }
  else if(flickerControl == 0){
    screenTemplate(2);
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);
    myTFT.text("Keep inlet fixed",5,37);
    myTFT.text("Taking measurement...", 5, 47);
    delay(2000);
    exitCondition = 1; 
    flickerControl = 1;
  }
  timeTextTFT();
}
void singleMeasure3(){
  if(blue == 1){
    screenSelect = 2;
    blue = 0;
  }
  else if(green == 1 | red == 1 | yellow == 1){
    green = 0;
    red = 0;
    yellow = 0;
    flickerControl = 1;
  }
  else if(flickerControl == 0){
    char outputChar[20];
    screenTemplate(2);
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);
    myTFT.text("Measurement",5,37);
    myTFT.text("Filename: ", 5, 47);
    myTFT.text("AVERAGES    (uW/cm2)", 5,57);
    
    myTFT.text("200-400nm: ", 5, 67);
    //dtostrf(uvPower, sizeof outputChar, 2, outputChar);
    myTFT.text(outputChar,-5,67);
    Serial.println(uvPower);
    Serial.println(outputChar);
    
    myTFT.text("400-500nm: ", 5, 77);
    //dtostrf(bluePower, sizeof outputChar, 2, outputChar);
    myTFT.text(outputChar,-5,77);
    Serial.println(bluePower);
    Serial.println(outputChar);
    
    myTFT.text("500-600nm: ", 5, 87);
    //dtostrf(greenPower, sizeof outputChar, 2, outputChar);
    myTFT.text(outputChar,-5,87);
    Serial.println(greenPower);
    Serial.println(outputChar);
    
    myTFT.text("600-700nm: ", 5, 97);
    //dtostrf(redPower, sizeof outputChar, 2, outputChar);
    myTFT.text(outputChar,-5,97);
    Serial.println(redPower);
    Serial.println(outputChar);
        
    myTFT.text("> Push BLUE for main menu", 4, 115);
    flickerControl = 1;
  }
  timeTextTFT();
}

void sampleSession1(){
  if(blue == 1){
    screenSelect = 3;
    blue = 0;
  }
  else if(yellow == 1){
    screenSelect = 10;
    yellow = 0;
  }
  else if(green == 1 | red == 1){
    red = 0;
    green = 0;
    flickerControl = 1;
  }
  else if(flickerControl == 0){
    screenTemplate(3);
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);
    myTFT.text("Dark Spectrum File: ",5,37);
    myTFT.text("Taken on: ", 5, 47);
    myTFT.text("> Push BLUE for new DS", 4, 105);
    myTFT.text("> Push YELLOW to start", 4, 115);
    flickerControl = 1;
  }
  timeTextTFT();
}

void sampleSession2(){
  if(yellow == 1){
    screenSelect = 11;
    yellow = 0;
  }
  else if(green == 1 | blue == 1 | red == 1){
    blue = 0;
    green = 0;
    red = 0;
    flickerControl = 1;
  }
  else if(flickerControl == 0){
    screenTemplate(3);
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);
    myTFT.text("Sample session active",5,37);
    myTFT.text("> Push YELLOW to stop", 5, 115);
    delay(2000);
    flickerControl = 1;
  }
  timeTextTFT();
}

void sampleSession3(){
  if(blue == 1){
    screenSelect = 2;
    blue = 0;
  }
  else if(green == 1 | red == 1 | yellow == 1){
    green = 0;
    red = 0;
    yellow = 0;
    flickerControl = 1;
  }
  else if(flickerControl == 0){
    char outputChar[20];
    screenTemplate(3);
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);
    myTFT.text("Measurement",5,37);
    myTFT.text("Filename: ", 5, 47);
    myTFT.text("Session Stopped", 5,57);
    myTFT.text("> Push BLUE for main menu", 4, 115);
    flickerControl = 1;  
    myTFT.text("> Push BLUE for main menu", 4, 115);
    flickerControl = 1;
  }
  timeTextTFT();
}
  
void storedMeasures(){
  if(blue == 1){
    screenSelect = 12;
    blue = 0;
  }
  else if(yellow == 1){
    screenSelect = 12;
    yellow = 0;

  }
  else if(red == 1){
    screenSelect = 2;
    red = 0;
  }
  else if(green == 1){
    green = 0;
    flickerControl = 1;
  }
  else if(flickerControl == 0){
    char outputChar[20];
    screenTemplate(4);
    myTFT.setTextSize(1);
    myTFT.stroke(255,255,255);
    myTFT.text("Measurement",5,37);
    myTFT.text("Filename: ", 5, 47);
    myTFT.text("AVERAGES    (uW/cm2)", 5,57);
    
    myTFT.text("200-400nm: ", 5, 67);
    //dtostrf(uvPower, sizeof outputChar, 2, outputChar);
    myTFT.text(outputChar,-5,67);
    Serial.println(uvPower);
    Serial.println(outputChar);
    
    myTFT.text("400-500nm: ", 5, 77);
    //dtostrf(bluePower, sizeof outputChar, 2, outputChar);
    myTFT.text(outputChar,-5,77);
    Serial.println(bluePower);
    Serial.println(outputChar);
    
    myTFT.text("500-600nm: ", 5, 87);
    //dtostrf(greenPower, sizeof outputChar, 2, outputChar);
    myTFT.text(outputChar,-5,87);
    Serial.println(greenPower);
    Serial.println(outputChar);
    
    myTFT.text("600-700nm: ", 5, 97);
    //dtostrf(redPower, sizeof outputChar, 2, outputChar);
    myTFT.text(outputChar,-5,97);
    Serial.println(redPower);
    Serial.println(outputChar);
        
    myTFT.text("> Push BLUE for main menu", 4, 115);
    flickerControl = 1;
  }
  timeTextTFT();
}

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
      strcpy(title,"Dark S");
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
      red = 255;
      break;
  }
  myTFT.background(0,0,0);
  timeTextTFT();
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
}
