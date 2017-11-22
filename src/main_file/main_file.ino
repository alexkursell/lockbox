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

//Servo position constants
#define LOCK_POSITION 2000 //PWM value to send to servo for lock position.
#define OPEN_POSITION 1200 //PWM value to sent to servo for open position.

//Frequency detection constants
#define MIN_FREQ_BIN 15 //Ignore readings with a too-low frequency.
#define MIN_FREQ_INTENSITY 400 //Ignore readings with weak frequency intensity.
#define BIN_TO_FREQ_RATIO 17.6 //Converts frequency bin index to Hz.
#define MAXIMUM_SILENT_SAMPLES 25 //See loop().

//Sequence comparaison constants
#define SKIP_REFERENCE_NOTE_COST 0.5 //Cost of skipping a reference note (Option 1)
#define SKIP_TEST_NOTE_COST 0.3 //Cost of skipping a test note (Option 2)
#define PITCH_DIFFERENCE_COST_MULTIPLIER 25 //Relative importance of pitch difference
#define TIME_DIFFERENCE_COST_MULTIPLIER 1 //Relative importance of time difference
#define ACCEPTABLE_DISTANCE 10.0 //Maximum acceptable distance for lock to open.

//Defining colors for better readability
#define COLOR_RED 255,0,0
#define COLOR_GREEN 0,255,0
#define COLOR_BLUE 0,0,255
#define COLOR_NONE 0,0,0
#define COLOR_WHITE 255,255,255

//Sequence sizes
#define NOTE_BUFFER_SIZE 128 //We store the last n notes that were registered.

#define MIC_AVG 360
#define MIC_DELTA 25

//int master_song[] = {45, 45, 50, 49, 52, 51, 53, 53, 53, 49, 50, 50, 53, 53, 54, 56, 0};
//int master_song[] = {42, 46, 45, 45, 46, 45, 50, 50, 50, 50, 50, 48, 48, 48, 53, 46};
//int master_song[] = {0, 0, 5, 5, 0, 5, 7, 7, 9, 9, 7, 7, 8, 9, 11, 4};//grenadiers' march (new)
int master_song[] = {0, 0, 0, 0, 6, 6, 6, 7, 7, 7, 9, 9, 9, 9, 7, 7}; //twinkle twinkle little star
const int MASTER_SONG_SIZE = sizeof(master_song)/sizeof(int);

int this_song[16];

bool caught = false; //stop box from liking arpeggios and single note-tunes

void setup() {
  Serial.begin(9600);
  Serial.println("start");
  freq_init();
  led_init();
  //relativize(master_song,master_song_size);
}

int freq_raw; //Store raw frequency reading
double note;  //Store the processed note value.
int note_index = 0;
double notes[NOTE_BUFFER_SIZE];
int silence_buffer = 0; //Counts number of samples since we last heard an actual note
int score = 0;

void loop() {
  //Get a new frequency value
  freq_raw = get_freq();

  //If a note was actually detected.
  if (freq_raw != 0) {
    set_color(COLOR_BLUE);
    silence_buffer = 0;

    //Convert the frequency value to an actual note.
    note = freq_to_note(freq_raw); 
    
    //Check if index is within buffer size
    //If it is, add the new note.
    if (note_index < NOTE_BUFFER_SIZE) {
      notes[note_index] = note;
      note_index++;
    }

    //If it isn't flush the buffer.
    else {
      flush_notes();
      note_index = 0;
    }
    Serial.println(note); 
  }

  //If no note detected.
  else{ 
    silence_buffer++;
  }

  //Too much silence means the person is done whistling
  if (silence_buffer > MAXIMUM_SILENT_SAMPLES && notes[0] != 0) {
    caught = false;
    process(notes, note_index);
    relativize(this_song, MASTER_SONG_SIZE);
    score = nw_compare(this_song, master_song);
    Serial.println(score);


    //The input was close enough to the master song.
    if (score > ACCEPTABLE_DISTANCE) {
      set_color(COLOR_GREEN);
      open_box();
      delay(1000);
      set_color(COLOR_NONE);
    }

    //The input failed the test!
    else {
      set_color(COLOR_RED);
      lock_box();
      delay(1000);
      set_color(COLOR_NONE);
    }
    flush_notes();
    note_index = 0;
    silence_buffer = 0;
  }
  delay(0);
}

