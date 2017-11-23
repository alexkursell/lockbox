/*
 * Servo library, contains functions to open and close the lock.
 */

//Create servo object
Servo servo;

//Flag to store whether the lock is locked or not.
bool is_locked = false;

void servo_init(){
  pinMode(SERVO_PIN, OUTPUT);
}

void switch_box(){
  if(is_locked) open_box();
  else lock_box();
}

void lock_box(){
  if(!is_locked){ //Only try to lock box if not already locked.
    servo.attach(SERVO_PIN);
    servo.writeMicroseconds(LOCK_POSITION);
    is_locked = true;
    delay(1000); //Wait for servo to move into position
    servo.detach();
  }
  
}

void open_box(){
  is_locked = false;
  servo.attach(SERVO_PIN);
  servo.writeMicroseconds(OPEN_POSITION);
  delay(1000); //Wait for servo to move into position
  servo.detach();
}



