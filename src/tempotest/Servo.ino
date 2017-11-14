/*
 * Servo library, contains functions to open and close the lock.
 */

#include <Servo.h> 

Servo servo;

void servo_init(){
  pinMode(SERVO_PIN, OUTPUT);
}

void lock_box(){
  servo.attach(SERVO_PIN);
  servo.writeMicroseconds(LOCK_POSITION);
  delay(1000);
  servo.detach();
}

void open_box(){
  servo.attach(SERVO_PIN);
  servo.writeMicroseconds(OPEN_POSITION);
  delay(1000);
  servo.detach();
}

