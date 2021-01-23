#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void peano(unsigned degree, uint64_t* x, uint64_t* y);
void reverse(u_int64_t in, u_int64_t out, unsinged size);
void mirror(u_int64_t in, u_int64_t out, unsinged size);

void printCurve(int length, int* curve)
{
    for (int i = 0; i < length; i++)
    {
        printf("%d, ", curve[i]);
    }
}

int main(int argc, char** argv)
{
    char *pCh;
    unsigned long deg = 42;
    // Check enough arguments.

    if (argc != 2) {
        puts ("Not enough arguments");
        return 1;
    }    
    
    // Convert to ulong WITH CHECKING!

    deg = strtoul (argv[1], &pCh, 10);
// Ensure argument was okay.

    if ((pCh == argv[1]) || (*pCh != '\0')) {
        puts ("Invalid number");
        return 1;
    }    
// Avoid warning about unused parameter.
    (void) argc; (void) argv;
	
	unsigned dim=pow(9,deg);
    int length = 8;
	u_int64_t *x=(u_int64_t*)malloc(dim*sizeof(u_int64_t));
	u_int64_t *y=(u_int64_t*)malloc(dim*sizeof(u_int64_t));
	int curr1[] = {0,0,1,2,2,1,0,0}; //0 : up , 1 : right , 2 : down , 3 : left , Kurve mit grad 1
    int* curr=(int*)malloc(length*sizeof(int));
    int* curr2=(int*)malloc(length*sizeof(int));
	//peano(deg,x,y);
    printCurve(length, curr1);    
    printCurve(length, curr);
    printCurve(length, curr2);
	free(x);
	free(y);
	free(curr);
    free(curr1);

    return 0;
}


 

 
	

