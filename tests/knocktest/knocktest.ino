#include <Servo.h> 

#define LOCK_POSITION 2000
#define OPEN_POSITION 1200
#define SERVO_PIN 3
#define MIC_PIN A0

#define MIC_AVG 360
#define MIC_DELTA 350


Servo servo;
bool is_locked;


void lock_box(){
  servo.writeMicroseconds(LOCK_POSITION);
  is_locked = true;
}

void open_box(){
  servo.writeMicroseconds(OPEN_POSITION);
  is_locked = false;
}

void setup() {
  //Initialize servo.
  pinMode(SERVO_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
  open_box();
}

void loop() {
  int sensorValue = analogRead(MIC_PIN);
  // print out the value you read:

  if(abs(sensorValue - MIC_AVG) > MIC_DELTA){
    if(is_locked){ open_box(); }
    else {lock_box();}
    delay(1000);
  }
  delay(10); 
}
