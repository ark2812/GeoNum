#include <stdio.h>
#include <stdlib.h>



//void affiche(int *tableau, int tailleTableau);
int isInside(double x1, double y1, double x2, double y2,double x3, double y3, double xR, double yR );
int answer(double x1, double y1, double x2, double y2,double x3, double y3, double xR, double yR );
int createPoints(double *X, double *Y, int length);

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
   int ins = createPoints(X,Y,length);
   printf("%d\n",ins);
   return 0;

}
