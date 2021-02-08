#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    u_int64_t i = atol(argv[1]);
    
    printf("%f\n", i * 1e-9 + 0.02); 


    // u_int64_t i = 1;
    // u_int64_t* v;
    // while(1)
    // {
    //     v = (u_int64_t *) malloc(i);
    //     if(v != NULL)
    //          printf("%ld\n", i);
    //     else
    //         break;

    //     i <<= 1;
    //     free(v);
    // }

    // u_int64_t* v;
    // for (u_int64_t i = 1; v = (u_int64_t *) malloc(i); i <<= 1)
    // {
    //     printf("%ld\n", i);
    //     free(v);
    // }
    return 0;
}