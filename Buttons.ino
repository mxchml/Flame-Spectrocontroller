void buttonsStart(){
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_BLUE, INPUT_PULLUP);
  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);
}

void yellowButtonPush(){
  int buttonValue = digitalRead(BUTTON_YELLOW);
  if (buttonValue == LOW){
    yellow = 1;
    flickerControl = 0;
    delay(200);
  }
}
void redButtonPush(){
  int buttonValue = digitalRead(BUTTON_RED);
  if (buttonValue == LOW){
    red = 1;
    flickerControl = 0;
    delay(200);
  }
}
void greenButtonPush(){
  int buttonValue = digitalRead(BUTTON_GREEN);
  if (buttonValue == LOW){
    green = 1;
    flickerControl = 0;
    delay(200);
  }
}
void blueButtonPush(){
  int buttonValue = digitalRead(BUTTON_BLUE);
  if (buttonValue == LOW){
    blue = 1;
    flickerControl = 0;
    delay(200);
  }
}
