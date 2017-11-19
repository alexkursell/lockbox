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
double note;
double prev_note = 0;
double prev_time = millis();
int silence_buffer = 0;
int ms = millis();

int sensorValue;

void setup() {
  //servo_init();
  led_init();
  freq_init();
  Serial.begin(9600);
  //set_color(0, 0, 255);
  //open_box();
  
}

bool near(double a, double b){
  return abs(a - b) < 0.1;
}

void end_note(double note, double ntime){
  if(near(note, 0)) return;
  Serial.print(note);
  Serial.print(" : ");
  Serial.println(ntime);
}

void loop() { 
    //Note just ended into silence
    if(silence_buffer == 3){
      end_note(prev_note, millis() - prev_time);
    }
    
    //No note currently playing
    if(silence_buffer >= 3){
      prev_note = 0;
      prev_time = millis();
      set_color(0, 0, 0);
    }

    freq = freq_listen();
    note = freq_to_note(freq);

    
    //If no note detected
    if(near(note, 0) || near(note, 74.40) || near(note, 66.08)){
      silence_buffer++;
      note = prev_note;
    }
    //If note detected
    else{
      silence_buffer = 0;
    }
    
    //If note just switched or started
    if(abs(note - prev_note) > 1 ){
      
      //Note just switched
      if(!near(prev_note, 0.0)){
        end_note(prev_note, millis() - prev_time);
      }

      prev_note = note;
      prev_time = millis();
    }
      

  delay(5); //keeps the LED from flickering
  
}
