#define LED_R_PIN 5
#define LED_G_PIN 9
#define LED_B_PIN 11

#include <math.h>

const double half_pi = M_PI*.5;
const double period_term = M_PI/32.0;

void led_init(){
  pinMode(LED_R_PIN,OUTPUT);
  pinMode(LED_G_PIN,OUTPUT);
  pinMode(LED_B_PIN,OUTPUT);
}

void set_color(int red_value, int green_value, int blue_value) {
  analogWrite(LED_R_PIN,red_value);
  analogWrite(LED_G_PIN,green_value);
  analogWrite(LED_B_PIN,blue_value);
}
