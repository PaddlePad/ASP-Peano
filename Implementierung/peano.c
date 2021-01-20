#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define LENGTH(x)  (sizeof(x) / sizeof((x)[0]))
//void peano(unsigned degree, uint64_t* x, uint64_t* y);
//------------------------HELP FUNCTIONS-------------------




 void arraycopy (int* src, int srcPos, int* dest, int destPos, int length1 )//VERIFY IF THIS WORKS
{

	for (int i=srcPos; i<(srcPos+length1);i++){
	dest[destPos+(i-srcPos)]=src[i];}

}
void drawSvg( unsigned deg ,u_int64_t* x, u_int64_t* y ,int size)
{

    FILE *fp;

   fp = fopen("tot.svg", "w+");

   fprintf(fp, " <svg viewBox=""-10 -10 120 120"" xmlns=""http://www.w3.org/2000/svg"">\n");
   fprintf(fp, "<polyline stroke=""black"" fill=""none""\n");
   fprintf(fp, "points=");
   for (int i=0 ; i<size;i++ ){
      
       fprintf(fp, "%ld,%ld ",x[i],y[i]);
       
   }
   
   fprintf(fp, "/>\n");
   fprintf(fp, "</svg>\n");
   
   //fputs("This is testing for fputs...\n", fp);
   fclose(fp);
}
void mirror(int* out , int* in, int size ) // spiegelt up&Down, gibt dircetions zurück
	{
		

		for (int i = 0; i < size; i++)
		{
			if ((in[i] %2) == 0)
			{
				out[i] = (in[i]+2) % 4;
			}
			 else
			{
			out[i] == in[i];}
			
		}

		
	}
void reverse(int* out , int* in, int size)// spiegelt up&Down, left&right
	{
		

		for (int i = 0; i < size; i++)
		{
			out[i]=(in[i]+2)%4;
		}

		
	}
void changesize(int size , int** b){
	free(*b);
	*b=(int*) malloc (size);
	
}



//---------------------DECLARATIONS------------------------
	// base Kurve merken
	int* pre=(int*)malloc(5);
	int* curr=(int*)malloc(5);
	int curr1[5] = {0,0,1,2,2,1,0,0}; //0 : up , 1 : right , 2 : down , 3 : left
	
	arraycopy(pre,0,curr1,0,8);
	

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
	uint64_t *x;
	uint64_t *y;
	unsigned deg;
	peano(deg,x,y);
drawSvg(deg,x,y);

return 0;
}


 void peano(unsigned grad, uint64_t* x1, uint64_t* y1)
	{
		int currGrad = 2;
		if (grad <= 0){
				printf("Error number not valid !");
				return;
		}
		
		
		if(grad != 1)
		{
			while(currGrad <= grad)
			{
				calcNext(currGrad);
				currGrad++;
			}
		}

		
		int x = 1;
		int y = 1;
		int i = 1;
		x1[0]=x;
		y1[0]=y;
		

		for (int  d=0; d <length(curr) ;d++)
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
		x1[i]=x;
		y1[i]=y;
			
			i++;
		}

	
	}

 void calcNext(int currGrad)
	{
		//System.arraycopy(curr, 0, pre, 0, curr.length);
		int len = length(curr);
		pre =(int*) malloc (len);
		arraycopy(curr,0,pre,0,len);
		mir= (int* )(int*) malloc (len);
		rev= (int* )(int*) malloc (len);
		revMir= (int* )(int*) malloc (len);
		mirror(mir,curr,len);
		reverse(rev,curr,len);
		reverse(revMir,mir, len)
		
		free(curr);
		curr = (int*)malloc (pow(9,currGrad) - 1); // für 8 Dir pro base Kurve//TODO

		int i = 0;
		
		arraycopy(pre, 0, curr, i, len);
		i = i + len;
		curr[i] = 0;
		i++;

		arraycopy(revMir, 0, curr, i, len);
		i = i + len;
		curr[i] = 0;
		i++;

		arraycopy(pre, 0, curr, i, len);
		i = i + len;
		curr[i] = 1;
		i++;

		arraycopy(mir, 0, curr, i, len);
		i = i + len;
		curr[i] = 2;
		i++;

		arraycopy(rev, 0, curr, i, len);
		i = i + len;
		curr[i] = 2;
		i++;

		arraycopy(mir, 0, curr, i, len);
		i = i + len;
		curr[i] = 1;
		i++;

		arraycopy(pre, 0, curr, i, len);
		i = i + len;
		curr[i] = 0;
		i++;

		arraycopy(revMir, 0, curr, i, len);
		i = i + len;
		curr[i] = 0;
		i++;

		arraycopy(pre, 0, curr, i, len);
	}

	

