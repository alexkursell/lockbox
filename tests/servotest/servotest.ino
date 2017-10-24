#include <Servo.h> 

#define LOCK_POSITION 2000
#define OPEN_POSITION 1200


  Servo servo;
void setup() {
  // put your setup code here, to run once:
  pinMode(11, OUTPUT);

  servo.attach(11);
  

}

void loop() {
 // put your main code here, to run repeatedly:
  servo.writeMicroseconds(OPEN_POSITION);
  delay(5000);
  servo.writeMicroseconds(LOCK_POSITION);
  delay(5000);

}
