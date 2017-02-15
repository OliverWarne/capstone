// miliseconds - keep the lights on for this amount of time
#define ONTIME    500
// miliseconds - keep the lights off for this amount of time
#define OFFTIME   500
// on/off led cycles - how many times that the LEDs have been turned off.
// After this is exceded, arduino goes into sleep mode
#define CYCLES    10
// seconds - how long should it stay asleep
#define SLEEPTIME 28800

unsigned int  state      = 0;
unsigned long wait_time  = 0;
unsigned long sleep_time = 0;
unsigned long loop_count = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  switch(state) {

    case 0: // initial state
      //turn on
      digitalWrite(LED_BUILTIN, HIGH);
      //wait until current time + 1 second
      wait_time = millis() + ONTIME;
      Serial.print(wait_time);
      state = 1;
      break;

    case 1: // short LED wait state
      //wait until time meets it
      delay(ONTIME);
      if(millis() > wait_time) {
        state = 2;
      };
      break;

    case 2: // turn off, wait, then check loops
      digitalWrite(LED_BUILTIN, LOW);
      delay(OFFTIME);
      loop_count = loop_count + 1;
      state = 3;
      break;
    
    case 3: // loop checker
      if(loop_count >= CYCLES) {
        state = 4;
      } else {
        state = 0;
      }
      break;
      
    case 4: // begin the long wait; sleepytime
      digitalWrite(LED_BUILTIN, LOW);
      sleep_time = (millis()/1000 + 8*60*60);
      state = 5;
      break;
      
    case 5: //wait, wakeup every 10 minutes to check the status
      unsigned long wakeup_period = 10 * 60 * 1000;
      delay(wakeup_period);
      unsigned long cur_time = millis()/1000;
      if(cur_time >= sleep_time) {
        state = 0;
      };
      break;
  }
}
