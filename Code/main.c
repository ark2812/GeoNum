#include <stdio.h>
#include <stdlib.h>



//void affiche(int *tableau, int tailleTableau);
double SetOfPointsCoordinates(double *table);
 static double valOfPoints[6] = {0,0,0,1,1,1};
int main(int argc, char *argv[])
{
    //int tableau[4] = {10, 15, 3};
 
    //affiche(tableau, 4);
    printf("%f\n",valOfPoints[3]);
  double x =  SetOfPointsCoordinates(valOfPoints);
   
   printf("%f\n",x);
   return 0;
 
}
 

