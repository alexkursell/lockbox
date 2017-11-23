//Inverse of log(2). Used to change log base in frequency conversion function.
const int l_inverse_2 = 1.0/log(2.0);

/*
 * Take a frequency value in Hz and convert it to a note.
 */
double freq_to_note(double freq){
    //440Hz is standard A note, 49 scales everthing to be >= 0
    return 12.0 * log(freq/440)*l_inverse_2+49;
}

/*
 * Takes integers a, b, c. Return which of b and c is closer to a.
 */
int closest(int a, int b, int c) {
  return fabs(a-b) < fabs(a-c) ? b : c;
}

/*
 * Calculates a "distance" between two integers.
 */
int match(int a, int b) {
  if (a-b == 0) return 5;
  return fabs(a-b)*-1+1;
}

/*
 * Just returns max of 3 integers.
 */
int max_of_3(int a, int b, int c) {
  int max = a;
  if (b>max) max = b;
  if (c>max) max = c;
  return max;
}

/*  
 * Implementation of the Needleman-Wunsch algorithm  
 * See https://en.wikipedia.org/wiki/Needleman%E2%80%93Wunsch_algorithm.  
 * Finds "distance" between two sequences of integers.  
*/
int nw_compare(int a[], int b[]) {

  int len_a = MASTER_SONG_SIZE;
  int len_b = MASTER_SONG_SIZE;
  char sim[len_a+1][len_b+1];

  //Fill first row of matrix
  for (int i=1;i<len_a+1;i++) sim[i][0] = i*SKIP_PENALTY;

  //Fill first column of matrix
  for (int j=0;j<len_b+1;j++) {
    sim[0][j] = j*SKIP_PENALTY;
  }

  //Iterate through each row and column and find the value of each cell.
  for (int i=1;i<len_a+1;i++) {
    for (int j=1;j<len_b+1;j++) {
      sim[i][j] = max_of_3(sim[i-1][j] + SKIP_PENALTY, //Add space to row sequence.
                          sim[i][j-1] + SKIP_PENALTY,  //Add space to column sequence.
                          sim[i-1][j-1] + match(a[i-1], b[j-1])); //Take new note from each.
    }
  }


  if (caught) return -100;
  return sim[len_a][len_b]; //Return last cell in last row
}


/*
 * For every note in the buffer, make it relative to the first note.
 * First note is the "tonic note".
 */
void relativize(int a[],int n) {
  int tonic = a[0];

  for (int i=0;i<n;i++) {
    a[i] -= tonic;
    Serial.print(a[i]);
    Serial.print(", ");
  }
  /*for (int i=0; i<n;i++) {
    a[i]-=a[i+1s];
  }*/
}

void process(double notes[], int old_sum) {
  printf("start\n");
  Serial.println(old_sum);


  double new_notes[128];
  int new_lengths[128];
  int j=0;
  int length = 0;
  int new_sum = 0;
  
  for (int i=0;i<old_sum;i++) {
    if (i !=0 && notes[i] != notes[i+1] && notes[i] != notes[i-1]) {
      notes[i] = closest(notes[i],notes[i-1],notes[i+1]);
    //printf("%g %g\n",notes[i],closerst(notes[i],notes[i-1],notes[i+1]));
    }
    else if (i == 0 && notes[i] != notes[i+1]) notes[i] = notes[i+1];
    else if (i == old_sum-1 && notes[i] != notes[i-1]) notes[i] = notes[i-1];
    if (i != 0 && notes[i] != notes[i-1] || i == old_sum-1) {
    new_notes[j] = notes[i-1];
      if (i==old_sum-1) length++;
      new_lengths[j] = length;
      length = 0;
      j++;
     }
    length++;
    //Serial.println(round(notes[i]));
  }
  if (j < 8) {
    caught = true;
    Serial.println("caught!");
    return;
  }
  for (int i=0;i<j;i++) {
    new_lengths[i] = ((double)(new_lengths[i])/((double)(old_sum))*MASTER_SONG_SIZE);
  }

  new_sum = j;
  j=0;
  for (int i=0;i<MASTER_SONG_SIZE;i++) {
    this_song[i] = new_notes[j]; 
    new_lengths[j]-=1;
    if (new_lengths[j] <= 0) j++;
  if (j>=new_sum) j--;
  }
  for (int i=0;i<MASTER_SONG_SIZE;i++) {
    //Serial.print(this_song[i]);
    //Serial.print(", ");
  }
}

/*
 * Get rid of all stored notes.
 */
void flush_notes() {
  Serial.println("reset");
  for (int i=0;i<128;i++) notes[i] = 0.0;
}
