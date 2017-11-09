#include <Servo.h> 


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



