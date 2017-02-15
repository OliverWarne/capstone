#include <Time.h>
#include <TimeLib.h>
#include <Wire.h>
#include <DS1302RTC.h>

/*================
  PIN DEFINITIONS
 ===============*/

#define RST_PIN 2
#define SDA_PIN 3
#define SCL_PIN 4

DS1302RTC rtc(RST_PIN, SDA_PIN, SCL_PIN);
time_t my_time;

// seconds - keep the lights on for this amount of time
#define ONTIME    1

// seconds - keep the lights off for this amount of time
#define OFFTIME   2

// on/off led cycles - how many times that the LEDs have been turned off.
// After this is exceded, arduino goes into sleep mode
#define CYCLES    5

// seconds - Wakeup this often to check if the correct time has passed
#define WAKEUPPERIOD 10

// seconds - how long should it stay asleep
#define SLEEPTIME 28800

unsigned int  state      = 0;
time_t wait_time  = 0;
time_t sleep_time = 0;
unsigned long loop_count = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  setSyncProvider(rtc.get);
  if(timeStatus() == timeSet) {
    Serial.println(" Ok!");
  } else {
    Serial.println(" FAIL!");
  }
}

void loop() {

  switch(state) {

    case 0: // initial state
      Serial.print("case: ");
      Serial.println(state);
      //turn on
      digitalWrite(LED_BUILTIN, HIGH);
      //wait until current time + 1 second
      wait_time = rtc.get() + ONTIME;
      Serial.println(wait_time);
      state = 1;
      break;

    case 1: // short LED wait state
      Serial.print("case: ");
      Serial.println(state);
      //wait until time meets it
      if(rtc.get() > wait_time) {
        state = 2;
      };
      break;

    case 2: // turn off, wait, then check loops
      Serial.print("case: ");
      Serial.println(state);
      digitalWrite(LED_BUILTIN, LOW);
      wait_time = rtc.get() + OFFTIME;
      while ( wait_time > rtc.get()) {
        delay(1000);
      }
      loop_count = loop_count + 1;
      state = 3;
      break;
    
    case 3: // loop checker
      Serial.print("case: ");
      Serial.println(state);
      if(loop_count >= CYCLES) {
        state = 4;
      } else {
        state = 0;
      }
      break;
      
    case 4: // begin the long wait; sleepytime
      Serial.print("case: ");
      Serial.println(state);
      digitalWrite(LED_BUILTIN, LOW);
      sleep_time = (rtc.get() + SLEEPTIME);
      state = 5;
      break;
      
    case 5: //wait, wakeup every 10 minutes to check the status
      Serial.print("case: ");
      Serial.println(state);
      unsigned long wakeup_period = WAKEUPPERIOD;
      delay(wakeup_period * 1000);
      time_t cur_time = rtc.get();
      Serial.print("Sleeping for ");
      Serial.print(sleep_time - cur_time);
      Serial.println("longer");
      if(cur_time >= sleep_time) {
        state = 0;
      };
      break;
  }
}
