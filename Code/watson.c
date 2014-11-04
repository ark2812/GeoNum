#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <GL/glfw.h>
#include <pthread.h>
#include <errno.h>

/*
// le pointeur vers les coordonnées de notre point
typedef struct {
    double x;
    double y;
} meshPoint;

typedef struct {
    meshPoint *Point;
} meshPoints;


typedef struct {
    meshPoints *Points;
} meshProblem;

meshPoint *meshPointCreate(double x, double y);
meshPoints *meshPointsCreate(double *table);
double findCenter(meshPoints* Points);
//meshPoint *meshPointCreate(double x, double y);
static meshPoints* ListPoints;

// C example funcion
void affiche(int *tableau, int tailleTableau)
{
    int i;

    for (i = 0 ; i < tailleTableau ; i++)
    {
        printf("%d\n", tableau[i]);
    }
}

double SetOfPointsCoordinates(double* table)
{
	printf("coucou");
	meshPointsCreate(table);
	double x = findCenter(ListPoints);
	return x;

}

meshPoints *meshPointsCreate(double *table)
{
	int i;

	meshPoints* ListPoints = malloc(sizeof(meshPoints));
	for (i = 0 ; i < 4 ; i++)
    {
        ListPoints->Point[i].x = table[2*i];
        ListPoints->Point[i].y = table[2*i+1];
    }
    return ListPoints;

}



double findCenter(meshPoints* Points)
{
	double a,b,slopA, slopB, xCenter;//, yCenter;
	double x1,x2,x3,y1,y2,y3;
        x1 = ListPoints->Point[0].x;
        printf("%f",x1);
        y1 = ListPoints->Point[0].y;
        x2 = ListPoints->Point[1].x;
        y2 = ListPoints->Point[1].y;
         x3 = ListPoints->Point[2].x;
        y3 = ListPoints->Point[2].y;


	if (x2 == x1)
	{
		if(x3 == x1)
		{
			return -1;
		}
		a = x2;
		b = y2;
		x2 = x3;
		y2 = y3;
		x3 = a;
		y3 = b;

	}
	if (x3 == x2)
	{
		a = x2;
		b = y2;
		x2 = x1;
		y2 = y1;
		x1 = a;
		y1 = b;
	}
	if(y3 == y1)
	{
			if(y3 == y2)
			{
			return -1;
			}
	}
	printf("%f\n",x1);
	slopA = (y2-y1)/(x2-x1);
	printf("%f\n",slopA);
	slopB = (y3-y2)/(x3-x2);
	printf("%f\n",slopB);
	xCenter = (slopA*slopB*(y1-y3) + slopB*(x1+x2) - slopA*(x2 + x3))/(2*(slopB-slopA));

	//yCenter = -1/slopA*(xCenter - (x1+x2)/2) + (y1+y2)/2;
	return xCenter;
}*/

double isInside( double x1, double y1, double x2, double y2,double x3, double y3, double xR, double yR )
{
    double a, b, slopA, slopB, xCenter, yCenter;
  if (x2 == x1)
  {
    if(x3 == x1)
    {
      return -1;
    }
    a = x2;
    b = y2;
    x2 = x3;
    y2 = y3;
    x3 = a;
    y3 = b;

  }
  if (x3 == x2)
  {
    a = x2;
    b = y2;
    x2 = x1;
    y2 = y1;
    x1 = a;
    y1 = b;
  }
  if(y3 == y1)
  {
      if(y3 == y2)
      {
      return -1;
      }
  }
  printf("%f\n",x1);
  slopA = (y2-y1)/(x2-x1);
  printf("%f\n",slopA);
  slopB = (y3-y2)/(x3-x2);
  printf("%f\n",slopB);
  xCenter = (slopA*slopB*(y1-y3) + slopB*(x1+x2) - slopA*(x2 + x3))/(2*(slopB-slopA));
  yCenter = -1/slopA*(xCenter - (x1+x2)/2) + (y1+y2)/2;

  double radius; int inside;
  radius = sqrt((x1-xCenter)*(x1-xCenter)+(y1-yCenter)*(y1-yCenter));
  if((xR-xCenter)*(xR-xCenter)+(yR-yCenter)*(yR-yCenter)<=radius*radius)
    {
        inside = 1;
    }
    else
      {
        inside =0;
      }

  return inside ;
}
