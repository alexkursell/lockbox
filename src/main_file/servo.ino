/*
 * Servo library, contains functions to open and close the lock.
 */

#include <Servo.h> 


Servo servo;

bool is_locked = false;

void servo_init(){
  pinMode(SERVO_PIN, OUTPUT);
}

void lock_box(){
  if(!is_locked){
    servo.attach(SERVO_PIN);
    servo.writeMicroseconds(LOCK_POSITION);
    is_locked = true;
    delay(1000);
    servo.detach();
  }
  
}

void open_box(){
  is_locked = false;
  servo.attach(SERVO_PIN);
  servo.writeMicroseconds(OPEN_POSITION);
  delay(1000);
  servo.detach();
}



