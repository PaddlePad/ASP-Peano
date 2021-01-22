#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void Peano(int x, int y, int lg, int i1, int i2);

void main(int argc, char *argv[])
{
    int dim = 0;
    if (argc != 2)
    {
        char scn[1];
        printf("Enter dimension...\n");
        scanf("%s", scn);
        dim = atoi(scn);
    }
    else
        dim = atoi(argv[1]);

    if (dim < 1)
    {
        printf("Dimension must be > 0!\n");
        exit(0);
    }

    int width = 1;
    for (int i = 0; i < dim; i++)
    {
        width = width * 3;
    }

    //printf("Width is %d\n", width);

    Peano(1, 1, width, 0, 0);
}

//Curve width must be 3 m .
//Start Peano recursion.

void Peano(int x, int y, int lg, int i1, int i2)
{
    if (lg == 1)
    {
        printf("%d%c%d\n", x, ',', y);
        // Output coordinates.
        return; // Exit recursion.
    }
    // Divide by 3.
    lg = lg / 3;

    Peano(x + (2 * i1 * lg), y + (2 * i1 * lg), lg, i1, i2);
    Peano(x + ((i1 + i2) * lg), y + ((i1 - i2 + 1) * lg), lg, 1 - i1, 1 - i2);
    Peano(x + (2 * i2 * lg), y + (2 * (1 - i2) * lg), lg, i1, i2);
    Peano(x+((1+i2-i1)*lg), y+((2-i1-i2)*lg), lg, i1, i2);
    Peano(x + lg, y + lg, lg, i1, 1 - i2);
    Peano(x + ((i1 - i2 + 1) * lg), y + ((i1 + i2) * lg), lg, i1, 1 - i2);
    Peano(x+(2*(1-i2)*lg), y+(2*i2*lg), lg, 1-i1, i2);
    Peano(x+((2-i1-i2)*lg), y+((1+i2-i1)*lg), lg, 1-i1, i2);
    Peano(x+(2*(1-i1)*lg), y+(2*(1-i1)*lg), lg, i1, i2);


    // Peano(x + (2 * i1 * lg), y + (2 * i1 * lg), lg, i1, i2);
    // Peano(x + ((i1 - i2 + 1) * lg), y + ((i1 + i2) * lg), lg, i1, 1 - i2);
    // Peano(x + lg, y + lg, lg, i1, 1 - i2);
    // Peano(x + ((i1 + i2) * lg), y + ((i1 - i2 + 1) * lg), lg, 1 - i1, 1 - i2);
    // Peano(x + (2 * i2 * lg), y + (2 * (1 - i2) * lg), lg, i1, i2);
    // Peano(x+((1+i2-i1)*lg), y+((2-i1-i2)*lg), lg, i1, i2);
    // //Peano(x+(1+i2-i1)*lg), y+((2-i1-i2)*lg), lg, i1, i2); eventuell auch so
    // Peano(x+(2*(1-i1)*lg), y+(2*(1-i1)*lg), lg, i1, i2);
    // Peano(x+((2-i1-i2)*lg), y+((1+i2-i1)*lg), lg, 1-i1, i2);
    // Peano(x+(2*(1-i2)*lg), y+(2*i2*lg), lg, 1-i1, i2);
}