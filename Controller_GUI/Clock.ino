void beginRTC(){
  //start the rtc clock and set the time to the computer time
  rtc.begin();
  rtc.adjust(DateTime(__DATE__, __TIME__));
  Serial.println("Real time clock is active");
}

