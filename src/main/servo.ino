#include <Servo.h> 

#define LOCK_POSITION 2000
#define OPEN_POSITION 1200
#define SERVO_PIN 3


Servo servo;

void servo_init(){
  pinMode(SERVO_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
}

void lock_box(){
  servo.writeMicroseconds(LOCK_POSITION);
}

void open_box(){
  servo.writeMicroseconds(OPEN_POSITION);
}



