#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
void peano(unsigned degree, u_int64_t* x, u_int64_t* y);
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
    int width = size / 3;
    int dim = width / 3;
    int pixelWidth = dim * 300;

    FILE *fp;

    char* title = (char*) malloc(sizeof(char) * 10);
    sprintf(title, "peanoGrad%d.svg", dim);
    free(title);
    
    fp = fopen("peano.svg", "w+");

    fprintf(fp, "<?xml version=\"1.0\"?>\n");
    fprintf(fp, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n");
    fprintf(fp, "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");

    fprintf(fp, "\n<svg height=\"%d\" width=\"%d\">\n", pixelWidth, pixelWidth);
    fprintf(fp, "\t<polyline points=\"");
    for (int i = 0; i < size; i++)
    {
        //fprintf(fp, "%ld,%ld ", x[i], y[i]);
        fprintf(fp, "%ld,%ld ", x[i], y[i]);
        if(i > 0 && i % 9 == 0)
            fprintf(fp, "\n\t");
    }      
    fprintf(fp, "\"\n\tstyle=\"fill:none;stroke:black;stroke-width:3\" />\n");
    fprintf(fp, "</svg>\n");
                
               

    // fprintf(fp, "\n<svg viewBox=\"0 0 100 100\" xmlns=\"http://www.w3.org/2000/svg\">\n");
    // fprintf(fp, "<polyline style=\"stroke:black;fill:none;stroke-width:1\"\n");
    // fprintf(fp, "points=\"");
    // for (int i = 0; i < size; i++)
    // {
    //     fprintf(fp, "%ld,%ld ", x[i], y[i]);
    // }
    // fprintf(fp, "\"/>\n");
    // fprintf(fp, "</svg>\n");

    //fputs("This is testing for fputs...\n", fp);
    fclose(fp);
}

void mirror(int *out, int *in, int size) // spiegelt up&Down, gibt dircetions zurück
{
    for (int i = 0; i < size; i++)
    {
        if ((in[i] % 2) == 0)
        {
            out[i] = (in[i] + 2) % 4;
        }
        else
        {
            out[i] = in[i];
        }
    }
}

void reverse(int *out, int *in, int size) // spiegelt up&Down, left&right
{
    for (int i = 0; i < size; i++)
    {
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

    arraycopy(curr, 0, pre, 0, length);
    curr = (int *)realloc(curr, ((int)pow(9, currGrad) - 1) * sizeof(int)); // für 8 Dir pro base Kurve//TODO

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

void printCoordinates(u_int64_t *x, u_int64_t *y, unsigned len)
{

    (void)y;

    for (unsigned i = 0; i < len; i++)
    {
       printf("x: %ld, y: %ld\n", x[i], y[i]);
    }

    // for (unsigned i = 1; i < len; i++)
    // {
    //     printf("Direction: %ld\n", x[i]);
    // }
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

    //WENN FERTIG EINKOMMENTIEREN!!
    if(argc == 2)   //Execute Assembler
    {
        puts("Assembler, GOGOGO!");
        peano(deg, x, y);
        printCoordinates(x, y, dim);
        drawSvg(x, y, dim);
    }
    else if(argc == 3 && strcmp(argv[1], executeInC) == 0)   //execute Peano in C
    {
        puts("Iterativ, GOGOGO!");
        peanoInC(deg, x, y);
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
