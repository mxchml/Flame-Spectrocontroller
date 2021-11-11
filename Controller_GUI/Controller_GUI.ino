#include "RTClib.h"
#include "SdFat.h"
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
#define BUTTON_YELLOW 12
boolean yellow_button = 0;
boolean green_button = 0;
boolean blue_button = 0;

//ScreenSelect
int screen_select = 1;
boolean flicker_control = 0;
int text_size = 1;
int exit_condition = 0;

//Clock
RTC_DS3231 rtc;
char days_of_the_week[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char time_stamp[] = "DDD_ DD MMM YYYY hh_mm_ss";
char now_time[20];

//Screen
TFT myTFT = TFT(CS_PIN, DC_PIN, 3);

//SD Card
SdFat sd;

//File Handler
int file_selection = 0;

//Data Handlers
char compressor;
char file_name[40] = "";
char file_path[256] = "";
char session_folder[40] = "";
char measurement_time[30];

//Spectrum Measurement Handlers
word integration_time = 100;
word scans_accumulated = 0;
int escape_global = 0;
int data_count;
word spectrum[2100];
word corrected_spectrum[2100];
double pixel_power[2100];
double uv_power = 0;
double blue_power = 0;
double green_power = 0;
double red_power = 0;

//Session Accumulated Energy Handlers
double uv_average = 0;
double blue_average = 0;
double green_average = 0;
double red_average = 0;
unsigned long start_time = 0;
unsigned long stop_time = 0;
unsigned int session_measurement_count = 0;
unsigned long session_duration = stop_time - start_time;
unsigned long uv_energy;
unsigned long blue_energy;
unsigned long green_energy;
unsigned long red_energy;


void setup() {
  initializeSerial();
  buttonsStart();
  initializeScreen();
  startSDCard();
  beginRTC();
  checkSpectrometerConnection();
}

void loop() {
  yellowButtonPush();
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
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);
}

void yellowButtonPush(){
  int button_value = digitalRead(BUTTON_YELLOW);
  if (button_value == LOW){
    yellow_button = 1;
    flicker_control = 0;
    delay(200);
  }
}

void greenButtonPush(){
  int button_value = digitalRead(BUTTON_GREEN);
  if (button_value == LOW){
    green_button = 1;
    flicker_control = 0;
    delay(200);
  }
}

void blueButtonPush(){
  int button_value = digitalRead(BUTTON_BLUE);
  if (button_value == LOW){
    blue_button = 1;
    flicker_control = 0;
    delay(200);
  }
}

