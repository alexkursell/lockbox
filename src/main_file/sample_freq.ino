/*
fht_adc.pde
guest openmusiclabs.com 9.5.12
example sketch for testing the fht library.
it takes in data on ADC0 (Analog0) and processes them
with the fht. the data is sent out over the serial
port at 115.2kb.  there is a pure data patch for
visualizing the data.
*/

/*
 * Freqency measuring library, contains functions for sampling audio and finding its frequency.
 */

#define LIN_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht
#define SCALE 128

#include <FHT.h> // include the library


const int l_inverse_2 = 1.0/log(2.0);

void freq_init() {
  ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2));
  ADCSRA |= bit (ADPS0) | bit (ADPS2) | (1 << ADSC) | (1 << ADEN);
  ADCSRA |= (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADSC); //start ADC measurements
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

double freq_listen(){
  int minl;
  int maxl;
  
  while(1) {
    for (int i = 0 ; i < FHT_N ; i++) {
      while(!(ADCSRA & 0x10));
      ADCSRA |= bit (ADPS0) | bit (ADPS1) | bit (ADPS2) | (1 << ADSC) | (1 << ADEN);
      byte m = ADCL; // fetch adc data
      byte j = ADCH;

      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int

      //minl = min(minl, k);
      //maxl = max(maxl, k);
      
      fht_input[i] = k; // put real data into bins
    }

    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_lin(); // take the output of the fht
    int id = -1;

    //Serial.println(maxl - minl);

    

    for(int i = 4; i < 128; i++){
     //Serial.print(i);
     //Serial.print(" : ");
     //Serial.println(fht_lin_out[i]);
      id = fht_lin_out[i] > fht_lin_out[id] ? i : id; 
    }

    int val = fht_lin_out[id];
    //Serial.print("val :");
    
    if(id >= 13 && val > 200 /*&& maxl - minl > 11000*/){
      return ((double) id) * 17.6;
    }
    else return 0;
  }
}

double log_2(double n){
    return n == 0 ? 0 : log(n) * l_inverse_2;
}

double freq_to_note(double freq){
    return 12.0 * log_2(freq);
}




