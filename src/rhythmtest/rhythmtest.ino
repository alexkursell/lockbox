#include <Servo.h>
#include <math.h>

#define LOCK_POSITION 2000
#define OPEN_POSITION 1200
#define SERVO_PIN 3
#define MIC_PIN A0

#define MIC_AVG 360
#define MIC_DELTA 350


Servo servo;
bool is_locked;

int base_tempo = 0;
int pattern[5] =  {4,4,2,2,4};
int knocks[5];
int length = 5;
int count = 0;
int index = 0;
int epsilon = 200;
int valid = 0;

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
  Serial.begin(9600);
  Serial.println("Start");
}

void loop() {
  int sensorValue = analogRead(MIC_PIN);
  // print out the value you read:

  if(abs(sensorValue - MIC_AVG) > MIC_DELTA){
    knocks[index] = count;
    index++;
    if (index == length) {
      base_tempo = (knocks[1]-knocks[0])/pattern[0];
      valid = 1;
      for (int i=2;i<length;i++) {
        if ( abs(((knocks[i]-knocks[i-1]) - pattern[i-1]*base_tempo)) > epsilon) { 
          valid = 0;
          break;	 	
        }
      }
      if (valid) {
        if (is_locked) open_box();
        else lock_box();
      }
      index = 0;
      for (int i=0;i<length;i++) {knocks[i]=0;}
      delay(1000);
    }
    delay(50);
    count=(count+5)%1000000;
  }
  delay(10); 
  count = (count+1)%1000000;
}
