/*
 * Frequency testing library. 
 * Contains functions to compare frequency samples and 
 * detemine if they are the same tune.
 */

double initold[31]; //= (double*) malloc(sizeof(double) * 31);
double initnew[31]; //= (double*) malloc(sizeof(double) * 31);


double dist(struct Note *s1, struct Note *s2, int n1, int n2, double skip_penalty){

    double *olda = (double *) initold;
    double *newa = (double * ) initnew;
    
    double *tmp;
    
    double omin;
    double nmin;

    for(int i = 0; i < n2 + 1; i++){
        olda[i] = (double) i;
    }

    
    for(int x = 0; x < n1; x++){
        newa[0] = (double) (x + 1);
        for(int y = 1; y < n2 + 1; y++){
            omin = olda[y] + skip_penalty;
            
            nmin = newa[y - 1] + 0.3;
            omin = omin <= nmin ? omin : nmin;
            
            nmin = olda[y - 1] + comp(s1[x], s2[y - 1]);
            omin = omin <= nmin ? omin : nmin;

            newa[y] = omin;
        }
        
        tmp = olda;
        olda = newa;
        newa = tmp;
    }

    double ans = olda[n2];
    return ans;
}


double comp(struct Note a, struct Note b){
  double diffpitch = a.pitch - b.pitch;
  double difftimes = a.time - b.time;
  //Serial.println(diffpitch * 100);
  return fabs(diffpitch) * 100 + fabs(difftimes);
}
