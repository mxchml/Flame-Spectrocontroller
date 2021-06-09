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
char measurementTime[30];
char backMeasurementTime[30];

//Spectrum Capture Handlers
word integrationTime = 10;
word scansAccumulated = 0;
int escapeGlobal = 0;
word spectrum[2100];
word backSpectrum[2100];
double spectrumPixelPower[2100];
int dataCount;
double uvPower = 0;
double bluePower = 0;
double greenPower = 0;
double redPower = 0;

double calibrationFactor[2048];
double wavelengthDelta[2048];
double backGroundSpectrum[2048];

void setup() {
  initializeSerial();
  buttonsStart();
  startSDCard();
  beginRTC();
  initializeScreen();
  //changeBaud(6);
}

void loop() {
  yellowButtonPush();
  redButtonPush();
  greenButtonPush();
  blueButtonPush();  
  screenControl();
}

//Support Functions
void initializeSerial(){
  // Open regular serial connections for RX/TX and the software serial pins
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.println("The Serial Port is active");
}

