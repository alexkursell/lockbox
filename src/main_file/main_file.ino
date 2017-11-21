/*
 * Main file, contains the "glue logic" that uses the fuctions from the libraries.
 */

//FHT library constants
#define LIN_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht
#define SCALE 128 // set scaling to 128

//Library includes
#include <FHT.h> //To perform a Fast Hartley Transform on the microphone samples
#include <Servo.h> //To open and close the lock
#include <math.h> //For log function and sin function (note conversion and display)

//Define hardware locations. Just defines the pin for various perhipherals.
#define SERVO_PIN 3
#define MIC_PIN A0
#define LED_R_PIN 5
#define LED_G_PIN 9
#define LED_B_PIN 11

#define MIC_AVG 360
#define MIC_DELTA 25

//Servo position constants
#define LOCK_POSITION 2000 //PWM value to send to servo for lock position.
#define OPEN_POSITION 1200 //PWM value to sent to servo for open position.

//Frequency detection constants
#define MIN_FREQ_BIN 15 //Ignore readings with a too-low frequency.
#define MIN_FREQ_INTENSITY 400 //Ignore readings with weak frequency intensity.
#define BIN_TO_FREQ_RATIO 17.6 //Converts frequency bin index to Hz.
#define MAXIMUM_SILENT_SAMPLES 1 //See loop().
#define PITCH_VARIATION_TOLERANCE 1 //How much can pitch change before it's a new note?
#define NOTE_EPSILON 0.1 //For testing equality of note doubles.

//Sequence comparaison constants
#define SKIP_REFERENCE_NOTE_COST 0.5 //Cost of skipping a reference note (Option 1)
#define SKIP_TEST_NOTE_COST 0.3 //Cost of skipping a test note (Option 2)
#define PITCH_DIFFERENCE_COST_MULTIPLIER 25 //Relative importance of pitch difference
#define TIME_DIFFERENCE_COST_MULTIPLIER 1 //Relative importance of time difference
#define ACCEPTABLE_DISTANCE 10.0 //Maximum acceptable distance for lock to open.

//Sequence sizes
#define NOTE_BUFFER_SIZE 30 //We store the last n notes that were registered.


//Just contains a pitch and time value.
struct Note{
  double pitch;
  double time;
};

//Create buffer for the last NOTE_BUFFER_SIZE notes
//that were registered by the microphone
struct Note init_note_buffer[NOTE_BUFFER_SIZE];
struct Note *note_buffer = (struct Note *) init_note_buffer;
struct Note last_note = {1, 1};

//Reference sequence. In this case, Hot Cross Buns
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

const int REFERENCE_SEQUENCE_SIZE = sizeof(inita) / sizeof(struct Note);

//Take a pointer to the sequence
struct Note *reference_sequence = (struct Note*) inita;


//Position in buffer to append new notes
//1 is added every time before use.
int note_index = -1;


void setup() {
  //Initialize some of the hardware components.
  led_init();
  freq_init();

  //Even if we aren't printing anything, this seems to solve a tricky
  //timing issue that was affecting out frequency detection.
  Serial.begin(9600); 
  
  set_color(255, 255, 255); //White to show that the box is starting.
  
}

/*
 * Tests equality of two note doubles.
 */
bool near(double a, double b){
  return abs(a - b) < NOTE_EPSILON;
}


/*
 * Called when the end of a note is detected. Updates the note buffer and checks
 * for a match with the reference sequence. 
 * If one is found, perform the opening actions.
 */
void end_note(double note, double ntime){
  if(near(note, 0)) return; //Don't do anything for a non-note.

  //Move everything in the buffer back by 1.
  //Delete the oldest entry. Only do this if the buffer is full.
  if(note_index >= NOTE_BUFFER_SIZE - 1){
    for(int i = 1; i < NOTE_BUFFER_SIZE; i++){
      note_buffer[i - 1] = note_buffer[i];
    }

    //(NOTE_BUFFER_SIZE - 2)++ = NOTE_BUFFER_SIZE - 1
    //, so the note_index will always be 
    //the last position in the buffer after it fills up
    note_index = NOTE_BUFFER_SIZE - 2;
  }

  note_index++;

  //Make the pitch and time relative to the previous note.
  //This adjusts for different keys and tempos.
  note_buffer[note_index].time = ntime / last_note.time;
  note_buffer[note_index].pitch = note / last_note.pitch;

  //Save the actual time and pitch of the last note for the next division.
  last_note.time = ntime;
  last_note.pitch = note;

  //The first note in the buffer is always relative to itself.
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

  //Get the minimum distance between the buffer and the reference sequence
  double score = dist(reference_sequence, 
                      note_buffer, 
                      REFERENCE_SEQUENCE_SIZE,
                      NOTE_BUFFER_SIZE
                      );


  //Serial.println(score);

  //If the distance is small enough, perform the opening actions.
  if(score < ACCEPTABLE_DISTANCE){

    //Go green, open box.
    set_color(0, 255, 0);
    open_box();

    //Flush the buffer.
    for(int i = 0; i < NOTE_BUFFER_SIZE; i++){
      note_buffer[i] = {0, 0};
    }
  }

  //Otherwise make sure the box is locked.
  else{
    lock_box();
    set_color(255, 0, 0); //Show red
  }
  
}

//Store currently playing note
double note;

//Store last note and time
double prev_note = 0;
double prev_time = millis();

//How many silent samples have we had since the last non-silent one?
int silence_buffer = 0;

void loop() { 
    //Note just ended into silence
    if(silence_buffer == MAXIMUM_SILENT_SAMPLES){
      end_note(prev_note, millis() - prev_time);
    }
    
    //No note currently playing
    if(silence_buffer >= MAXIMUM_SILENT_SAMPLES){
      prev_note = 0;
      prev_time = millis();
    }

    //Get new note 
    note = freq_to_note(freq_listen());

    //If no note detected
    //These constants are because the process of printing data over the serial
    //line causes voltage fluctuations that can register as one of the two notes
    //below. So we ignore them.
    if(near(note, 0) || near(note, 74.40) || near(note, 66.08)){
      silence_buffer++;
      note = prev_note;
    }

    //If note detected
    else{
      silence_buffer = 0;
    }
    
    //If note just switched or started
    if(abs(note - prev_note) > PITCH_VARIATION_TOLERANCE){
      
      //Note just switched
      if(!near(prev_note, 0.0)){
        end_note(prev_note, millis() - prev_time);
      }

      prev_note = note;
      prev_time = millis();
    }
      

  delay(5); //keeps the LED from flickering 
}
