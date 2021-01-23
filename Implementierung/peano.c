#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//void peano(unsigned degree, uint64_t* x, uint64_t* y);
//void reverse(u_int64_t in, u_int64_t out, unsinged size);
//void mirror(u_int64_t in, u_int64_t out, unsinged size);
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
    FILE *fp;

    fp = fopen("tot.svg", "w+");

    fprintf(fp, " <svg viewBox="
                "-10 -10 120 120"
                " xmlns="
                "http://www.w3.org/2000/svg"
                ">\n");
    fprintf(fp, "<polyline stroke="
                "black"
                " fill="
                "none"
                "\n");
    fprintf(fp, "points=");
    for (int i = 0; i < size; i++)
    {
        fprintf(fp, "%ld,%ld ", x[i], y[i]);
    }

    fprintf(fp, "/>\n");
    fprintf(fp, "</svg>\n");

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
    arraycopy(curr, 0, pre, 0, length);
    int *mir = (int *)malloc(length * sizeof(int));
    int *rev = (int *)malloc(length * sizeof(int));
    int *revMir = (int *)malloc(length * sizeof(int));
    mirror(mir, curr, length);
    reverse(rev, curr, length);
    reverse(revMir, mir, length);

    curr = (int *)realloc(curr,  ((int)pow(9, currGrad) - 1) * sizeof(int)); // für 8 Dir pro base Kurve//TODO

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
    free(mir);
    free(rev);
    free(revMir);
}

void peano(unsigned grad, u_int64_t *x1, u_int64_t *y1)
{
    unsigned currGrad = 2;
    if (grad <= 0)
    {
        printf("Error number not valid !");
        return;
    }

    int *curr = (int*)malloc(length * sizeof(int));
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
    int i = 1;
    x1[0] = x;
    y1[0] = y;

    for (int d = 0; d < length; d++)
    {
        switch (curr[d])
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

        i++;
    }

    free(curr);
}

int main(int argc, char **argv)
{
    char *pCh;
    unsigned long deg = 42;
    // Check enough arguments.

    if (argc != 2)
    {
        puts("Not enough arguments");
        return 1;
    }

    // Convert to ulong WITH CHECKING!

    deg = strtoul(argv[1], &pCh, 10);
    // Ensure argument was okay.

    if ((pCh == argv[1]) || (*pCh != '\0'))
    {
        puts("Invalid number");
        return 1;
    }
    // Avoid warning about unused parameter.
    (void)argc;
    (void)argv;

    unsigned dim = (int)pow(9, deg);
    u_int64_t *x = (u_int64_t *)malloc(dim * sizeof(u_int64_t));
    u_int64_t *y = (u_int64_t *)malloc(dim * sizeof(u_int64_t));

    peano(deg, x, y);
    drawSvg(x, y, dim);
    free(x);
    free(y);


    return 0;
}
