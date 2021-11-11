void beginRTC(){
  //start the rtc clock and set the time to the computer time
  if (! rtc.begin()) {  
    Serial.println("RTC failed!");
      
    myTFT.background(0,0,0);
    myTFT.fill(255,255,255);
    myTFT.stroke(255,255,255);
    myTFT.text("Clock failure",14,45);
    
    exit(2);
  }
    
  //rtc.adjust(DateTime(__DATE__, __TIME__));
  Serial.println("Real time clock is active");

  char hour_displayed[3];
  char minute_displayed[3];

  DateTime dt = rtc.now();

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
  strcpy(now_time,hour_displayed);
  strcat(now_time,":");
  strcat(now_time,minute_displayed);
}

