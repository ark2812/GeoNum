#include <stdio.h>
#include <stdlib.h>
#include "watson.h"
#include "glfem.h"
int test();
int main(int argc, char *argv[])
{
	
	FILE *fp;
	fp = fopen("datas.txt", "r");
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
	fclose(fp);
   createPoints(X,Y,length);
   //test();
   int ins = testFunctions();
   printf("%d\n",ins);
   return 0;

}
