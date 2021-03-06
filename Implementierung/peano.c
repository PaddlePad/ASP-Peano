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
 
// Assemblyfunction
void peano(unsigned degree, u_int64_t *x, u_int64_t *y);

//----------------------------PEANO ITERATIVE OUT-OF-PLACE----------------------------

void arraycopy(int *src, int *dest, int insertStartPosition, int length1)
{
    for (int i = 0; i < +length1; i++)
    {
        dest[insertStartPosition + i] = src[i];
    }
}

void mirror(int *out, int *in, int size, int reverse) // switches up&Down, returns dircetions. if 'reverse' is 1, reversed is also executed
{
    for (int i = 0; i < size; i++)
    {
        int temp = in[i];

        if (temp % 2 == 0)
        {
            if (reverse == 1)
                out[i] = temp;
            else
                out[i] = (temp + 2) % 4;
        }
        else
        {
            if (reverse == 1)
                out[i] = (temp + 2) % 4;
            else
                out[i] = temp;
        }
    }
}

void reverse(int *out, int *in, int size) // switches up&Down, left&right
{
    for (int i = 0; i < size; i++)
    {
        // int tmp = (in[i] + 2) % 4;
        // out[i] = tmp;
        out[i] = (in[i] + 2) % 4;
    }
}

void calcNext(int prevGrad, int *array)
{
    u_int64_t currSize = (u_int64_t)pow(3, prevGrad * 2);
    u_int64_t stepSize = currSize - 1;

    // Saving Permutations
    int *pre = (int *)malloc(currSize * sizeof(int));
    int *mir = (int *)malloc(currSize * sizeof(int));
    int *rev = (int *)malloc(currSize * sizeof(int));
    int *revMir = (int *)malloc(currSize * sizeof(int));

    if (pre == NULL || mir == NULL || rev == NULL || revMir == NULL)
    {
        perror("Please try again with a smaller degree ");
    }

    arraycopy(array, pre, 0, currSize);
    mirror(mir, array, currSize, 0);
    reverse(rev, array, currSize);
    mirror(revMir, array, currSize, 1);

    int i = stepSize;

    // First Step between Permutations upwards
    array[i++] = 0;

    // First Step: reversed, mirrored
    arraycopy(revMir, array, i, currSize);
    i += stepSize;

    // Second Step between Permutations upwards
    array[i++] = 0;

    // Second Step
    arraycopy(pre, array, i, currSize);
    i += stepSize;

    // Third Step between Permutations right
    array[i++] = 1;

    // Third Step: mirrored
    arraycopy(mir, array, i, currSize);
    i += stepSize;

    // 4th Step between Permutations downwards
    array[i++] = 2;

    //4th Step: reversed
    arraycopy(rev, array, i, currSize);
    i += stepSize;

    // 5th Step between Permutations downwards
    array[i++] = 2;

    // 5th Step: mirrored
    arraycopy(mir, array, i, currSize);
    i += stepSize;

    // 6th Step between Permutations right
    array[i++] = 1;

    // 6th Step
    arraycopy(pre, array, i, currSize);
    i += stepSize;

    // 7thStep between Permutations upwards
    array[i++] = 0;

    // 7th Step: reversed + mirrored
    arraycopy(revMir, array, i, currSize);
    i += stepSize;

    // 8th Step between Permutations upwards
    array[i++] = 0;

    // 8th Step
    arraycopy(pre, array, i, currSize);

    currSize = ((int)pow(9, prevGrad) - 1);
    free(pre);
    free(mir);
    free(rev);
    free(revMir);
}

void peanoOutOfPlace(unsigned grad, u_int64_t *x1, u_int64_t *y1)
{
    if (grad < 1)
    {
        perror("Error number not valid !");
        return;
    }

    // Curve with Deg. 1 hardcoded:
    u_int64_t size = (u_int64_t)pow(3, 2 * grad);
    int *array = (int *)malloc(size * sizeof(int));

    if (array == NULL)
    {
        perror("Couldnt allocate enough memory, please try again with a smaller Degree!\n");
    }

    array[0] = 0;
    array[1] = 0;
    array[2] = 1;
    array[3] = 2;
    array[4] = 2;
    array[5] = 1;
    array[6] = 0;
    array[7] = 0;

    unsigned currGrad = 1;
    while (currGrad < grad)
    {
        calcNext(currGrad, array);
        currGrad++;
    }

    int x = 1;
    int y = 1;
    x1[0] = x;
    y1[0] = y;

    for (u_int64_t i = 1; i < size; i++)
    {
        switch (array[i - 1])
        {
        case 0:
            y++;
            break;
        case 2:
            y--;
            break;
        case 3:
            x--;
            break;
        case 1:
            x++;
            break;
        default:
            break;
        }
        x1[i] = x;
        y1[i] = y;
    }

    free(array);
}

//---------------------------------PEANO ITERATIVE IN-PLACE---------------------------------

void reverseInPlace(int *arr, u_int64_t pos, u_int64_t size)
{
    for (u_int64_t i = 1; i <= size; i++)
    {
        int temp = (arr[i] + 2) % 4;
        arr[pos + i] = temp;
    }
}

void mirrorInPlace(int *arr, u_int64_t pos, u_int64_t size)
{
    for (u_int64_t i = 1; i <= size; i++)
    {
        int temp = arr[i];

        if (temp % 2 == 0)
        {
            arr[pos + i] = (temp + 2) % 4;
        }
        else
        {
            arr[pos + i] = temp;
        }
    }
}

void copyInPlace(int *arr, u_int64_t pos, u_int64_t size)
{
    for (u_int64_t i = 1; i <= size; i++)
    {
        int temp = arr[i];
        arr[pos + i] = temp;
    }
}

void reverseMirrorInPlace(int *arr, u_int64_t pos, u_int64_t size)
{
    for (u_int64_t i = 1; i < size; i++)
    {
        int temp = arr[i];

        if (temp % 2 == 0)
        {
            arr[pos + i] = temp;
        }
        else
        {
            arr[pos + i] = (temp + 2) % 4;
        }
    }
}

int calcNextInplace(int currGrad, int *curr, u_int64_t pos)
{
    u_int64_t size = (u_int64_t)pow(3, 2 * currGrad);

    // First Step between Permutations upwards
    curr[pos] = 0;

    // second step
    reverseMirrorInPlace(curr, pos, size);
    pos += size;

    //Step between Permutations upwards
    curr[pos] = 0;

    // third step
    copyInPlace(curr, pos, size);
    pos += size;

    //Step between Permutations rigth
    curr[pos] = 1;

    //4th Step
    mirrorInPlace(curr, pos, size);
    pos += size;

    //Step between Permutations downwards
    curr[pos] = 2;

    //5th Step
    reverseInPlace(curr, pos, size);
    pos += size;

    //Step between Permutations downwards
    curr[pos] = 2;

    //6th Step
    mirrorInPlace(curr, pos, size);
    pos += size;

    //Step between Permutations rigth
    curr[pos] = 1;

    //7th Step
    copyInPlace(curr, pos, size);
    pos += size;

    //Step between Permutations upwards
    curr[pos] = 0;

    //8th Step
    reverseMirrorInPlace(curr, pos, size);
    pos += size;

    //Step between Permutations upwards
    curr[pos] = 0;

    //last Step
    copyInPlace(curr, pos, size);
    pos += size;
    return pos;
}

void peanoInPlace(unsigned grad, u_int64_t *x1, u_int64_t *y1)
{
    if (grad <= 0)
    {
        printf("Error number not valid !");
        return;
    }

    u_int64_t size = (u_int64_t)pow(3, 2 * grad);

    // Allocate Directionarray and hardcode first curve
    int *array = (int *)malloc(size * sizeof(int));

    if (array == NULL)
    {
        perror("Please try again with a smaller degree ");
    }

    array[1] = 0;
    array[2] = 0;
    array[3] = 1;
    array[4] = 2;
    array[5] = 2;
    array[6] = 1;
    array[7] = 0;
    array[8] = 0;

    unsigned pos = 9;
    unsigned currGrad = 1;
    while (currGrad < grad)
    {
        pos = calcNextInplace(currGrad, array, pos);
        currGrad++;
    }

    //printDirections(array, amt);

    u_int64_t x = 1;
    u_int64_t y = 1;
    x1[0] = x;
    y1[0] = y;

    for (u_int64_t i = 1; i < size; i++)
    {
        switch (array[i])
        {
        case 0: //up
            y++;
            break;
        case 2: //down
            y--;
            break;
        case 3: //left
            x--;
            break;
        case 1: //right
            x++;
            break;
        default:
            break;
        }

        x1[i] = x;
        y1[i] = y;
    }

    free(array);
}

//----------------------------PEANO RECURSIVE (TODO)----------------------------
void calcNextInplaceRecursive(int grad, int currGrad, int *curr, u_int64_t pos)
{
    if (currGrad >= grad)
    {
        return;
    }

    u_int64_t size = (u_int64_t)pow(3, 2 * currGrad);

    // first step between Permutation
    curr[pos] = 0;

    // second step
    reverseMirrorInPlace(curr, pos, size);
    pos += size;

    //Step between Permutations up
    curr[pos] = 0;

    // third step
    copyInPlace(curr, pos, size);
    pos += size;

    //Step between Permutations right
    curr[pos] = 1;

    //4th Step
    mirrorInPlace(curr, pos, size);
    pos += size;

    //Zwischenschritt nach unten
    curr[pos] = 2;

    //5th Step
    reverseInPlace(curr, pos, size);
    pos += size;

    //Step between Permutations down
    curr[pos] = 2;

    //6th Step
    mirrorInPlace(curr, pos, size);
    pos += size;

    ////Step between Permutations right
    curr[pos] = 1;

    //7th Step
    copyInPlace(curr, pos, size);
    pos += size;

    //Step between Permutations up
    curr[pos] = 0;

    //8th Step
    reverseMirrorInPlace(curr, pos, size);
    pos += size;

    //Step between Permutations up
    curr[pos] = 0;

    //last Step
    copyInPlace(curr, pos, size);
    pos += size;
    calcNextInplaceRecursive(grad, currGrad + 1, curr, pos);
}

void peanoInPlaceRecursive(unsigned grad, u_int64_t *x1, u_int64_t *y1)
{
    if (grad <= 0)
    {
        printf("Error number not valid !");
        return;
    }

    u_int64_t size = (u_int64_t)pow(3, 2 * grad);

    // allocate direction array and hardcode first curve
    int *array = (int *)malloc(size * sizeof(int));

    if (array == NULL)
    {
        perror("Please try again with a smaller degree ");
    }

    array[1] = 0;
    array[2] = 0;
    array[3] = 1;
    array[4] = 2;
    array[5] = 2;
    array[6] = 1;
    array[7] = 0;
    array[8] = 0;

    unsigned pos = 9;

    calcNextInplaceRecursive(grad, 1, array, pos);

    //printDirections(array, amt);

    u_int64_t x = 1;
    u_int64_t y = 1;
    x1[0] = x;
    y1[0] = y;

    for (u_int64_t i = 1; i < size; i++)
    {
        switch (array[i])
        {
        case 0: //up
            y++;
            break;
        case 2: //down
            y--;
            break;
        case 3: //left
            x--;
            break;
        case 1: //right
            x++;
            break;
        default:
            break;
        }

        x1[i] = x;
        y1[i] = y;
    }

    free(array);
}

//----------------------------HELPER FUNCTIONS----------------------------

void getHelp()
{
    puts("\n-------------------------------HELP-------------------------------\n");
    puts("This programm offers multiple diffrent algorithms to calculate");
    puts("the peano curve for any entered degree greater than zero.\n");

    puts("\t\t~~~~~~~~~~Algorithms~~~~~~~~~~\n");
    puts("1) Iterative in Assembly:");
    puts("\tStart the program with the the following arguments:");
    puts("\t-a <degree>");
    puts("\twhere degree must be an integer greater than zero.");

    puts("2) Iterative in-place in C:");
    puts("\tStart the program with the the following arguments:");
    puts("\t-i <degree>");
    puts("\twhere degree must be an integer greater than zero.");

    puts("3) Iterative out-of-place in C:");
    puts("\tStart the program with the the following arguments:");
    puts("\t-o <degree>");
    puts("\twhere degree must be an integer greater than zero.");

    puts("4) Recursive in C:");
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

void printCoordinates(u_int64_t *x, u_int64_t *y, u_int64_t len)
{
    for (u_int64_t i = 0; i < len; i++)
    {
        printf("x: %ld, y: %ld\n", x[i], y[i]);
    }
}

void printDirections(u_int64_t *dir, u_int64_t len)
{
    for (u_int64_t i = 0; i < len; i++)
    {
        printf("Direction: %ld \n", dir[i]);
        if (i % 9 == 0)
        {
            printf("\n");
        }
        
    }
    puts("\n");
}

void drawSvg(u_int64_t *x, u_int64_t *y, int size)
{
    int grad = (int)(log(size) / log(3) / 2);
    int pxWidth = 2187; // 3⁷
    int step = pxWidth / (int)sqrt(size);
    pxWidth += step; // pixel offset

    FILE *fp = fopen("peano.svg", "w+");

    fprintf(fp, "<?xml version=\"1.0\"?>\n");
    fprintf(fp, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n");
    fprintf(fp, "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");

    fprintf(fp, "\n<svg height=\"%d\" width=\"%d\">\n", pxWidth, pxWidth);
    fprintf(fp, "\t<polyline points=\"");
    for (int i = 0; i < size; i++)
    {
        // If the previous and the next point are on the same line the current point can be skipped
        // This reduces operation time and memory by ~50% 
        if (i > 0 && (x[i - 1] == x[i + 1] || y[i - 1] == y[i + 1])) 
            continue;

        fprintf(fp, "%ld,%ld ", x[i] * step, pxWidth - y[i] * step);
        if (i > 0 && i % 9 == 0)
            fprintf(fp, "\n\t");
    }
    fprintf(fp, "\"\n\tstyle=\"fill:none;stroke:black;stroke-width:%d\" />\n", 10 / grad * 2);
    fprintf(fp, "</svg>\n");

    fclose(fp);
}

enum Algorithm
{
    Assembly,
    InPlace,
    OutOfPlace,
    Recursiv,
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
    case Assembly:
        printf("Running assembly algorithm with degree %d...\n", deg);
        clock_gettime(CLOCK_MONOTONIC, &before);
        peano(deg, x, y);
        clock_gettime(CLOCK_MONOTONIC, &after);
        break;
    case InPlace:
        printf("Running In-Place iterativ algorithm in C with degree %d...\n", deg);
        clock_gettime(CLOCK_MONOTONIC, &before);
        peanoInPlace(deg, x, y);
        clock_gettime(CLOCK_MONOTONIC, &after);
        break;
    case OutOfPlace:
        printf("Running Out-Of-Place iterativ algorithm in C with degree %d...\n", deg);
        clock_gettime(CLOCK_MONOTONIC, &before);
        peanoOutOfPlace(deg, x, y);
        clock_gettime(CLOCK_MONOTONIC, &after);
        break;
    case Recursiv:
        printf("Running recursive algorithm in C with degree %d...\n", deg);
        clock_gettime(CLOCK_MONOTONIC, &before);
        peanoInPlaceRecursive(deg, x, y);
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

//-----------------------------------MAIN--------------------------------------

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
        case 'a': // choose assembly
            mandArgs++;
            alg = Assembly;
            degree = atoi(optarg);
            break;
        case 'i':
            mandArgs++;
            alg = InPlace;
            degree = atoi(optarg);
            break;
        case 'o':
            mandArgs++;
            alg = OutOfPlace;
            degree = atoi(optarg);
            break;
        case 'r':
            mandArgs++;
            alg = Recursiv;
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
        puts("Specify an algorithm by entering either -a, -i, -o or -r.");
        return 1;
    }
    else if (mandArgs > 0)
    {
        puts("Can only choose one of the algorithm arguments -a, -i, -o and -r.");
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