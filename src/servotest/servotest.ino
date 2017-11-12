#include <Servo.h> 

#define LOCK_POSITION 2000
#define OPEN_POSITION 1200
#define SERVO_PIN 3
#define MIC_PIN A0


Servo servo;

void lock_box(){
  servo.writeMicroseconds(LOCK_POSITION);
}

void open_box(){
  servo.writeMicroseconds(OPEN_POSITION);
}

void setup() {
  //Initialize servo.
  pinMode(SERVO_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
}

void loop() {
  //5 second cycle of locked and unlocked.
  open_box();
  delay(5000);
  lock_box();
  delay(5000);
}
