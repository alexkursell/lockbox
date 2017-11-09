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


void freq_init() {
}

int freq_listen(){
  while(1) { // reduces jitter
    for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
      fht_input[i] = analogRead(MIC_PIN);
    }
    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_lin(); // take the output of the fht
    int id = -1;

    

    for(int i = 4; i < 128; i++){
     // Serial.print(i);
      //Serial.print(" : ");
      //Serial.println(fht_lin_out[i]);
      id = fht_lin_out[i] > fht_lin_out[id] ? i : id; 
    }

    int val = fht_lin_out[id];
    
    if(id >= 20){
      //Serial.println(val);
      return id;
    }
  }
}

