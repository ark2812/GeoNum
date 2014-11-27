#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "watson.h"

//#include "glfem.h"


/*TODO 
-3 points extremes
-Meilleur interface graphique
-Robust pour les quad par exemple

*/
//int test();
int main(int argc, char *argv[])
{
	//int ans  = test();	
	FILE *fp;
	fp = fopen("data3.txt", "r");
	int length;
	char nPoints[10]; 
	fscanf(fp,"%s %d",nPoints, &length);
	double X[length];
	double Y[length];
	int i = 0;
	while (!feof(fp)) {
  		if (fscanf(fp, "%lf,%lf", &X[i], &Y[i]) != 2)
    		break;
  		i++;
	}
	//printf("X[0] : %f\n",X[0]);
	//printf("Y[0] : %f\n",Y[0]);
	fclose(fp);
   initialiseThePoint(X,Y,length);
   //test();
   int ins = testFunctions(length+3);
   printf("%d\n",ins);
   return 0;

}
