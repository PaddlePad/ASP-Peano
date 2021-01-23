#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern void peano(unsigned degree, u_int64_t *x, u_int64_t *y);
extern void reverse(u_int64_t *in, u_int64_t *out, unsigned size);
extern void mirror(u_int64_t *in, u_int64_t *out, unsigned size);

void printCurve(int length, u_int64_t *curve)
{
    for (int i = 0; i < length; i++)
    {
        printf("%ld, ", curve[i]);
    }

    printf("\n");
}

int main(int argc, char **argv)
{
    // Avoid warning about unused parameter.
    (void)argc;
    (void)argv;

    int length = 8;
    u_int64_t curr1[] = {0,0,1,2,2,1,0,0}; //0 : up , 1 : right , 2 : down , 3 : left , Kurve mit grad 1
    // u_int64_t *curr1 = (u_int64_t *)malloc(length * sizeof(u_int64_t));
    // curr1[0] = 0;
    // curr1[1] = 0;
    // curr1[2] = 1;
    // curr1[3] = 2;
    // curr1[4] = 2;
    // curr1[5] = 1;
    // curr1[6] = 0;
    // curr1[7] = 0;
    u_int64_t *rev = (u_int64_t *)malloc(length * sizeof(u_int64_t));
    u_int64_t *mirr = (u_int64_t *)malloc(length * sizeof(u_int64_t));

    printCurve(length, curr1);
    reverse(curr1, rev, length);
    printCurve(length, rev);
    mirror(curr1, mirr, length);
    printCurve(length, mirr);
    free(rev);
    //free(curr1);
    free(mirr);

    return 0;
}
