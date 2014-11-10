#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
//#include <GL/glfw.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include "watson.h"

//Declaration of global variables
static meshPoint **thePoint;
static int length;
//TODO add your new functions in watson.h

/*
Don't forget to test the differents functions created with this one
*/ 
int testFunctions()
{
int answer =0;
//answer = isInsideGen(thePoint[0],thePoint[1],thePoint[2],thePoint[3]);
//answer = leftRightSegment(thePoint[0],thePoint[3],thePoint[3]);
DelaunayTriangulation(thePoint[0],length);
return answer;
}


 /*
 createPoints creates a list of meshPoint pointers based on the lists X and Y received 
 */
void createPoints(double *X, double *Y, int len)
{
//Tableau de pointeur c'est un pointeur de pointeur
	thePoint = (meshPoint**) malloc(length * sizeof(meshPoint*));
	length = len;
	int i =0;
	for(i=0;i<length;i++)
	{
	thePoint[i] =(meshPoint*)malloc(sizeof(meshPoint));
    thePoint[i] =  meshPointCreate(X[i], Y[i]);
	}
	
}

meshPoint *meshPointCreate(double x, double y)
{
  meshPoint *thePoint =  malloc(sizeof(meshPoint));
  thePoint->x = x;
  thePoint->y = y;
  
  return thePoint;

}
void freeAll()
{
int i;
for(i=0;i<length;i++)
	{
    	free(thePoint[i]);
    }
    	free(thePoint);

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
        s1 = leftRightSegment(T->A,T->B,P);
        s2 = leftRightSegment(T->B,T->C,P);
        s3 = leftRightSegment(T->C,T->A,P);
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
 Locate the point P, create the new elements and add them in the tree structure and return a pointer
 towards the triangle element.
 */

ElementLoc *LocatePoint(ElementLoc *currentElement,meshPoint *P, int status)
{
    int inOut = InOutTriangle(P, currentElement->next1->T);
    if (inOut>=0)
    {
        status = inOut;
        LocatePoint(currentElement->next1,P,status);
    }
    else
    {
        inOut = InOutTriangle(P, currentElement->next2->T);
        if (inOut>=0)
        {
            status = inOut;
            LocatePoint(currentElement->next2,P,status);
        }
        else
        {
            inOut = InOutTriangle(P, currentElement->next3->T);
            if (inOut>=0)
            {
                status = inOut;
                LocatePoint(currentElement->next3,P,status);
            }
            else //leaf
            {
                addTreeToLeaf(currentElement,P);
                return currentElement;
            }
        }
    }
    //TO COMPILE
	return currentElement;
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

//Mesh instead of void
void DelaunayTriangulation(meshPoint *P, int n)
{
    int i=0;
    srand(time(NULL));
    //random permutation ...
    for (i=length-1;i>=0;i--)
      {
        printf("%f,%f\n",thePoint[i]->x,thePoint[i]->y);
      }
      meshPoint *a = malloc(sizeof(meshPoint));
    for (i=length-1;i>=0;i--)
      {
        int j =  (int)(rand() / (double)RAND_MAX) * (i-1);
       	a = thePoint[i];
        thePoint[i] = thePoint[j];
        thePoint[j] = a;
      }
      for (i=length-1;i>=0;i--)
      {
      	printf("coucou\n");
        printf("%f,%f\n",thePoint[i]->x,thePoint[i]->y);
      }
      
 }     
    
/*
    //initialisation de D et T
	LocationTree *D = malloc(sizeof(LcationTree);
	*D.first = NULL;

    ElementLoc lastElem = ElementLocCreate();
    
    for (i=0;i<n;i++)
    {
        int *status;
        lastElem = LocatePoint(D.first, P[i],status);
        if (status == 0) //point dans le triangle
        {
            meshEdge *Edge1 = NULL;
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



void LegalizeEdge(meshPoint *R, meshEdge *E, ElementLoc *currentElement)
{
    int stat=0;
    if (currentElement->T==E->right)
    {
        if (E->left->A==E->A || E->left->A==E->B)
        {
            if (E->left->B==E->A || E->left->B==E->B)
            {
                stat = isInsideGen(E->A,E->B,P,E->left->C);
                if (stat==1)
                {
                    //pivoter
                    //appeler LegalizeEdge sur les 2 edges
                }
            }
            esle
            {
                stat = isInsideGen(E->A,E->B,P,E->left->B);
                if (stat==1)
                {
                    //pivoter
                    //appeler LegalizeEdge sur les 2 edges
                }
            }
        }
        esle
        {
            stat = isInsideGen(E->A,E->B,P,E->left->A);
            if (stat==1)
            {
                //pivoter
                //appeler LegalizeEdge sur les 2 edges
            }
        }
    }
    else
    {
        if (E->right->A==E->A || E->right->A==E->B)
        {
            if (E->right->B==E->A || E->right->B==E->B)
            {
                stat = isInsideGen(E->A,E->B,P,E->right->C);
                if (stat==1)
                {
                    //pivoter
                    //appeler LegalizeEdge sur les 2 edges
                }
            }
            esle
            {
                stat = isInsideGen(E->A,E->B,P,E->right->B);
                if (stat==1)
                {
                    //pivoter
                    //appeler LegalizeEdge sur les 2 edges
                }
            }
        }
        esle
        {
            stat = isInsideGen(E->A,E->B,P,E->right->A);
            if (stat==1)
            {
                //pivoter
                //appeler LegalizeEdge sur les 2 edges
            }
        }

    }
}

*/








