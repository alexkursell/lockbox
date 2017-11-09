/*
 * LED library, contains functions for setting the color of the RGB LED
 */

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


