#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>
struct option long_options[] =
{
    {"help", no_argument, NULL, 'h'},
    {"svg", no_argument, NULL, 's'}
};
//_________________ITERATIVE IN PLACE V2 ALGORITHM______________________________
void reverseInPlaceV2(u_int64_t *x, u_int64_t *y, u_int64_t pos, u_int64_t size)
{

    for (u_int64_t i = 1; i < size; i++)
    {
        u_int64_t tempy = y[i] - y[i - 1]; // direction left=-1 / right=1
        u_int64_t tempx = x[i] - x[i - 1]; // direction up=1 / down =-1
        y[pos + i] = y[pos + i - 1] - tempy;
        x[pos + i] = x[pos + i - 1] - tempx;
    }
}

void mirrorInPlaceV2(u_int64_t *x, u_int64_t *y, u_int64_t pos, u_int64_t size)
{
    for (u_int64_t i = 1; i < size; i++)
    {
        u_int64_t tempy = y[i] - y[i - 1]; // direction left=-1 / right=1
        u_int64_t tempx = x[i] - x[i - 1]; // direction up=1 / down =-1
        y[pos + i] = y[pos + i - 1] - tempy;
        x[pos + i] = x[pos + i - 1] + tempx;
    }
}

void copyInPlaceV2(u_int64_t *x, u_int64_t *y, u_int64_t pos, u_int64_t size)
{
    for (u_int64_t i = 1; i < size; i++)
    {
        u_int64_t tempy = y[i] - y[i - 1]; // direction left=-1 / right=1
        u_int64_t tempx = x[i] - x[i - 1]; // direction up=1 / down =-1
        y[pos + i] = y[pos + i - 1] + tempy;
        x[pos + i] = x[pos + i - 1] + tempx;
    }
}

void reverseMirrorInPlaceV2(u_int64_t *x, u_int64_t *y, u_int64_t pos, u_int64_t size)
{
    for (u_int64_t i = 1; i < size; i++)
    {
        u_int64_t tempy = y[i] - y[i - 1]; // direction left=-1 / right=1
        u_int64_t tempx = x[i] - x[i - 1]; // direction up=1 / down =-1
        y[pos + i] = y[pos + i - 1] + tempy;
        x[pos + i] = x[pos + i - 1] - tempx;
    }
}
int calcNextInPlaceV2(int currGrad, u_int64_t *x, u_int64_t *y, u_int64_t pos)
{
    u_int64_t size = (u_int64_t)pow(3, 2 * currGrad);
    // Erster Zwischenstep
    x[pos] = x[pos - 1];
    y[pos] = y[pos - 1] + 1;

    // Zweiter step
    reverseMirrorInPlaceV2(x, y, pos, size);
    pos += size;

    //Zwischenschritt nach oben
    x[pos] = x[pos - 1];
    y[pos] = y[pos - 1] + 1;

    // Dritter step
    copyInPlaceV2(x, y, pos, size);
    pos += size;

    //Zwischenschritt nach rechts
    x[pos] = x[pos - 1] + 1;
    y[pos] = y[pos - 1];

    //Vierter Step
    mirrorInPlaceV2(x, y, pos, size);
    pos += size;

    //Zwischenschritt nach unten
    x[pos] = x[pos - 1];
    y[pos] = y[pos - 1] - 1;

    //Fünfter Step
    reverseInPlaceV2(x, y, pos, size);
    pos += size;

    //Zwischenschritt nach unten
    x[pos] = x[pos - 1];
    y[pos] = y[pos - 1] - 1;

    //Sechster Step
    mirrorInPlaceV2(x, y, pos, size);
    pos += size;

    //Zwischenschritt nach rechts
    x[pos] = x[pos - 1] + 1;
    y[pos] = y[pos - 1];

    //Siebter Step
    copyInPlaceV2(x, y, pos, size);
    pos += size;

    //Zwischenschritt nach oben
    x[pos] = x[pos - 1];
    y[pos] = y[pos - 1] + 1;

    //Achter Step
    reverseMirrorInPlaceV2(x, y, pos, size);
    pos += size;

    //Zwischenschritt nach oben
    x[pos] = x[pos - 1];
    y[pos] = y[pos - 1] + 1;

    //Letzter Step
    copyInPlaceV2(x, y, pos, size);
    pos += size;
    return pos;
}
void peanoInPlaceV2(unsigned grad, u_int64_t *x1, u_int64_t *y1)
{
    if (grad <= 0 || grad > 9) //auch Auf überlauf checken!!
    {
        printf("Error number not valid !");
        return;
    }
    //array
    x1[0] = 1;
    y1[0] = 1;
    //array[1]
    x1[1] = 1;
    y1[1] = 2;
    //array[2]
    x1[2] = 1;
    y1[2] = 3;
    //array[3]
    x1[3] = 2;
    y1[3] = 3;
    //array[4]
    x1[4] = 2;
    y1[4] = 2;
    //array[5]
    x1[5] = 2;
    y1[5] = 1;
    //array[6]
    x1[6] = 3;
    y1[6] = 1;
    //array[7]
    x1[7] = 3;
    y1[7] = 2;
    //array[8]
    x1[8] = 3;
    y1[8] = 3;
    unsigned pos = 9;
    unsigned currGrad = 1;
    while (currGrad < grad)
    {
        pos = calcNextInPlaceV2(currGrad, x1, y1, pos);
        currGrad++;
    }
}
//_________________RECURSIVEV2 IN PLACE ALGORITHM______________________________
int calcNextRecursiveV2(unsigned currGrad,unsigned Grad, u_int64_t *x,u_int64_t*y, u_int64_t pos)
{
    if (currGrad>=Grad){
        return pos;
    }
    u_int64_t size = (u_int64_t)pow(3, 2 * currGrad);
    // Erster Zwischenstep
    x[pos] = x[pos-1];
    y[pos]=y[pos-1]+1;

    // Zweiter step
    reverseMirrorInPlaceV2(x,y, pos, size);
    pos += size;

    //Zwischenschritt nach oben
    x[pos] = x[pos-1];
    y[pos]=y[pos-1]+1;

    // Dritter step
    copyInPlaceV2(x,y, pos, size);
    pos += size;

    //Zwischenschritt nach rechts
    x[pos] = x[pos-1]+1;
    y[pos]=y[pos-1];

    //Vierter Step
    mirrorInPlaceV2(x,y, pos, size);
    pos += size;

    //Zwischenschritt nach unten
    x[pos] = x[pos-1];
    y[pos]=y[pos-1]-1;

    //Fünfter Step
    reverseInPlaceV2(x,y, pos, size);
    pos += size;

    //Zwischenschritt nach unten
    x[pos] = x[pos-1];
    y[pos]=y[pos-1]-1;

    //Sechster Step
    mirrorInPlaceV2(x,y, pos, size);
    pos += size;

    //Zwischenschritt nach rechts
    x[pos] = x[pos-1]+1;
    y[pos]=y[pos-1];

    //Siebter Step
    copyInPlaceV2(x,y, pos, size);
    pos += size;

    //Zwischenschritt nach oben
    x[pos] = x[pos-1];
    y[pos]=y[pos-1]+1;

    //Achter Step
    reverseMirrorInPlaceV2(x,y, pos, size);
    pos += size;

    //Zwischenschritt nach oben
    x[pos] = x[pos-1];
    y[pos]=y[pos-1]+1;

    //Letzter Step
    copyInPlaceV2(x,y, pos, size);
    pos += size;
    calcNextRecursiveV2(currGrad+1,Grad, x,y,  pos);
}
void peanoRecursiveV2(unsigned grad, u_int64_t *x1, u_int64_t *y1)
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
    
    calcNextRecursiveV2(currGrad,grad,x1,y1, pos);
}
//_________________HELP FUNCTIONS______________________________
void getHelp()
{
    puts("\n-------------------------------HELP-------------------------------\n");
    puts("This programm offers multiple diffrent algorithms to calculate");
    puts("the peano curve for any entered degree greater than zero.\n");

    puts("\t\t~~~~~~~~~~Algorithms~~~~~~~~~~\n");


    puts("1) Iterative in-place V2 in C:");
    puts("\tStart the program with the the following arguments:");
    puts("\t-i <degree>");
    puts("\twhere degree must be an integer greater than zero.");

    puts("2) RecursiveV2 in C:");
    puts("\tStart the program with the the following arguments:");
    puts("\t-r <degree>");
    puts("\twhere degree must be an integer greater than zero.\n");

    puts("Optionally you can add an argument --svg (or -s) which will");
    puts("create a svg file with the calculated peano curve for visual");
    puts("representation and/or an argument -t which will print the");
    puts("time the algorithm needed to successfully terminate.\n");

    puts("For more detailed information about the peano curve please read");
    puts("the attributive document \"Ausarbeitung.pdf\".");

    puts("------------------------------------------------------------------\n");
}
void drawSvg(u_int64_t *x, u_int64_t *y, int size)
{
    int grad = (int)(log(size) / log(3) / 2);
    int pxWidth = 2187; // 3⁷
    int step = pxWidth / (int)sqrt(size);
    pxWidth += step; // pixel offset

    FILE *fp = fopen("PeanoCurveV2.svg", "w+");

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

enum Algorithm
{
    InPlaceV2,
    RecursiveV2,
    Empty
};
int runAlgorithm(enum Algorithm alg, int deg, bool svg, bool time)
{
    int size = (int)pow(9, deg);
    u_int64_t *x = (u_int64_t *)malloc(size * sizeof(u_int64_t));
    u_int64_t *y = (u_int64_t *)malloc(size * sizeof(u_int64_t));

    if (x == NULL || y == NULL)
    {
        perror("Please try again with a smaller degree ");
        return (3);
    }

    struct timespec before;
    struct timespec after;

    switch (alg)
    {

    case InPlaceV2:
        printf("Running In-PlaceV2 iterativ algorithm in C with degree %d...\n", deg);
        clock_gettime(CLOCK_MONOTONIC, &before);
        peanoInPlaceV2(deg, x, y);
        clock_gettime(CLOCK_MONOTONIC, &after);
        break;
    case RecursiveV2:
        printf("Running RecursiveV2 algorithm in C with degree %d...\n", deg);
        clock_gettime(CLOCK_MONOTONIC, &before);
        peanoRecursiveV2(deg, x, y);
        clock_gettime(CLOCK_MONOTONIC, &after);
        break;
    default:
        break;
    }

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

    // Depending on the input arguments an svg file is created
    if (svg)
    {
        puts("Creating svg file...");
        clock_gettime(CLOCK_MONOTONIC, &before);
        drawSvg(x, y, size);
        clock_gettime(CLOCK_MONOTONIC, &after);
        printf("Svg file successfully created");

        if (time) // With argument -t the operation time to create the svg file is also printed
        {
            if (deg > 6)
            {
                double secBefore = before.tv_sec + 1e-9 * before.tv_nsec;
                double secAfter = after.tv_sec + 1e-9 * after.tv_nsec;
                printf(" after %f seconds", secAfter - secBefore);
            }
            else
                printf(" after %ld nanoseconds", (u_int64_t)after.tv_nsec - (u_int64_t)before.tv_nsec);
        }
        puts(".");
    }

    free(x);
    free(y);

    return 0;
}


int main(int argc, char *argv[])
{
    int index = 0;
    unsigned degree = 0;
    enum Algorithm alg = Empty;

    int mandArgs = -1;
    bool time = false;
    bool svg = false;

    if(argc == 1)
    {
        puts("Not enough arguments, try -h or --help for help");
        return 0;
    }

    while ((index = getopt_long(argc, argv, "ha:i:o:r:ts", long_options, NULL)) != -1)
    {
        switch (index)
        {
        case 'h':
            getHelp();
            return 0;
            break;
      
        case 'i':
            mandArgs++;
            alg = InPlaceV2;
            degree = atoi(optarg);
            break;
        
        case 'r':
            mandArgs++;
            alg = RecursiveV2;
            degree = atoi(optarg);
            break;
        case 't':
            time = true;
            break;
        case 's':
            svg = true;
            break;
        case '?':
            return 1;
        default:
            break;
        }
    }

    if (mandArgs < 0)
    {
        puts("Specify an algorithm by entering either -i or -r.");
        return 1;
    }
    else if (mandArgs > 0)
    {
        puts("Can only choose one of the algorithm arguments -iand -r.");
        return 1;
    }
    if (degree < 1)
    {
        printf("Invalid input for degree, try an integer greater than 0.\n");
        return 1;
    }

    //printf("Algorithm: %d, Degree: %d, Time: %d, SVG: %d\n", alg, degree, time, svg);

    return runAlgorithm(alg, degree, svg, time);
}


