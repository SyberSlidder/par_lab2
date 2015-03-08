#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/* a = B*c  B \in R M x N*/
#define M 10000
#define N 40000

/* ********************************************************************** */
void mxv(int m, int n, double *a, double *B, double *c){
  int i,j;

#pragma omp parallel for default(none) shared(m,n,a,B,c) private(i,j)    
  for (i=0; i<m; i++){
	 a[i]=0.0;
	 for (j=0; j<n; j++) a[i] += B[i*n+j]*c[j];
  }
}


/* ********************************************************************** */
int main( int argc, char *argv[]){
  double a[M], B[M*N], c[N];
  int n=N, m=M;
  int i,j;

  // omp_set_num_threads(2);

  // initialize
  for(j=0;j<n;j++)
	 c[j]=2.0;
  for(i=0;i<m;i++)
	 for(j=0;j<n;j++) B[i*n+j]=i;
  
  mxv(m,n,a,B,c);

  return 0;
}
	 
