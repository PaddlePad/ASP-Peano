#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void peano(unsigned degree, u_int64_t *x, u_int64_t *y);
void printDirections(int *dir, u_int64_t len);
//void reverse(u_int64_t in, u_int64_t out, unsinged size);
//void mirror(u_int64_t in, u_int64_t out, unsinged size);
//u_int64_t = long unsigned int
//---------------------DECLARATIONS------------------------
// base Kurve merken
//int length = 8;

//----------------------------PEANO ITERATIVE NOT OUT-OF-PLACE----------------------------

void arraycopy(int *src, int srcPos, int *dest, int destPos, int length1) //VERIFY IF THIS WORKS
{
    for (int i = srcPos; i < (srcPos + length1); i++)
    {
        dest[destPos + (i - srcPos)] = src[i];
    }
}

void mirror(int *out, int *in, int size, int reverse) // spiegelt up&Down, gibt dircetions zurück, wenn 'reverse' auf 1 gesetzt wird, wird zudem auch reversed
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

void reverse(int *out, int *in, int size) // spiegelt up&Down, left&right
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

    int *pre = (int *)malloc(currSize * sizeof(int));
    int *mir = (int *)malloc(currSize * sizeof(int));
    int *rev = (int *)malloc(currSize * sizeof(int));
    int *revMir = (int *)malloc(currSize * sizeof(int));
    arraycopy(array, 0, pre, 0, currSize);
    mirror(mir, array, currSize, 0);
    reverse(rev, array, currSize);
    mirror(revMir, array, currSize, 1);

    int i = stepSize;

    // Erster Zwischenschritt nach oben
    array[i++] = 0;

    // Erster Schritt: reversed, mirrored
    arraycopy(revMir, 0, array, i, currSize);
    i += stepSize;

    // Zweiter Zwischenschritt nach oben
    array[i++] = 0;

    // Zweiter Schritt
    arraycopy(pre, 0, array, i, currSize);
    i += stepSize;

    // Dritter Zwischenschritt nach rechts
    array[i++] = 1;

    // Dritter Schritt: mirrored
    arraycopy(mir, 0, array, i, currSize);
    i += stepSize;

    // Vierter Zwischenschritt nach unten
    array[i++] = 2;

    // Vierter Schritt: reversed
    arraycopy(rev, 0, array, i, currSize);
    i += stepSize;

    // Fünfter Zwischenschritt nach unten
    array[i++] = 2;

    // Fünfter Schritt: mirrored
    arraycopy(mir, 0, array, i, currSize);
    i += stepSize;

    // Sechster Zwischenschritt nach rechts
    array[i++] = 1;

    // Sechster Schritt
    arraycopy(pre, 0, array, i, currSize);
    i += stepSize;

    // Siebter Zwischenschritt nach oben
    array[i++] = 0;

    // Siebter Schritt: reversed + mirrored
    arraycopy(revMir, 0, array, i, currSize);
    i += stepSize;

    // Achter Zwischenschritt nach oben
    array[i++] = 0;

    // Achter Schritt
    arraycopy(pre, 0, array, i, currSize);

    currSize = ((int)pow(9, prevGrad) - 1);
    free(pre);
    free(mir);
    free(rev);
    free(revMir);
}

void peanoInC(unsigned grad, u_int64_t *x1, u_int64_t *y1)
{
    if (grad < 1 || grad > 9) //auch Auf überlauf checken!!
    {
        printf("Error number not valid !");
        return;
    }

    u_int64_t size = (u_int64_t)pow(3, 2 * grad);
    int *array = (int *)malloc(size * sizeof(int));
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
    // Erster Zwischenstep
    curr[pos] = 0;

    // Zweiter step
    reverseMirrorInPlace(curr, pos, size);
    pos += size;

    //Zwischenschritt nach oben
    curr[pos] = 0;

    // Dritter step
    copyInPlace(curr, pos, size);
    pos += size;

    //Zwischenschritt nach rechts
    curr[pos] = 1;

    //Vierter Step
    mirrorInPlace(curr, pos, size);
    pos += size;

    //Zwischenschritt nach unten
    curr[pos] = 2;

    //Fünfter Step
    reverseInPlace(curr, pos, size);
    pos += size;

    //Zwischenschritt nach unten
    curr[pos] = 2;

    //Sechster Step
    mirrorInPlace(curr, pos, size);
    pos += size;

    //Zwischenschritt nach rechts
    curr[pos] = 1;

    //Siebter Step
    copyInPlace(curr, pos, size);
    pos += size;

    //Zwischenschritt nach oben
    curr[pos] = 0;

    //Achter Step
    reverseMirrorInPlace(curr, pos, size);
    pos += size;

    //Zwischenschritt nach oben
    curr[pos] = 0;

    //Letzter Step
    copyInPlace(curr, pos, size);
    pos += size;
    return pos;
}

void peanoInPlace(unsigned grad, u_int64_t *x1, u_int64_t *y1)
{
    if (grad <= 0 || grad > 9) //auch Auf überlauf checken!!
    {
        printf("Error number not valid !");
        return;
    }

    u_int64_t size = (u_int64_t)pow(3, 2 * grad);

    // Richtungsarray allokieren und die Startkurve hardcodiert einfügen
    int *array = (int *)malloc(size * sizeof(int));
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
        case 0: //oben
            y++;
            break;
        case 2: //unten
            y--;
            break;
        case 3: //links
            x--;
            break;
        case 1: //rechts
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

//----------------------------HELPER FUNCTIONS----------------------------

void getHelp() //Wertebereich des Grads angeben!
{
    printf("---------------------HELP---------------------\n");
    printf("Dieses Programm enthält unterschiedliche Arten\n");
    printf("eine Peano-Kurve als .svg Datei ausgeben zu \n");
    printf("lassen. In dieser Implementierung benötigt \n");
    printf("die Kurve den Parameter <Grad>. Dieser gibt \n");
    printf("die Anzahl an Iterationen an. Für weiter\n");
    printf("Informationen (zur Peano-Kurve) lesen Sie \n");
    printf("bitte \"Ausarbeitung.pdf\".\n\n");

    printf("1.) Die Peano Kurve iterativ in C). \n");
    printf("\tDazu das Programm mit den Argumenten: \n");
    printf("\t-C <Grad> \n");
    printf("\tstarten, wobei <Grad> eine Ganze positive Zahl\n");
    printf("\tzwischen 1 und 9 (eingeschlossen) ist.\n");

    printf("2.) Die Peano Kurve iterativ in Assembler. \n");
    printf("\tDazu das Programm mit den Argumenten: \n");
    printf("\t<Grad> \n");
    printf("\tstarten, wobei <Grad> eine Ganze positive Zahl\n");
    printf("\tzwischen 1 und 9 (eingeschlossen) ist.\n");

    printf("3.) Die Peano Kurve rekursiv in C. \n");
    printf("\tDazu das Programm mit den Argumenten: \n");
    printf("\t-r <Grad> \n");
    printf("\tstarten, wobei <Grad> eine Ganze positive Zahl\n");
    printf("\tzwischen 1 und 9 (eingeschlossen) ist.\n");
    printf("----------------------------------------------\n");
}

void printCoordinates(u_int64_t *x, u_int64_t *y, u_int64_t len)
{
    for (u_int64_t i = 0; i < len; i++)
    {
        printf("x: %ld, y: %ld\n", x[i], y[i]);
    }
}

void printDirections(int *dir, u_int64_t len)
{
    for (u_int64_t i = 0; i < len; i++)
    {
        printf("Direction: %d \n", dir[i]);
    }
    puts("\n");
}

void drawSvg(u_int64_t *x, u_int64_t *y, int size)
{
    int grad = (int)(log(size) / log(3) / 2);
    int pxWidth = 2187; // 3⁷
    int step = pxWidth / (int)sqrt(size);
    pxWidth += step; // pixel offset

    FILE *fp;

    // char* title = (char*) malloc(sizeof(char) * 10);
    // sprintf(title, "peanoGrad%d.svg", dim);
    // free(title);

    fp = fopen("peano.svg", "w+");

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

//-----------------------------------MAIN--------------------------------------

int main(int argc, char **argv)
{
    // argument einbauen für SVG File ja/nein

    char *pCh;
    unsigned long deg = 42;
    char *executeRekursive = "-r";
    char *executeInplace = "-C";
    char *executeC = "-c";

    // Check arguments
    if (argc > 3 || argc < 2)
    {
        puts("Not enough or too many arguments. Try again with -h for Help.");
        return 1;
    }
    else
    {
        if (argc == 2)
        {
            if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0)
            {
                getHelp();
                return 0;
            }
            else //Peno Kurve iterativ in Assembler aufrufen
            {
                deg = strtoul(argv[1], &pCh, 10);
            }
        }
        else if (argc == 3 && (strcmp(argv[1], executeInplace) == 0 || strcmp(argv[1], executeRekursive) == 0 || strcmp(argv[1], executeC) == 0))
        {
            // Ensure argument was okay.
            deg = strtoul(argv[2], &pCh, 10);
        }
        else
        {
            puts("Invalid Arguments. Try again with -h for help (in GER).");
            return 2;
        }
    }

    // Convert to ulong WITH CHECKING!
    if ((pCh == argv[1]) || (*pCh != '\0'))
    {
        puts("Invalid character, try an integer ;)");
        exit(1);
    }
    if (1 > deg || deg > 9)
    {
        puts("Invalid integer, must be in [1;9]");
        exit(1);
    }

    char scn[1];
    printf("SVG erstellen? 1 = yes, 0 = no\n");
    int q = scanf("%s", scn);
    int svg = atoi(scn);

    (void)q;

    // Avoid warning about unused parameter.    eigentlich unnötig...
    // (void)argc;
    // (void)argv;

    int size = (int)pow(9, deg);
    u_int64_t *x = (u_int64_t *)malloc(size * sizeof(u_int64_t));
    u_int64_t *y = (u_int64_t *)malloc(size * sizeof(u_int64_t));

    if (x == NULL || y == NULL)
    {
        puts("It's to fucking big!");
        exit(3);
    }

    struct timespec before;
    struct timespec after;

    if (argc == 2) //Execute Assembler
    {
        puts("Assembly...");
        clock_gettime(CLOCK_MONOTONIC, &before);
        peano(deg, x, y);
        clock_gettime(CLOCK_MONOTONIC, &after);
        printf("Assembly Nanoseconds passed: %ld\n", after.tv_nsec - before.tv_nsec);
        printf("Assembly Seconds passed: %ld\n", (after.tv_sec - before.tv_sec));
        if (svg == 1)
            drawSvg(x, y, size);
    }
    else if (argc == 3 && strcmp(argv[1], executeInplace) == 0) //execute Peano in C
    {
        puts("In-Place Iterativ in C...");
        clock_gettime(CLOCK_MONOTONIC, &before);
        peanoInPlace(deg, x, y);
        clock_gettime(CLOCK_MONOTONIC, &after);
        printf("C Nanoseconds passed: %ld\n", (u_int64_t)after.tv_nsec - (u_int64_t)before.tv_nsec);
        printf("C Seconds passed: %ld\n", (after.tv_sec - before.tv_sec));
        if (svg == 1)
            drawSvg(x, y, size);
    }
    else if (argc == 3 && strcmp(argv[1], executeC) == 0)
    {
        puts("Out-Of-Place Iterativ in C...");
        clock_gettime(CLOCK_MONOTONIC, &before);
        peanoInC(deg, x, y);
        clock_gettime(CLOCK_MONOTONIC, &after);
        printf("C Nanoseconds passed: %ld\n", (u_int64_t)after.tv_nsec - (u_int64_t)before.tv_nsec);
        printf("C Seconds passed: %ld\n", (after.tv_sec - before.tv_sec));
        if (svg == 1)
            drawSvg(x, y, size);
    }
    else //execute Rekursive
    {
        puts("Rekursiv in C...");
    }

    free(x);
    free(y);

    return 0;
}