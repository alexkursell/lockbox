#define LED_R_PIN 5
#define LED_G_PIN 9
#define LED_B_PIN 11

void set_color(int red_value, int green_value, int blue_value) {
  analogWrite(LED_R_PIN,red_value);
  analogWrite(LED_G_PIN,green_value);
  analogWrite(LED_B_PIN,blue_value);
}

void setup() { 
  pinMode(LED_R_PIN,OUTPUT);
  pinMode(LED_G_PIN,OUTPUT);
  pinMode(LED_B_PIN,OUTPUT);
  Serial.begin(9600);
}

void loop() { 
  set_color(255,0,0);
  Serial.println("Red");
  delay(1000);
  set_color(0,255,0);
  Serial.println("Blue");
  delay(1000);
  set_color(0,0,255);
  Serial.println("Green");
  delay(1000);
  set_color(255,255,0);
  Serial.println("Yellow");
  delay(1000);
  set_color(255,0,255);
  Serial.println("Purple");
  delay(1000);
  set_color(255,128,0);
  Serial.println("Orange?");
  delay(1000);
  set_color(255,255,255);
  Serial.println("White");
  delay(1000);
}
