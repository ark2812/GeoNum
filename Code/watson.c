#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <GL/glfw.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>


// le pointeur vers les coordonnées de notre point
typedef struct {
    double x;
    double y;
} meshPoint;

//sens anti-horlogique
typedef struct {
    meshPoint *A;
    meshPoint *B;
    meshPoint *C;
} meshTriangle;

typedef struct {
    meshTriangle *right;
    meshTriangle *left;
    meshPoint *A;
    meshPoint *B;
} meshEdge;

typedef struct ElementLoc *ElementLoc;
struct  ElementLoc{
    meshTriangle *T;
    ElementLoc *next1;
    ElementLoc *next3;
    ElementLoc *next2;
};

// structure de donnée, arbre de recherche
typedef struct {
    ElementLoc *first;
} LocationTree;






/*
 typedef struct {
 meshPoint *Point;
 } meshPoint;


 typedef struct {
 meshPoint *Points;
 } meshProblem;
 */
meshPoint *meshPointCreate(double x, double y);
int isInsideGen( meshPoint *thePoint1,  meshPoint *thePoint2,
                meshPoint *thePoint3,  meshPoint *thePointR);
/*
 meshPoint *meshPointCreate(double *table);
 static meshPoint* ListPoints;

 // C example funcion
 void affiche(int *tableau, int tailleTableau)
 {
 int i;

 for (i = 0 ; i < tailleTableau ; i++)
 {
 printf("%d\n", tableau[i]);
 }
 }



 meshPoint *meshPointCreate(double *table)
 {
	int i;

	meshPoint* ListPoints = malloc(sizeof(meshPoint));
	for (i = 0 ; i < 4 ; i++)
 {
 ListPoints->Point[i].x = table[2*i];
 ListPoints->Point[i].y = table[2*i+1];
 }
 return ListPoints;

 }

 */

int answer(double x1, double y1, double x2, double y2,double x3, double y3, double xR, double yR )
{
    meshPoint *thePoint1 =  meshPointCreate(x1, y1);
    meshPoint *thePoint2 =  meshPointCreate(x2, y2);
    meshPoint *thePoint3 =  meshPointCreate(x3, y3);
    meshPoint *thePointR =  meshPointCreate(xR, yR);

    int ins = isInsideGen(thePoint1,thePoint2,thePoint3,thePointR);
    free(thePoint1);
    free(thePoint2);
    free(thePoint3);
    free(thePointR);
    return ins;
}

meshPoint *meshPointCreate(double x, double y)
{
  meshPoint *thePoint =  malloc(sizeof(meshPoint));
  thePoint->x = x;
  thePoint->y = y;
  
  return thePoint;

}

/*
 isInsideGen returns 1 if the point R is inside a circle made of the 3
 points 1, 2 and 3. 0 if outside of the circle and -1 if there is a
 problem (3 points aligned etc.).
 */
int isInsideGen( meshPoint *thePoint1,  meshPoint *thePoint2,
                meshPoint *thePoint3,  meshPoint *thePointR)
{
    double x1,y1,x2,y2,x3,y3,xR,yR;
    x1= thePoint1->x ;
    y1 = thePoint1->y;
    x2 = thePoint2->x ;
    y2 = thePoint2->y ;
    x3 =   thePoint3->x ;
    y3 =   thePoint3->y ;
    xR =   thePointR->x ;
    yR =   thePointR->y;
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
    slopA = (y2-y1)/(x2-x1);
    slopB = (y3-y2)/(x3-x2);
    xCenter = (slopA*slopB*(y1-y3) + slopB*(x1+x2) - slopA*(x2 + x3))/(2*(slopB-slopA));
    yCenter = -1/slopA*(xCenter - (x1+x2)/2) + (y1+y2)/2;

    double radius;
    int inside;

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

/*
 This function tests if the point R lies at the right or at the left of segment (oriented) AB.
 Return  1 if it lies on the right
 -1 if it lies on the left
 0 if it lies on the segment
 */
int leftRightSegment(meshPoint *A, meshPoint *B, meshPoint *R)
{
    double d = (B->x-A->x)*(R->y-A->y) - (B->y-A->y)*(R->x-A->x);
    if (d>0)
    {
        return -1;  //left
    }
    else if (d<0)
    {
        return 1;   //right
    }
    else
    {
        return 0;  //on the segment
    }

}


/*
 This function tests if a point P lies inside a triangle T or not.
 return 0 if P lies inside the triangle T
 -1 if P lies outside the triangle T
 1 if P lies on the edge AB of the triangle T
 2 if P lies on the edge BC of the triangle T
 3 if P lies on the edge CA of the triangle T
 */
int InOutTriangle(meshPoint *P,meshTriangle *T)
{
    if (T==NULL) {
        return -2;
    }
    else {
        int s1,s2,s3;
        s1 = leftRightSegment(T.A,T.B,P);
        s2 = leftRightSegment(T.B,T.C,P);
        s3 = leftRightSegment(T.C,T.A,P);
        if (s1+s2+s3 == -3)
        {
            return 0;
        }
        else if (s1+s2+s3 == -2)
        {
            if (s1==0)
            {
                return 1;
            }
            else if (s2==0)
            {
                return 2;
            }
            else
            {
                return 3;
            }
        }
        else
        {
            return -1;
        }
    }
}


/*
 Loacte the point P, create the new elements and add them in the tree structure and return a pointer
 towards the triangle element.
 */

ElementLoc LocatePoint(ElementLoc *currentElement,meshPoint *P, int status)
{
    int inOut = InOutTriangle(P, currentElement.next1);
    if (inOut>=0)
    {
        *status = inOut;
        LocatePoint(currentElement.next1,*P,status)
    }
    else
    {
        inOut = InOutTriangle(P, currentElement.next2);
        if (inOut>=0)
        {
            *status = inOut;
            LocatePoint(currentElement.next2,*P,status)
        }
        else
        {
            inOut = InOutTriangle(P, currentElement.next3);
            if (inOut>=0)
            {
                *status = inOut;
                LocatePoint(currentElement.next3,*P,status)
            }
            else //leaf
            {
                addTreeToLeaf(currentElement,P);
                return currentElement;
            }
        }
    }

}


/*

 */
void addTreeToLeaf(ElementLoc *leaf,meshPoint *P)
{
    ElementLoc *T1 = ElementLocCreate();
    ElementLoc *T2 = ElementLocCreate();
    ElementLoc *T3 = ElementLocCreate();

    T1->T->A = P;
    T1->T->B = leaf->T->A;
    T1->T->C = leaf->T->B;

    T2->T->A = P;
    T2->T->B = leaf->T->A;
    T2->T->C = leaf->T->B;

    T3->T->A = P;
    T3->T->B = leaf->T->A;
    T3->T->C = leaf->T->B;

    leaf->next1 = T1;
    leaf->next2 = T2;
    leaf->next3 = T3;
}


ElementLoc *ElementLocCreate()
{
    ElementLoc *E = malloc(sizeof(ElementLoc));;
    E->T=NULL;
    E->next1=NULL;
    E->next2=NULL;
    E->next3=NULL;
    return E;
}




Mesh DelaunayTriangulation(meshPoint *P, int n)
{
    int i=0;
    srand(time(NULL));
    //random permutation ...
    for (i=n-1;i>=0;i--)
      {
        int j =  (int)(rand() / (double)RAND_MAX * (i - 1));
        double a = P[i];
        P[i] = P[j];
        P[j] = P[i];
      }
    //initialisation de D et T

    ElementLoc lastElem = ElementLocCreate();
    
    for (i=0;i<n;i++)
    {
        int *status;
        lastElem = LocatePoint(D.first, P[i],status);
        if (status == 0) //point dans le triangle
        {
            meshEdge *Edge1 = ???
            LegalizeEdge(P[i], E,lastElem);
            LegalizeEdge(P[i], E,lastElem);
            LegalizeEdge(P[i], E,lastElem);
        }
        else
        {
            //point sur un edge
        }

    }
}


void LegalizeEdge(meshPoint *R, meshPoint *I, meshPoint *J, meshPoint *K, )
{
    int stat = isInsideGen(E.A,E.B,P,);
}



}
