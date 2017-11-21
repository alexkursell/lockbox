/*
 * Freqency measuring library, contains functions for sampling audio and finding its frequency.
 * This code is adapted from http://wiki.openmusiclabs.com/wiki/FHTExample
 */


/*
 * Sets up analog input registers for faster input.
 */
void freq_init() {
  ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2));
  ADCSRA |= bit (ADPS0) | bit (ADPS2) | (1 << ADSC) | (1 << ADEN);
  ADCSRA |= (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADSC); //start ADC measurements
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}


/*
 * When called, reads samples from microphone, 
 * performs FHT to find frequency distribution,
 * finds dominant frequency and either returns that frequency
 * or 0.0 if none is found.
 */
double freq_listen(){
  int minl;
  int maxl;
  
  //Loop reads 256 samples from microphone.
  //Uses registers directly for faster sampling.
  for (int i = 0 ; i < FHT_N ; i++) {
    while(!(ADCSRA & 0x10));
    ADCSRA |= bit (ADPS0) | bit (ADPS1) | bit (ADPS2) | (1 << ADSC) | (1 << ADEN);
    byte m = ADCL; // fetch adc data
    byte j = ADCH;

    int k = (j << 8) | m; // form into an int
    k -= 0x0200; // form into a signed int
    k <<= 6; // form into a 16b signed int

    minl = min(minl, k);
    maxl = max(maxl, k);
    
    fht_input[i] = k; // put real data into bins
  }

  fht_window(); // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run(); // process the data in the fht
  fht_mag_lin(); // take the output of the fht

  //Find the index of the most dominant frequency bin.
  //Starts at 4 because very low frequencies always have massive intensities.
  int id = -1;
  for(int i = 4; i < 128; i++){
   //Serial.print(i);
   //Serial.print(" : ");
   //Serial.println(fht_lin_out[i]);
    id = fht_lin_out[i] > fht_lin_out[id] ? i : id; 
  }

  //Frequency intensity of dominant frequency.
  int val = fht_lin_out[id];
  
  //Check against some conditions to avoid false positives.
  if(id >= MIN_FREQ_BIN && val > MIN_FREQ_INTENSITY){
    return ((double) id) * BIN_TO_FREQ_RATIO;
  }
  else return 0;
}



//Inverse of log(2). Used to change log base in frequency conversion function.
const int l_inverse_2 = 1.0/log(2.0);

/*
 * Log 2 function.
 * Modified so that 0 maps to 0.
 */
double log_2(double n){
    return n == 0 ? 0 : log(n) * l_inverse_2;
}

/*
 * Convert frequency in Hz to a "note".
 * Formula is note = 12*log_2(frequency).
 */
double freq_to_note(double freq){
    return 12.0 * log_2(freq);
}




