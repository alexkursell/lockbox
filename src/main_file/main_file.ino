/*
 * Main file, contains the "glue logic" that uses the fuctions from the libraries.
 */

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
  freq_init();
  Serial.begin(9600);
  
}

void loop() {
  //sensorValue = analogRead(MIC_PIN); 
  open_box();
  int i = freq_listen();
  Serial.println(i);
  set_color(i * 3, i * 3, i * 3);
}
