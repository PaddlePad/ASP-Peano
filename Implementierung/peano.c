#include <stdio.h>
#include <stdlib.h>
void peano(unsigned degree, uint64_t* x, uint64_t* y);


int main(int argc, char** argv) {

char *pCh;
    unsigned long unlong = 42;
    // Check enough arguments.

    if (argc != 2) {
        puts ("Not enough arguments");
        return 1;
    }    
    
    // Convert to ulong WITH CHECKING!

    unlong = strtoul (argv[1], &pCh, 10);
// Ensure argument was okay.

    if ((pCh == argv[1]) || (*pCh != '\0')) {
        puts ("Invalid number");
        return 1;
    }    
// Avoid warning about unused parameter.
    (void) argc; (void) argv;
    

return 0;
}

