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
#define MIC_DELTA 10

int i = 0;
int prev_i;
double freq;
int silence_buffer = 0;

int sensorValue;

void setup() {
  servo_init();
  led_init();
  freq_init();
  Serial.begin(9600);
  set_color(0, 0, 255);
  open_box();
  
}

void loop() {
  set_color(0, 0, (double)(i-15)/(60.0)*255); //displays a more visible range
  sensorValue = analogRead(MIC_PIN);
  if (abs(sensorValue-MIC_AVG) > MIC_DELTA) {
    silence_buffer = 0;
    prev_i = i;
    i = freq_listen();
    if (i != 0) { 
      freq = (double)(i)*17.6;//tuned to the standard of 440.0hz for A4
      Serial.println(freq);
    }
    else i = prev_i;
  } else silence_buffer++; //prevents LED flickering (because analog input is sinusodal)
  if (silence_buffer > 100) {
    i = 15;
  }
  delay(10); //keeps the LED from flickering
  
}
