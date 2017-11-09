#define LOCK_POSITION 2000
#define OPEN_POSITION 1200

#define SERVO_PIN 3
#define MIC_PIN A0


#define LED_R_PIN 5
#define LED_G_PIN 9
#define LED_B_PIN 11

#define MIC_AVG 360
#define MIC_DELTA 350

void setup() {
  servo_init();
  led_init();
  Serial.begin(9600);
}

void loop() {
  //sensorValue = analogRead(MIC_PIN); 
  /*if(abs(sensorValue - MIC_AVG) > MIC_DELTA){
    Serial.println("KNOCK");
    num_knocks += 1;
    last_knock_time = millis();
    Serial.println(num_knocks);
    delay(100);
  }*/ 
  set_color(0,255,0);
  open_box();
  delay(2000);
  set_color(255,0,0);
  lock_box();
  delay(2000);
}
