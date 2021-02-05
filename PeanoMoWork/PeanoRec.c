 
/*Abhishek Ghosh, 14th September 2018*/
#include <math.h>
 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
void Peano(int x, int y, int lg, int i1, int i2) {
 
	if (lg == 1) {
		printf("[%d,%d]\n", x,y);
		return;
	}
 
	lg = lg/3;
	/*Peano(x+(2*i1*lg), y+(2*i1*lg), lg, i1, i2); [0,0]
	Peano(x+((i1-i2+1)*lg), y+((i1+i2)*lg), lg, i1, 1-i2);[1,0]
	Peano(x+lg, y+lg, lg, i1, 1-i2);[1,1]
	Peano(x+((i1+i2)*lg), y+((i1-i2+1)*lg), lg, 1-i1, 1-i2);[0,1]
	Peano(x+(2*i2*lg), y+(2*(1-i2)*lg), lg, i1, i2);[0,2]
	Peano(x+((1+i2-i1)*lg), y+((2-i1-i2)*lg), lg, i1, i2);[1,2]
	Peano(x+(2*(1-i1)*lg), y+(2*(1-i1)*lg), lg, i1, i2);[2,2]
	Peano(x+((2-i1-i2)*lg), y+((1+i2-i1)*lg), lg, 1-i1, i2);[2,1]
	Peano(x+(2*(1-i2)*lg), y+(2*i2*lg), lg, 1-i1, i2);[2,0]*/
        Peano(x+(2*i1*lg), y+(2*i1*lg), lg, i1, i2); 
        Peano(x+((i1+i2)*lg), y+((i1-i2+1)*lg), lg, 1-i1, 1-i2);
        Peano(x+(2*i2*lg), y+(2*(1-i2)*lg), lg, i1, i2);
        Peano(x+((1+i2-i1)*lg), y+((2-i1-i2)*lg), lg, i1, i2);
        Peano(x+lg, y+lg, lg, i1, 1-i2);
        Peano(x+((i1-i2+1)*lg), y+((i1+i2)*lg), lg, i1, 1-i2);
        Peano(x+(2*(1-i2)*lg), y+(2*i2*lg), lg, 1-i1, i2);
        Peano(x+((2-i1-i2)*lg), y+((1+i2-i1)*lg), lg, 1-i1, i2);
        Peano(x+(2*(1-i1)*lg), y+(2*(1-i1)*lg), lg, i1, i2);
}
 
int main(void) {
 
	
	Peano(0, 0, 9, 0, 0); /* Start Peano recursion. */
 
	
 
	return 0;
}
