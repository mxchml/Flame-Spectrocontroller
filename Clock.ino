void beginRTC(){
  //start the rtc clock and set the time to the computer time
  if (! rtc.begin()) {    //start the communication between the real time clock and the arduino
    Serial.println("RTC failed!");    //should the communication between the real time clock and arduino fail, this code stops the whole program and causes the screen to display an error message.
      
    myTFT.fillScreen(BLACK);

    myTFT.setCursor(26,90);
    myTFT.print("Clock failure");
    
    exit(2);
  }
    
  // rtc.adjust(DateTime(__DATE__, __TIME__));
  Serial.println("Real time clock is active");

  char hour_displayed[3];   //variables that store the hour and minute provided by the RTC 
  char minute_displayed[3];

  DateTime dt = rtc.now();    //ask the RTC to send the current date and time in DateTime format

  itoa(dt.hour(), hour_displayed, 10);    //convert the DateTime data to character format
  itoa(dt.minute(), minute_displayed, 10);

  int hour_len = strlen(hour_displayed);    //identify many characters the hour array contains
  
  if(hour_len == 1){    //if the hour array contains a value between 0 and 9, this code ensures proper display on the screen and files
    hour_displayed[2] = hour_displayed[1];   
    hour_displayed[1] = hour_displayed[0];
    hour_displayed[0] = '0';
  }

  int minute_len = strlen(minute_displayed);    //identify many characters the minute array contains
  if(minute_len == 1){    //if the minute array contains a value between 0 and 9, this code ensures proper display on the screen and files
    minute_displayed[2] = minute_displayed[1];
    minute_displayed[1] = minute_displayed[0];
    minute_displayed[0] = '0';
  }
  strcpy(now_time,hour_displayed);    //set the characters representing the hour and minute pulled from the RTC to the global variable that will store them
  strcat(now_time,":");
  strcat(now_time,minute_displayed);
}

