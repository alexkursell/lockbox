#define MIC_AVG 360
#define MIC_DELTA 25

int freq_raw;
double freq;
double note;
int note_index = 0;
double notes[128];
int silence_buffer = 0;
int score = 0;

//int master_song[] = {45, 45, 50, 49, 52, 51, 53, 53, 53, 49, 50, 50, 53, 53, 54, 56, 0};
//int master_song[] = {42, 46, 45, 45, 46, 45, 50, 50, 50, 50, 50, 48, 48, 48, 53, 46};
//int master_song[] = {0, 0, 5, 5, 0, 5, 7, 7, 9, 9, 7, 7, 8, 9, 11, 4};//grenadiers' march (new)
int master_song[] = {0, 0, 0, 0, 6, 6, 6, 7, 7, 7, 9, 9, 9, 9, 7, 7}; //twinkle twinkle little star
int master_song_size = 16;//sizeof(master_song)/sizeof(int);

int this_song[16];

bool caught = false; //stop box from liking arpeggios and single note-tunes

void setup() {
  Serial.begin(9600);
  Serial.println("start");
  freq_init();
  led_init();
  //relativize(master_song,master_song_size);
}

void loop() {
  freq_raw = get_freq();
  if (freq_raw != 0) {
    set_color(0,0,255);
    silence_buffer = 0;
    note = freq_to_note(freq_raw); 
    if (note_index < 128) {
      notes[note_index] = note;
      note_index++;
    }
    else {
      flush_notes();
      note_index = 0;
    }
    Serial.println(note); 
  }
  else{ 
    silence_buffer++;
  }
  if (silence_buffer>25 && notes[0] != 0) {
    caught = false;
    process(notes,note_index);
    relativize(this_song,master_song_size);
    score = nw_compare(this_song,master_song);
    Serial.println(score);
    if (score > 10) {
      set_color(0,255,0);
      open_box();
      delay(1000);
      set_color(0,0,0);
    }
    else {
      set_color(255,0,0);
      lock_box();
      delay(1000);
      set_color(0,0,0);
    }
    flush_notes();
    note_index = 0;
    silence_buffer = 0;
  }
  delay(0);
}

