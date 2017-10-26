#include <Servo.h> 

#define LOCK_POSITION 2000
#define OPEN_POSITION 1200
#define SERVO_PIN 3
#define MIC_PIN A0

#define MIC_AVG 360
#define MIC_DELTA 325


Servo servo;
bool is_locked;

int num_knocks = 0;
unsigned long last_knock_time = 0;


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
}

void loop() {
  //Get mic level
  int sensorValue = analogRead(MIC_PIN);

  
  //If it's been more than 1.5 seconds since last knock, reset knock count.
  if(millis() - last_knock_time > 1500){
    num_knocks = 0;
    last_knock_time = millis();
  }

  //If we detect a knock, increment counter and reset last knock time.
  if(abs(sensorValue - MIC_AVG) > MIC_DELTA){
    Serial.println("KNOCK");
    num_knocks += 1;
    last_knock_time = millis();
    Serial.println(num_knocks);
    delay(100);
  }

  //More than 3 knocks means switch lock. Wait 3 seconds.
  if(num_knocks > 3){
    num_knocks = 0;
    if(is_locked){ open_box(); }
    else {lock_box();}
    delay(3000);
  }

  delay(10);

  
   
}
