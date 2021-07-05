 
#include <stdio.h>
#include <omp.h>


int main(int argc, char* argv[]) {
    printf("Anzahl Prozessoren: %d\n", omp_get_num_procs());
    #pragma omp parallel
    { 
        printf ("Thread %d von %d sagt \“Hallo Welt!\“\n", omp_get_thread_num(), omp_get_num_threads());
    }
    printf("Fertig.\n"); 
    return 0;
}