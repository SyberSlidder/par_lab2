void axpy_core( float a, float *y, float *x, int n){
	int i;
	for (i=0;i<n;i++){
		x[i] += a*x[i] + y[i];
	}
}
