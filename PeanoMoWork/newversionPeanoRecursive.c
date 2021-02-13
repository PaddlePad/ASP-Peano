#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
void reverseInPlace(u_int64_t *x, u_int64_t *y, u_int64_t pos, u_int64_t size)
{

    for (u_int64_t i = 1; i < size; i++)
    {
        u_int64_t tempy=y[i]-y[i-1];// direction left=-1 / right=1
        u_int64_t tempx=x[i]-x[i-1];// direction up=1 / down =-1
            y[pos + i] = y[pos + i-1] - tempy ;
            x[pos + i] = x[pos + i-1] - tempx ;
    }
}

void mirrorInPlace(u_int64_t *x, u_int64_t*y, u_int64_t pos, u_int64_t size)
{
    for (u_int64_t i = 1; i < size; i++)
    {
        u_int64_t tempy=y[i]-y[i-1];// direction left=-1 / right=1
        u_int64_t tempx=x[i]-x[i-1];// direction up=1 / down =-1        
            y[pos + i] = y[pos + i-1] - tempy ;
            x[pos + i] = x[pos + i-1] + tempx ;
    }
}

void copyInPlace(u_int64_t *x,u_int64_t*y, u_int64_t pos, u_int64_t size)
{
    for (u_int64_t i = 1; i < size; i++)
    {
        u_int64_t tempy=y[i]-y[i-1];// direction left=-1 / right=1
        u_int64_t tempx=x[i]-x[i-1];// direction up=1 / down =-1
            y[pos + i] = y[pos + i-1] + tempy ;
            x[pos + i] = x[pos + i-1] + tempx ;
    }
}

void reverseMirrorInPlace(u_int64_t *x,u_int64_t*y,  u_int64_t pos, u_int64_t size)
{
    for (u_int64_t i = 1; i < size; i++)
    {
        u_int64_t tempy=y[i]-y[i-1];// direction left=-1 / right=1
        u_int64_t tempx=x[i]-x[i-1];// direction up=1 / down =-1
            y[pos + i] = y[pos + i-1] + tempy ;
            x[pos + i] = x[pos + i-1] - tempx ;
        }
}
int calcNextInplace(unsigned currGrad,unsigned Grad, u_int64_t *x,u_int64_t*y, u_int64_t pos)
{
    if (currGrad>=Grad){
        return pos;
    }
    u_int64_t size = (u_int64_t)pow(3, 2 * currGrad);
    // Erster Zwischenstep
    x[pos] = x[pos-1];
    y[pos]=y[pos-1]+1;

    // Zweiter step
    reverseMirrorInPlace(x,y, pos, size);
    pos += size;

    //Zwischenschritt nach oben
    x[pos] = x[pos-1];
    y[pos]=y[pos-1]+1;

    // Dritter step
    copyInPlace(x,y, pos, size);
    pos += size;

    //Zwischenschritt nach rechts
    x[pos] = x[pos-1]+1;
    y[pos]=y[pos-1];

    //Vierter Step
    mirrorInPlace(x,y, pos, size);
    pos += size;

    //Zwischenschritt nach unten
    x[pos] = x[pos-1];
    y[pos]=y[pos-1]-1;

    //Fünfter Step
    reverseInPlace(x,y, pos, size);
    pos += size;

    //Zwischenschritt nach unten
    x[pos] = x[pos-1];
    y[pos]=y[pos-1]-1;

    //Sechster Step
    mirrorInPlace(x,y, pos, size);
    pos += size;

    //Zwischenschritt nach rechts
    x[pos] = x[pos-1]+1;
    y[pos]=y[pos-1];

    //Siebter Step
    copyInPlace(x,y, pos, size);
    pos += size;

    //Zwischenschritt nach oben
    x[pos] = x[pos-1];
    y[pos]=y[pos-1]+1;

    //Achter Step
    reverseMirrorInPlace(x,y, pos, size);
    pos += size;

    //Zwischenschritt nach oben
    x[pos] = x[pos-1];
    y[pos]=y[pos-1]+1;

    //Letzter Step
    copyInPlace(x,y, pos, size);
    pos += size;
    calcNextInplace(currGrad+1,Grad, x,y,  pos);
}
void peanoInPlace(unsigned grad, u_int64_t *x1, u_int64_t *y1)
{
    if (grad <= 0 || grad > 9) //auch Auf überlauf checken!!
    {
        printf("Error number not valid !");
        return;
    }
//array
    x1[0]=1;
    y1[0]=1;
    //array[1]
    x1[1]=1;
    y1[1]=2;
    //array[2]
    x1[2]=1;
    y1[2]=3;
    //array[3]
    x1[3]=2;
    y1[3]=3;
    //array[4]
    x1[4]=2;
    y1[4]=2;
    //array[5]
    x1[5]=2;
    y1[5]=1;
    //array[6]
    x1[6]=3;
    y1[6]=1;
    //array[7]
    x1[7]=3;
    y1[7]=2;
    //array[8]
    x1[8]=3;
    y1[8]=3;
    unsigned pos = 9;
    unsigned currGrad = 1;
    
    calcNextInplace(currGrad,grad,x1,y1, pos);
        
    

}
void printCoordinates(u_int64_t *x, u_int64_t *y, u_int64_t len)
{
    for (u_int64_t i = 0; i < len; i++)
    {
        printf("x: %ld, y: %ld\n", x[i], y[i]);
    }
}

void drawSvg(u_int64_t *x, u_int64_t *y, int size)
{
    int grad = (int)(log(size) / log(3) / 2);
    int pxWidth = 2187; // 3⁷
    int step = pxWidth / (int)sqrt(size);
    pxWidth += step; // pixel offset

    FILE *fp = fopen("peanoRecursiveV2.svg", "w+");

    fprintf(fp, "<?xml version=\"1.0\"?>\n");
    fprintf(fp, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n");
    fprintf(fp, "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");

    fprintf(fp, "\n<svg height=\"%d\" width=\"%d\">\n", pxWidth, pxWidth);
    fprintf(fp, "\t<polyline points=\"");
    for (int i = 0; i < size; i++)
    {
        fprintf(fp, "%ld,%ld ", x[i] * step, pxWidth - (y[i] * step));
        if (i > 0 && i % 9 == 0)
            fprintf(fp, "\n\t");
    }
    fprintf(fp, "\"\n\tstyle=\"fill:none;stroke:black;stroke-width:%d\" />\n", 10 / grad * 2);
    fprintf(fp, "</svg>\n");

    fclose(fp);

    printf("Created new svg file for Grad %d\n", grad);
}
int main(int argc, char *argv[])
{
    // Mit negativen graden testen

    char *pCh;
    unsigned deg = 42;
       struct timespec before;
    struct timespec after;
   
    deg = strtoul(argv[1], &pCh, 10);
    
    int size = (int)pow(9, deg);
    u_int64_t *x = (u_int64_t *)malloc(size * sizeof(u_int64_t));
    u_int64_t *y = (u_int64_t *)malloc(size * sizeof(u_int64_t));
    

    if (x == NULL || y == NULL)
    {
        puts("It's to fucking big!");
        return (3);
    }

  
    clock_gettime(CLOCK_MONOTONIC, &before);
        peanoInPlace(deg, x,y);
         
        clock_gettime(CLOCK_MONOTONIC, &after);
 drawSvg(x,y,size);
        // Je nach Angabeparameter wird eine SVG Datei erstellt
    

        // Je nach Angabe wird die Zeit ausgegeben
      printf("Peano calculation successfully terminated");
    if (time) // Depending on the input arguments the operating time is printed
    {
        if (deg > 7) // Upwards from degree 7 we use seconds to avoid overflows
        {
            double secBefore = before.tv_sec + 1e-9 * before.tv_nsec;
            double secAfter = after.tv_sec + 1e-9 * after.tv_nsec;
            printf(" after %f seconds", secAfter - secBefore);
        }
        else
            printf(" after %ld nanoseconds", (u_int64_t)after.tv_nsec - (u_int64_t)before.tv_nsec);
    }
    puts(".");

    free(x);
    free(y);

    return 0;
}

