#include "omp.h"
#include <stdio.h>
#include <unistd.h>
int main() {
    double start = omp_get_wtime( );
    usleep(100000);
    double end = omp_get_wtime( );
    double wtick = omp_get_wtick( );

    printf("start = %.16g\nend = %.16g\ndiff = %.16g\n",
             start, end, end - start);

    printf("wtick = %.16g\n1/wtick = %.16g\n",
             wtick, 1.0 / wtick);
}
