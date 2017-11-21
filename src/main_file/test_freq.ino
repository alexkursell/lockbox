/*
 * Frequency testing library. 
 * Contains functions to compare frequency samples and 
 * detemine if they are the same tune.
 */

double initold[31]; //= (double*) malloc(sizeof(double) * 31);
double initnew[31]; //= (double*) malloc(sizeof(double) * 31);


/*
 * Custom implementation of the Needleman-Wunsch algorithm
 * See https://en.wikipedia.org/wiki/Needleman%E2%80%93Wunsch_algorithm.
 * Finds "distance" between two sequences of Notes.
 */
double dist(struct Note *s1, struct Note *s2, int n1, int n2){

    //Make pointers to stack array to allow swapping them.
    double *olda = (double *) initold;
    double *newa = (double * ) initnew;
    
    double *tmp; // Used for switching old and new.
    
    //Old and new minimums for use inside loop.
    double omin;
    double nmin;

    //Initialize first row.
    for(int i = 0; i < n2 + 1; i++){
        olda[i] = (double) i;
    }

    
    //Iterate over all rows.
    for(int x = 0; x < n1; x++){
        newa[0] = (double) (x + 1);

        //For each entry, compute the new minimum distance
        //This is the minimum of 3 things:
        for(int y = 1; y < n2 + 1; y++){

            //Option 1: Skip a note in the reference sequence
            omin = olda[y] + SKIP_REFERENCE_NOTE_COST;
            
            //Option 2: Skip a note in the sequence being tested
            nmin = newa[y - 1] + SKIP_TEST_NOTE_COST;
            omin = omin <= nmin ? omin : nmin;

            //Option 3: Match the xth note in the reference sequence with
            //the (y - 1)th note in the sequence being tested.
            //In this case, the penalty is the "distance" between the two notes.
            nmin = olda[y - 1] + comp(s1[x], s2[y - 1]);
            omin = omin <= nmin ? omin : nmin;

            //Set the minimum for this position
            newa[y] = omin;
        }
        
        //Switch old and new arrays
        tmp = olda;
        olda = newa;
        newa = tmp;
    }

    //Return the last item in the last row
    //As that is the total distance.
    return olda[n2];
}


//Returns a measure of "distance" betwen two notes
//Takes into account their durations and pitches
double comp(struct Note a, struct Note b){
  double diffpitch = a.pitch - b.pitch;
  double difftimes = a.time - b.time;
  
  return fabs(diffpitch) * PITCH_DISTANCE_COST_MULTIPLIER 
       + fabs(difftimes) * TIME_DISTANCE_COST_MULTIPLIER;
}
