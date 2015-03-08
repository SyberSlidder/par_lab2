#include <stdlib.h>
#include <time.h>


void getCPUTime(double *t){
  *t = ((double)clock()) / ((double)CLOCKS_PER_SEC);
}  


int main(void){
  int i;
  double ts,te;
  
  getCPUTime(&ts);
  for (i=0;i<100000000;i++);
  getCPUTime(&te);

  printf("%g secs\n", te-ts);
}









