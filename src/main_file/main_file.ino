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
#define MIC_DELTA 25

int prev_i;
double freq;
int silence_buffer = 0;

int sensorValue;

void setup() {
  //servo_init();
  led_init();
  freq_init();
  Serial.begin(9600);
  //set_color(0, 0, 255);
  //open_box();
  
}

void loop() {
    freq = freq_listen();
    if (freq != 0) {
      Serial.println(freq_to_note(freq));
      display_note(freq_to_note(freq));
    }
  delay(100); //keeps the LED from flickering
  
}
