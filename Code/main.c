#include <stdio.h>
#include <stdlib.h>



//void affiche(int *tableau, int tailleTableau);
double isInside(double x1, double y1, double x2, double y2,double x3, double y3, double xR, double yR );
//double SetOfPointsCoordinates(double *table);
 //static double valOfPoints[6] = {0,0,0,1,1,1};
int main(int argc, char *argv[])
{
    //int tableau[4] = {10, 15, 3};

    //affiche(tableau, 4);
//    printf("%f\n",valOfPoints[3]);
  //double x =  SetOfPointsCoordinates(valOfPoints);
  int inside = isInside(0,0,0,1,1,1,0.6,0.4);
   printf("%d\n",inside);
   return 0;

}
