#include "RTClib.h"
#include "SD.h"
#include "SPI.h"
#include "string.h"
#include "TFT.h"
#include "time.h"
#include "Wire.h"
#include "avr/dtostrf.h"

#define SD_SPECTRO_CS 3

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
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char timeStamp[] = "DDD_ DD MMM YYYY hh_mm_ss";
char TIME[20];

//Screen
TFT myTFT = TFT(CS_PIN, DC_PIN, 3);

//File Handler
File root;
int fileCounter = 0;
int fileSelection = 5;

//Data Handlers
char compressor;
char fileName[13] = "";
char backFileName[13] = "";
char directoryName[30] = "";
char measurementTime[30];
char backMeasurementTime[30];
int filePosition = 0;

//Spectrum Measurement Handlers
word integrationTime = 10;
word backgroundIntegrationTime = 100;
word scansAccumulated = 0;
int escapeGlobal = 0;
int dataCount;
word spectrum[2100];
word thresholdedSpectrum[2100];
double spectrumPixelPower[2100];
double uvPower = 0;
double bluePower = 0;
double greenPower = 0;
double redPower = 0;

//Session Accumulated Energy Handlers
double uvAverage = 0;
double blueAverage = 0;
double greenAverage = 0;
double redAverage = 0;
unsigned long startTime = 0;
unsigned long stopTime = 0;
unsigned int sessionMeasureNumber = 0;
unsigned long sessionDuration = stopTime - startTime;
unsigned long uvEnergy;
unsigned long blueEnergy;
unsigned long greenEnergy;
unsigned long redEnergy;



void setup() {
  initializeSerial();
  buttonsStart();
  startSDCard();
  beginRTC();
  initializeScreen();
}

void loop() {
  yellowButtonPush();
  redButtonPush();
  greenButtonPush();
  blueButtonPush();  
  screenControl();
  if (Serial1.available()) {
    Serial.println(Serial1.read());
  }
}

//Basic Support Functions
void initializeSerial(){
  // Open regular serial connections for RX/TX and the software serial pins
  Serial.begin(115200);
  Serial1.begin(9600);
  while (Serial1.available()) {
    Serial.println("flushing the pipe");
    Serial.println(Serial1.read());
  }
  Serial.println("The Serial Port is active");
}

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

