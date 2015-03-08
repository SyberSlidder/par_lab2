template <typename T> inline T reduction( const T *a, const T *b, long int n){
  long int i = 0;
  T sum = 0;
  for(i=0; i<n; i++)  sum+=a[i]*b[i];
  return sum;
}
