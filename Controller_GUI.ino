#include "RTClib.h"
#include "SD.h"
#include "SPI.h"
#include "string.h"
#include "TFT.h"
#include "time.h"
#include "Wire.h"

#define SD_SPECTRO_CS 53

//Screen
#define PWM_PIN 8
#define CS_PIN 5
#define DC_PIN 6

//Buttons
#define BUTTON_GREEN 9
#define BUTTON_BLUE 10
#define BUTTON_RED 11
#define BUTTON_YELLOW 12
boolean yellow = 0;
boolean red = 0;
boolean green = 0;
boolean blue = 0;

//ScreenSelect
int screenSelect = 1;
boolean flickerControl = 0;
int textSize = 1;
int exitCondition = 0;

//Clock
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char timeStamp[] = "DDD_ DD MMM YYYY hh_mm_ss";
char TIME[20];

//Screen
TFT myTFT = TFT(CS_PIN, DC_PIN, 3);

//File Handler
File root;
int fileCounter = 0;
int fileSelection = 5;
char fileName[13] = "GH2count.TXT";

//Spectrum Capture Handlers
word integrationTime = 0;
word scansAccumulated = 0;
int escapeGlobal = 0;
word spectrum[2100];
int dataCount;
double uvPower = 0;
double bluePower = 0;
double greenPower = 0;
double redPower = 0;

void setup() {
  initializeSerial();
  buttonsStart();
  startSDCard();
  beginRTC();
  initializeScreen();
  calculateWattsfromCounts();
}

void loop() {
  yellowButtonPush();
  redButtonPush();
  greenButtonPush();
  blueButtonPush();  
  screenControl();
}
