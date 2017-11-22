
const int l_inverse_2 = 1.0/log(2.0);

double log_2(double n){
    return n == 0 ? 0 : log(n) * l_inverse_2;
}

double freq_to_note(double freq){
    return 12.0 * log(freq/440)/log(2)+49;
}

int closest(int a, int b, int c) {
  return fabs(a-b) < fabs(a-c) ? b : c;
}

int round_a_double(int x) {
    return (int)(x+0.5);
}

int match(int a, int b) {
  if (a-b == 0) return 5;
  return fabs(a-b)*-1+1;
}

int get_max(int a, int b, int c) {
  int max = a;
  if (b>max) max = b;
  if (c>max) max = c;
  return max;
}

int nw_compare(int a[], int b[]) {
  
  int m = 1;
  int n = -1;
  int d = -3;

  int len_a = master_song_size;
  int len_b = master_song_size;
  char sim[len_a+1][len_b+1];

  for (int i=1;i<len_a+1;i++) sim[i][0] = i*d;

  for (int j=0;j<len_b+1;j++) {
    sim[0][j] = j*d;
  }

  for (int i=1;i<len_a+1;i++) {
    for (int j=1;j<len_b+1;j++) {
      sim[i][j] = get_max(sim[i-1][j]+d,sim[i][j-1]+d,sim[i-1][j-1]+match(a[i-1],b[j-1]));
    };
  }
  if (caught) return -100;
  return sim[len_a][len_b];
}

void relativize(int a[],int n) {
  int tonic = a[0];
  for (int i=0;i<n;i++) {
    a[i]-=tonic;
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
    //Serial.println(round_a_double(notes[i]));
  }
  if (j < 8) {
    caught = true;
    Serial.println("caught!");
    return;
  }
  for (int i=0;i<j;i++) {
    new_lengths[i] = ((double)(new_lengths[i])/((double)(old_sum))*master_song_size);
  }

  new_sum = j;
  j=0;
  for (int i=0;i<master_song_size;i++) {
    this_song[i] = new_notes[j]; 
    new_lengths[j]-=1;
    if (new_lengths[j] <= 0) j++;
  if (j>=new_sum) j--;
  }
  for (int i=0;i<master_song_size;i++) {
    //Serial.print(this_song[i]);
    //Serial.print(", ");
  }
}

void flush_notes() {
  Serial.println("reset");
  for (int i=0;i<128;i++) notes[i] = 0.0;
}
