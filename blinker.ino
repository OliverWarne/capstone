void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  unsigned int state;
  unsigned long wait_time;
  state = 0;
  switch(state) {
    case 0: // initial state
      //turn on
      digitalWrite(LED_BUILTIN, HIGH);
      //wait until current time + 1 second
      wait_time = millis() + 1000;
      Serial.print(wait_time);
      state = 1;
    case 1: // wait state
      //wait until time meets it
      delay(1000);
      if(millis() > wait_time) {
        state = 2;
      };
    case 2: // turn on then loop back
      digitalWrite(LED_BUILTIN, LOW);
      delay(10000);
      state = 0;
  }
}
