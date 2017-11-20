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

struct Note{
  double pitch;
  double time;
};

int prev_i;
double freq;
double note;
double prev_note = 0;
double prev_time = millis();
int silence_buffer = 0;
int ms = millis();

struct Note init_note_buffer[30];
struct Note *note_buffer = (struct Note *) init_note_buffer;
struct Note last_note = {1, 1};


struct Note inita[] = 
{{1.00 , 1.00},
{0.98 , 0.47},
{1.00 , 0.84},
{1.04 , 1.78},
{0.98 , 0.78},
{0.98 , 1.28},
{1.00 , 0.45},
{1.00 , 1.23},
{1.00 , 1.20},
{1.00 , 0.84},
{1.02 , 1.00},
{1.00 , 1.00},
{1.00 , 1.00},
{1.00 , 1.00},
{1.02 , 1.39},
{0.98 , 1.00},
{0.99 , 0.99}};

struct Note *hot_cross_buns = (struct Note*) inita;

int note_index = -1;

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
  int t =millis();
  if(near(note, 0)) return;

  if(note_index >= 29){
    for(int i = 1; i < 30; i++){
      note_buffer[i - 1] = note_buffer[i];
    }
    note_index = 28;
  }

  note_index++;
  note_buffer[note_index].time = ntime / last_note.time;
  note_buffer[note_index].pitch = note / last_note.pitch;

  last_note.time = ntime;
  last_note.pitch = note;

  note_buffer[0] = {1, 1};
  
  //Serial.print(note);
  //Serial.print(" : ");
  //Serial.println(ntime);
  //Serial.print(note_buffer[note_index].pitch);
  //Serial.print(" : ");
  //Serial.println(note_buffer[note_index].time);

  /*for(int i = 0; i < 30; i++){
    Serial.print(note_buffer[i].time);
    Serial.print(" ");
  }*/

  double score = dist(hot_cross_buns, 
                      note_buffer, 
                      17,//sizeof(hot_cross_buns)/sizeof(struct Note),
                      30, //sizeof(note_buffer)/sizeof(struct Note),
                      0.5);
  Serial.println(score);
  if(score < 12.0){
    set_color(0, 255, 0);
    /*for(int i = 0; i < 30; i++){
      note_buffer[i] = {0, 0};
    }*/
    
  }
  else{
    set_color(255, 0, 0);
  }

  Serial.println(millis() -t);
  
}

void loop() { 
    
    //Note just ended into silence
    if(silence_buffer == 1){
      end_note(prev_note, millis() - prev_time);
    }
    
    //No note currently playing
    if(silence_buffer >= 1){
      prev_note = 0;
      prev_time = millis();
      //set_color(0, 0, 0);
    }

    freq = freq_listen();
    note = freq_to_note(freq);
    //Serial.println(note);

    
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

      //display_note(note);
      prev_note = note;
      prev_time = millis();
    }
      

  delay(5); //keeps the LED from flickering
  
}
