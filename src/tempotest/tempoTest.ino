/* Detects patterns of knocks and move the servo to unlock if the pattern is correct.
   added on tempo adjustments
   
   Reference from: Steve Hoefer http://grathio.com
   Version 0.1.10.20.10
   Licensed under Creative Commons Attribution-Noncommercial-Share Alike 3.0
   http://creativecommons.org/licenses/by-nc-sa/3.0/us/
   (In short: Do what you want, just be sure to include this line and the four above it, and don't sell it or use it in anything you sell without contacting me.)
*/

//servo position definitions
#define LOCK_POSITION 2000
#define OPEN_POSITION 1200
 
// Pin definitions
#define SERVO_PIN 3//for servo library
const int knockSensor = A0;           // Microphone on pin A0.
const int servoMotor = 3;             // Gear motor used to turn the lock.
const int redLED = 5;                 // Red LED for warning
const int greenLED = 9;               // Green LED for displaying the pattern and showing correctness
 
// Tuning constants
const int threshold = 420;            // Minimum signal from the microphone to register as a knock
const int rejectValue = 25;           // If an individual knock is off by this percentage of a knock we don't unlock..
const int averageRejectValue = 15;    // If the average timing of the knocks is off by this percent we don't unlock.
const int knockFadeTime = 100;        // Milliseconds we allow a knock to fade before we listen for another one. (Debounce timer.)
const int lockTurnTime = 100;         // Milliseconds that we run the servo to get it into position.
const int numKnocks = 7;              // Number number of knocks to listen for.
const int knockComplete = 3000;       // Longest time to wait for a knock before we assume that it's finished.

// Variables.
int secretCode[numKnocks] = {50, 25, 25, 50, 100, 50, 0};   // Initial setup: "Shave and a Hair Cut, two bits."
int knockReadings[numKnocks];                               // When someone knocks this array fills with delays between knocks.
int knockSensorValue = 0;                                   // Last reading of the knock sensor.
int is_locked = false;                                      // Flag, indicates if the box is locked for now, present status

void setup() {
  pinMode(servoMotor, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  
  //Serial.begin(9600);                  
  //Serial.println("Program start.");      
  
  digitalWrite(greenLED, HIGH);             // Green LED on, starts running the code.
}

void loop() {
  // Listen for any knock in general.
  knockSensorValue = analogRead(knockSensor);

  //Call the function to record the timing when a knock is detected
  if (knockSensorValue >=threshold){
    listenToSecretKnock();
  }
} 

// Records the timing of knocks.
void listenToSecretKnock(){
  //Serial.println("knock starting"); 

  //initialization
  int i = 0;//index counter
  for (i=0;i<numKnocks;i++){
    knockReadings[i]=0;                      //Reset the listening array, all elements are initialized to zero
  } 
  int currentKnockNumber=0;                 // Incrementer for the array.
  int startTime=millis();                   // Reference for when this knock started.
  int now=millis();
  
  digitalWrite(greenLED, LOW);              // Blink the LED for a bit as a visual indicator of the knock.
  delay(knockFadeTime);                     // Wait for this peak to fade before we listen to the next one.
  digitalWrite(greenLED, HIGH);  
  do {                                      // Listen for the next knock or wait for it to timeout. (Goes directly to analysis when time out.)
    knockSensorValue = analogRead(knockSensor);
    if (knockSensorValue >=threshold){      // Got another knock.
      //record the delay time.
      //Serial.println("knock.");
      now=millis();
      knockReadings[currentKnockNumber] = now-startTime;
      Serial.println(knockReadings[currentKnockNumber]);
      currentKnockNumber ++;                // Increment the counter.
      startTime=now;          
      // and reset our timer for the next knock
      digitalWrite(greenLED, LOW);  
      delay(knockFadeTime);                 // Again, a little delay to let the knock decay.
      digitalWrite(greenLED, HIGH);
    }

    now=millis();
    
  } while ((now-startTime < knockComplete) && (currentKnockNumber < numKnocks)); //Check if timeout or if knocks has been runout.
  
  //See if the recorded knock is valid
    if (validateKnock() == true){
      boxUnlock();                       // If valid, unlock (or lock) the box
    } 
    else {                                // Not valid, blinking red LED for warning.
      //Serial.println("Secret knock failed.");
      digitalWrite(greenLED, LOW);      
      for (i=0;i<4;i++){          
        digitalWrite(redLED, HIGH);
        delay(100);
        digitalWrite(redLED, LOW);
        delay(100);
      }
      digitalWrite(greenLED, HIGH);
    }
}


// Runs the servo to unlock (or lock) the box.
void boxUnlock(){
  Serial.println("box unlocked!");
  int i=0;

  //use servo libary
  servo_init();
  if (is_locked){
    open_box();
    is_locked = false;
  }
  else{
    lock_box();
    is_locked = true;
  }
  
  // Blink the green LED a few times for correct unlocking/locking
  for (i=0; i < 5; i++){   
      digitalWrite(greenLED, LOW);
      delay(100);
      digitalWrite(greenLED, HIGH);
      delay(100);
  }
   
}

// Sees if detected knock matches the secret.
// returns true if it's a good knock, false if it's not.
boolean validateKnock(){
  int i=0;
  int currentKnockCount = 0;
  int secretKnockCount = 0;
  int maxKnockInterval = 0;                       // We use this later to normalize the times.
  
  // simplest check first: Did we get the right number of knocks?
  for (i=0;i<numKnocks;i++){
    if (knockReadings[i] > 0){
      currentKnockCount++;
    }
    if (secretCode[i] > 0){           
      secretKnockCount++;
    }
    
    if (knockReadings[i] > maxKnockInterval){     // collect normalization data while we're looping.
      maxKnockInterval = knockReadings[i];
    }
  }  
  if (currentKnockCount != secretKnockCount){
    return false; 
  }
  
  /*  Now compare the relative intervals of two knocks, not the absolute time between them.
      (ie: if you do the same pattern slow or fast it should still open the box.)
  */
  int totaltimeDifferences=0;
  int timeDiff=0;
  for (i=0;i<numKnocks;i++){ // Normalize the times
    knockReadings[i]= map(knockReadings[i],0, maxKnockInterval, 0, 100);      
    timeDiff = abs(knockReadings[i]-secretCode[i]);
    if (timeDiff > rejectValue){ // Individual value too far out of whack
      return false;
    }
    totaltimeDifferences += timeDiff;
  }
  // It can also fail if the whole thing is too inaccurate.
  if (totaltimeDifferences/secretKnockCount>averageRejectValue){
    return false; 
  }
  return true; 
}
