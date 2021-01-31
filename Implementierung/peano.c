#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

void peano(unsigned degree, u_int64_t* x, u_int64_t* y);
void printDirections(int *dir, u_int64_t len);
//void reverse(u_int64_t in, u_int64_t out, unsinged size);
//void mirror(u_int64_t in, u_int64_t out, unsinged size);
//u_int64_t = long unsigned int
//---------------------DECLARATIONS------------------------
// base Kurve merken
int length = 8;

//int curr1[] = {0, 0, 1, 2, 2, 1, 0, 0}; //0 : up , 1 : right , 2 : down , 3 : left
//arraycopy(curr, 0, curr1, 0, length);
//free(curr1);

//------------------------HELP FUNCTIONS-------------------
void arraycopy(int *src, int srcPos, int *dest, int destPos, int length1) //VERIFY IF THIS WORKS
{
    for (int i = srcPos; i < (srcPos + length1); i++)
    {
        dest[destPos + (i - srcPos)] = src[i];
    }
}

void drawSvg(u_int64_t *x, u_int64_t *y, int size)
{
    int grad = (int) (log(size) / log(3) / 2);
    int pxWidth = 2187;             // 3⁷
    int step = pxWidth / (int)sqrt(size);
    pxWidth += step;                // pixel offset 

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
        if(i > 0 && i % 9 == 0)
            fprintf(fp, "\n\t");
    }      
    fprintf(fp, "\"\n\tstyle=\"fill:none;stroke:white;stroke-width:%d\" />\n", 10 / grad * 2);
    fprintf(fp, "</svg>\n");

    fclose(fp);
}

void mirror(int *out, int *in, int size) // spiegelt up&Down, gibt dircetions zurück
{
    for (int i = 0; i < size; i++)
    {
        if (in[i] % 2 == 0)
        {
            // int tmp = (in[i] + 2) % 4;
            // out[i] = tmp;
            out[i] = (in[i] + 2) % 4;
        }
        else
        {
            // int tmp = in[i];
            // out[i] = tmp;
            out[i] = in[i];
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

//----------------------------PEANO HELP FUNCTIONS--------------
void calcNext(int currGrad, int *curr)
{
    int *pre = (int *)malloc(length * sizeof(int));
    int *mir = (int *)malloc(length * sizeof(int));
    int *rev = (int *)malloc(length * sizeof(int));
    int *revMir = (int *)malloc(length * sizeof(int));
    mirror(mir, curr, length);
    reverse(rev, curr, length);
    reverse(revMir, mir, length);

    int i = 0;

    arraycopy(pre, 0, curr, i, length);
    i = i + length;
    curr[i] = 0;
    i++;

    arraycopy(revMir, 0, curr, i, length);
    i = i + length;
    curr[i] = 0;
    i++;

    arraycopy(pre, 0, curr, i, length);
    i = i + length;
    curr[i] = 1;
    i++;

    arraycopy(mir, 0, curr, i, length);
    i = i + length;
    curr[i] = 2;
    i++;

    arraycopy(rev, 0, curr, i, length);
    i = i + length;
    curr[i] = 2;
    i++;

    arraycopy(mir, 0, curr, i, length);
    i = i + length;
    curr[i] = 1;
    i++;

    arraycopy(pre, 0, curr, i, length);
    i = i + length;
    curr[i] = 0;
    i++;

    arraycopy(revMir, 0, curr, i, length);
    i = i + length;
    curr[i] = 0;
    i++;

    arraycopy(pre, 0, curr, i, length);
    length = ((int)pow(9, currGrad) - 1);
    free(pre);
    free(mir);
    free(rev);
    free(revMir);
}

void reverseInPlace(int *arr, u_int64_t pos, u_int64_t size)
{
    for (u_int64_t i = 1; i <= size; i++)
    {
        int temp  = (arr[i] + 2) % 4;
        arr[pos + i] = temp;
    }
}

void mirrorInPlace(int *arr, u_int64_t pos, u_int64_t size)
{
    for(u_int64_t i = 1; i <= size; i++)
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
    u_int64_t size = (u_int64_t) pow(3, 2 * currGrad);
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

void peanoInC(unsigned grad, u_int64_t *x1, u_int64_t *y1)
{
    unsigned currGrad = 2;
    if (grad <= 0)  //auch Auf überlauf checken!!
    {
        printf("Error number not valid !");
        return;
    }

    int *curr = (int *)malloc(length * sizeof(int));
    int curr1[] = {0, 0, 1, 2, 2, 1, 0, 0}; //0 : up , 1 : right , 2 : down , 3 : left
    arraycopy(curr1, 0, curr, 0, length);

    if (grad != 1)
    {
        while (currGrad <= grad)
        {
            calcNext(currGrad, curr);
            currGrad++;
        }
    }

    int x = 1;
    int y = 1;
    x1[0] = x;
    y1[0] = y;

    for (int i = 0; i < length; i++)
    {
        switch (curr[i])
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

    free(curr);
}

void peanoInPlace(unsigned grad, u_int64_t *x1, u_int64_t *y1)
{
    if (grad <= 0)  //auch Auf überlauf checken!!
    {
        printf("Error number not valid !");
        return;
    }

    unsigned currGrad = 1;
    u_int64_t amt = (u_int64_t) pow(3, 2 * grad);

    int *array = (int *)malloc(amt * sizeof(int));

    u_int64_t curr1[] = {0, 0, 1, 2, 2, 1, 0, 0}; //0 : up , 1 : right , 2 : down , 3 : left
    unsigned pos = 9;

    for (u_int64_t i = 1; i <= pos; i++)
    {
        array[i] = curr1[i-1];
    }

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

    for (u_int64_t i = 1; i < amt; i++)
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

void getHelp()  //Wertebereich des Grads angeben!
{
    printf("---------------------HELP---------------------\n");
    printf("Dieses Programm enthält unterschiedliche Arten\n");
    printf("eine Peano-Kurve als .svg Datei ausgeben zu \n");
    printf(" lassen. In dieser Implementierung benotigt \n");
    printf(" die Kurve den Parameter <Grad>. Dieser gibt \n");
    printf(" die Anzahl an Iterationen an. Für weiter\n");
    printf(" Informationen (zur Peano-Kurve) lesen sie \n");
    printf(" bitte \"Ausarbeitung.pdf\". \n");

    printf("1.) Die Peano Kurve iterativ in C). \n");
    printf("\tDazu das Programm mit den Argumenten: \n");
    printf("\t -C <Grad> \n");
    printf("\tstarten, wobei <Grad> eine Ganze positive Zahl\nist.\n");

    printf("2.) Die Peano Kurve iterativ in Assembler. \n");
    printf("\tDazu das Programm mit den Argumenten: \n");
    printf("\t <Grad> \n");
    printf("\tstarten, wobei <Grad> eine Ganze positive Zahl\nist.\n");

    printf("3.) Die Peano Kurve rekursiv in C. \n");
    printf("\tDazu das Programm mit den Argumenten: \n");
    printf("\t -r <Grad> \n");
    printf("\tstarten, wobei <Grad> eine Ganze positive Zahl\nist.\n");
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

int main(int argc, char **argv)
{
    char *pCh;
    unsigned long deg = 42;
    char* executeRekursive = "-r";
    char* executeInC = "-C";

    // Check arguments
    if (argc > 3 || argc < 2)
    {
        puts("Not enough or too many arguments. Try again with -h for Help.");
        return 1;
    }
    else
    {
        if(argc == 2)
        {
            if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0)
            {
                getHelp();
                return 0;
            }
            else    //Peno Kurve iterativ in Assembler aufrufen
            {
                deg = strtoul(argv[1], &pCh, 10);
            }
        }
        else if (argc == 3 && (strcmp(argv[1], executeInC) == 0 || strcmp(argv[1], executeRekursive) == 0))
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
        puts("Invalid number");
        return 1;
    }
    // Avoid warning about unused parameter.    eigentlich unnötig...
    (void)argc;
    (void)argv;

    unsigned dim = (int)pow(9, deg);
    u_int64_t *x = (u_int64_t *)malloc(dim * sizeof(u_int64_t));
    u_int64_t *y = (u_int64_t *)malloc(dim * sizeof(u_int64_t));

    struct timespec before;
    struct timespec after;


    //WENN FERTIG EINKOMMENTIEREN!!
    if(argc == 2)   //Execute Assembler
    {
        puts("Assembler, GOGOGO!");
        clock_gettime(CLOCK_MONOTONIC, &before);
        peano(deg, x, y);
        clock_gettime(CLOCK_MONOTONIC, &after);
        printf("Nanoseconds passed: %ld\n", after.tv_nsec - before.tv_nsec);
        printf("Seconds passed: %ld\n", (after.tv_sec - before.tv_sec));
        //printCoordinates(x, y, dim);
        drawSvg(x, y, dim);
    }
    else if(argc == 3 && strcmp(argv[1], executeInC) == 0)   //execute Peano in C
    {
        puts("Iterativ, GOGOGO!");
        //peanoInC(deg, x, y);
        peanoInPlace(deg, x, y);
        //printCoordinates(x,y, dim);
        drawSvg(x, y, dim);
    }
    else    //execute Rekursive
    {
        puts("Rekursiv, GOGOGO!");
    }

    free(x);
    free(y);

    return 0;
}
