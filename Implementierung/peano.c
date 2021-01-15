#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
void drawSvg( int deg ,uint64_t* x, uint64_t* y ){
 uint64_t* x1=x;
 uint64_t* y1=y;
    FILE *fp;

   fp = fopen("/tmp/test.txt", "w+");

   fprintf(fp, " <svg viewBox=""-10 -10 120 120"" xmlns=""http://www.w3.org/2000/svg"">\n");
   fprintf(fp, "<polyline stroke=""black"" fill=""none""\n");
   fprintf(fp, "points=");
   for (int i=0 ; i<pow(3,deg),i++ ){
      
       fprintf(fp, "%d,%d ",x1,y1);
       y1++;
       x1++
   }
   
   fprintf(fp, "/>\n");
   fprintf(fp, "</svg>\n");
   
   //fputs("This is testing for fputs...\n", fp);
   fclose(fp);
}

