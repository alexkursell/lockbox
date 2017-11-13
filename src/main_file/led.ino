/*
 * LED library, contains functions for setting the color of the RGB LED
 */


#include <math.h>

const double half_pi = M_PI*.5;
const double period_term = M_PI/1250.0;

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

void display_freq(double freq){
    int red   = (int) 128 * (sin(period_term*(freq- 250) + half_pi) + 1);
    int green = (int) 128 * (sin(period_term*(freq- 875) + half_pi) + 1);
    int blue  = (int) 128 * (sin(period_term*(freq-1500) + half_pi) + 1);
    
    set_color(red, green, blue);
}

    


