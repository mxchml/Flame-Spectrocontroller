void beginRTC(){
  //start the rtc clock and set the time to the computer time
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  Serial.println("Real time clock is active");
}

void timeTextTFT(){
  DateTime dt = rtc.now();
  char DATE[20];
  char TIME_NOW[20];
  char YearHolder[5];
  itoa(dt.year(), YearHolder, 10);
  char Year[3];
  Year[2] = YearHolder[4];
  Year[1] = YearHolder[3];
  Year[0] = YearHolder[2];
  char Month[3];
  itoa(dt.month(), Month, 10);
  char Day[3];
  itoa(dt.day(), Day, 10);
  char Hour[3];
  itoa(dt.hour(), Hour, 10);
  int hour_len = strlen(Hour);
  if(hour_len == 1){
    Hour[2] = Hour[1];
    Hour[1] = Hour[0];
    Hour[0] = '0';
  }
  char Minute[3];
  itoa(dt.minute(), Minute, 10);
  int minute_len = strlen(Minute);
  if(minute_len == 1){
    Minute[2] = Minute[1];
    Minute[1] = Minute[0];
    Minute[0] = '0';
  }
  strcpy(DATE,Month);
  strcat(DATE,"/");
  strcat(DATE,Day);
  strcat(DATE,"/");  
  strcat(DATE,Year);
  strcpy(TIME_NOW,Hour);
  strcat(TIME_NOW,":");
  strcat(TIME_NOW,Minute);
  myTFT.setTextSize(1);
  myTFT.text(DATE,110,10);
  if(strcmp(TIME_NOW,TIME)){
    strcpy(TIME, TIME_NOW);
    myTFT.stroke(0,0,0);
    myTFT.fill(0,0,0);
    myTFT.rect(120,20,40,10);
  }
  myTFT.stroke(255,255,255);
  myTFT.text(TIME,120,20);
  myTFT.setTextSize(textSize);
}
