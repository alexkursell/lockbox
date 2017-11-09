#include "led.h"
#include "servo.h"
#include "pattern.h"


void setup() {
  
}

void loop() {
  if(abs(sensorValue - MIC_AVG) > MIC_DELTA){
    Serial.println("KNOCK");
    num_knocks += 1;
    last_knock_time = millis();
    Serial.println(num_knocks);
    delay(100);
  }
}
