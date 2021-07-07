 
#include <stdio.h>
#include <omp.h>


int main(int argc, char* argv[]) {
    printf("Anzahl Prozessoren: %d\n", omp_get_num_procs());
    #pragma omp parallel
    { 
        int id = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        printf("hello(%d/%d)\n", id, nthreads);
        printf("world(%d/%d)\n", id, nthreads);
    }
    printf("Fertig.\n"); 
    return 0;
}