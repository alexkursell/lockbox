#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct atm{
  int num_money;
  int money[];
};

int gcd(int a, int b){
  if(a == 1 || b == 1) return 1;
  if(b > a){
    return gcd(a, b % a);
  }
  else{
    return gcd(b, a % b);
  }
}

int obtainable(struct atm *m){
  int g = m.money[0];
  int sm = m.money[0];
  for(int i = 1; i < m.num_money; i++){
    g = gcd(g, m.money[i]);
    sm = sm < m.money[i] ? sm : m.money[i];
  }

  if(g != 1) return -1;

  int streak = 0;
  bool pos[10000] = {false};

  for(int i = 0; i < m.num_money; i++){
    pos[i] = true;
  }

  for(int i = 0; i < 10000; i++){
    if(pos[i]){
      for(int j = 0; j < m.num_money; j++){
        if(i + m.money[j] < 10000){
          pos[i + m.money[j]] = true;
        }
      streak++;
    }
    else{
      streak = 0;
    }

    if(streak == sm){
      return i - sm;
    }

  }
  assert(false);
}

int main(void){
  int n = 3;
  struct atm *machine = malloc(sizeof(struct atm)
             + n * sizeof(int));
  assert(machine);
  machine -> num_money = n;
  machine -> money[0] = 4;
  machine -> money[1] = 5;
  machine -> money[2] = 6;
  printf("%d\n",obtainable(machine));
  free(machine);
  struct atm *machine2 = malloc(sizeof(struct atm)
             + 1 * sizeof(int));
  assert(machine2);
  machine2 -> num_money = 1;
  machine2 -> money[0] = 2;
  printf("%d\n",obtainable(machine2));
  machine2 -> money[0] = 1;
  printf("%d\n",obtainable(machine2));
  free(machine2);
  return 0;
}