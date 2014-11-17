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
//TODO add your new functions in watson.h

/*
Don't forget to test the differents functions created with this one
*/ 
int testFunctions(int length)
{
int answer =0;
//answer = isInsideGen(thePoint[0],thePoint[1],thePoint[2],thePoint[3]);
//answer = leftRightSegment(thePoint[0],thePoint[3],thePoint[5]);
DelaunayTriangulation(thePoint, length );
//printf("point : %f,%f\n",thePoint[0]->x,thePoint[0]->y);
//printf("triangle : %f,%f\n",theTriangle[0]->E->origine->x,theTriangle[0]->E->origine->y);
//answer = InOutTriangle(thePoint[5],theTriangle[0]);

return answer;
}


 /*
 createPoints creates a list of meshPoint pointers based on the lists X and Y received 
 */
void initialiseThePoint(double *X, double *Y, int len)
{
//Tableau de pointeur c'est un pointeur de pointeur
	int length = len;
	thePoint = (meshPoint**) malloc(length * sizeof(meshPoint*));
	int i =0;
	for(i=0;i<length;i++)
	{
	thePoint[i] =(meshPoint*)malloc(sizeof(meshPoint));
	printf("X[i] : %f\n",X[i]);
	printf("Y[i] : %f\n",Y[i]);
    thePoint[i] = meshPointCreate(X[i], Y[i],i);
	}
	//printf("thePoint[0] : %f,%f\n",thePoint[0]->x,thePoint[0]->y);

 	//randomSwitch();
	
}


meshPoint *meshPointCreate(double x, double y, int numR)
{
  meshPoint *thePoint =  (meshPoint*)malloc(sizeof(meshPoint));
  
	//printf("X : %f\n",x);
	//printf("Y : %f\n",y);
  thePoint->x = x;
  thePoint->y = y;
  thePoint->num = i;
  
  return thePoint;

}


meshTriangle *meshTriangleCreate(meshEdge *ER)
{
	meshTriangle *theTriangle =  malloc(sizeof(meshTriangle));
	theTriangle->E = ER;
	return theTriangle;
}

meshEdge *meshEdgeCreate(meshTriangle *TR, meshEdge *twinR, meshEdge *nextR, meshPoint *origineR)
{
	meshEdge *theEdge =  malloc(sizeof(meshEdge));
	theEdge->T = TR;
	theEdge->twin = twinR;
	theEdge->next = nextR;
	theEdge->origine = origineR;
	return theEdge;

}


ElementLoc *ElementLocCreate(meshEdge *ER)
{
    ElementLoc *E = malloc(sizeof(ElementLoc));
    E->T=meshTriangleCreate(ER);
    E->next1=NULL;
    E->next2=NULL;
    E->next3=NULL;
    return E;
}

/*
void freeAll()
{
int i;
for(i=0;i<length;i++)
	{
    	free(thePoint[i]);
    }
    	free(thePoint);

}*/


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
int leftRightSegment(meshPoint *Origin, meshPoint *Dest, meshPoint *R)
{
    double d = (Dest->x-Origin->x)*(R->y-Origin->y) - (Dest->y-Origin->y)*(R->x-Origin->x);

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
int InOutTriangle(meshPoint *P,ElementLoc *currentElement)
{
	if (currentElement == NULL)
	{		
        return -2;
	}
    else {
    	meshTriangle *T = currentElement->T;
        int s1,s2,s3;
        s1 = leftRightSegment(T->E->origine,T->E->next->origine,P);
        s2 = leftRightSegment(T->E->next->origine,T->E->next->next->origine,P);
        s3 = leftRightSegment(T->E->next->next->origine,T->E->origine,P);
        if (s1+s2+s3 == -3)
        {
            return 0; //if inside
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
ElementLoc *LocatePoint(ElementLoc *currentElement,meshPoint *P, int *status)
{	

    int inOut = InOutTriangle(P, currentElement->next1);
    //printf("inout : %d\n",inOut);
    if (inOut>=0)
    {
        *status = inOut;
        LocatePoint(currentElement->next1,P,status);
    }
    else
    {
        inOut = InOutTriangle(P, currentElement->next2);
        if (inOut>=0)
        {
            *status = inOut;
            LocatePoint(currentElement->next2,P,status);
        }
        else;
        {
            inOut = InOutTriangle(P, currentElement->next3);
            if (inOut>=0)
            {
                *status = inOut;
                LocatePoint(currentElement->next3,P,status);
            }
            else //leaf
            {
            //printf("coucou\n");
                addTreeToLeaf(currentElement,P);
                return currentElement;
            }
        }
    }
	return NULL;
}



 
void addTreeToLeaf(ElementLoc *leaf,meshPoint *P)
{
    //on cree les nouveaux edges
    meshEdge *E11 = meshEdgeCreate(NULL,NULL,NULL,P);
    meshEdge *E12 = meshEdgeCreate(NULL,leaf->T->E->twin,NULL,leaf->T->E->origine);
    meshEdge *E13 = meshEdgeCreate(NULL,NULL,NULL,leaf->T->E->next->origine);
    meshEdge *E21 = meshEdgeCreate(NULL,NULL,NULL,P);
    meshEdge *E22 = meshEdgeCreate(NULL,leaf->T->E->next->twin,NULL,leaf->T->E->next->origine);
    meshEdge *E23 = meshEdgeCreate(NULL,NULL,NULL,leaf->T->E->next->next->origine);
    meshEdge *E31 = meshEdgeCreate(NULL,NULL,NULL,P);
    meshEdge *E32 = meshEdgeCreate(NULL,leaf->T->E->next->next->twin,NULL,leaf->T->E->next->next->origine);
    meshEdge *E33 = meshEdgeCreate(NULL,NULL,NULL,leaf->T->E->origine);
    
    //on les relie
    E11->next = E12;
    E12->next = E13;
    E13->next = E11;
    
    E21->next = E22;
    E22->next = E23;
    E23->next = E21;
    
    E31->next = E32;
    E32->next = E33;
    E33->next = E31;
    
    E11->twin = E33;
    E13->twin = E21;
    
    E21->twin = E13;
    E23->twin = E31;
    
    E31->twin = E23;
    E33->twin = E11;
    
    //on cree les nouveaux elements
    ElementLoc *T1 = ElementLocCreate(E12);
    ElementLoc *T2 = ElementLocCreate(E22);
    ElementLoc *T3 = ElementLocCreate(E32);
    
    //on associe les nouveaux edges aux triangles
    E11->T=T1->T;
    E12->T=T1->T;
    E13->T=T1->T;
    
    E21->T=T2->T;
    E22->T=T2->T;
    E23->T=T2->T;
    
    E31->T=T3->T;
    E32->T=T3->T;
    E33->T=T3->T;
    
    //on rajoute les nouveaux elements dans l'arbre de recherche
    leaf->next1 = T1;
    leaf->next2 = T2;
    leaf->next3 = T3;
}



void randomSwitch(int lengthR)
{
	int i=0;
    srand(time(NULL));
    //random permutation ...
    for (i=lengthR-1;i>=0;i--)
      {
        //printf("%f,%f\n",thePoint[i]->x,thePoint[i]->y);
      }
      meshPoint *a = malloc(sizeof(meshPoint));
    for (i=lengthR-1;i>=0;i--)
      {
      	double inter = (rand() / ( RAND_MAX / (i+1) ) ) ;
      	//double inter = (double)rand() / (double)RAND_MAX * (i-1);
        int j =  (int)(inter);
        //printf("%d",j);
       	a = thePoint[i];
        thePoint[i] = thePoint[j];
        thePoint[j] = a;
      }
      for (i=lengthR-1;i>=0;i--)
      {
      	//printf("coucou\n");
        //printf("%f,%f\n",thePoint[i]->x,thePoint[i]->y);
      }
      


}

//Mesh instead of void
void DelaunayTriangulation(meshPoint **P, int length)
{
    //initialisation de D et T
    int n = length;
	LocationTree *D = malloc(sizeof(LocationTree));
	meshEdge *EdgeInitA = meshEdgeCreate(NULL, NULL, NULL, thePoint[0]);	
	ElementLoc *FirstElem = ElementLocCreate(EdgeInitA);	
	D->first = FirstElem;
	//printf("D : %f,%f\n",D->first->T->E->origine->x,D->first->T->E->origine->y);
     meshEdge *EdgeInitB = meshEdgeCreate(D->first->T, NULL, NULL, thePoint[1]);
     meshEdge *EdgeInitC = meshEdgeCreate(D->first->T, NULL, EdgeInitA, thePoint[2]);
     D->first->T->E = EdgeInitA; 
     EdgeInitA->next = EdgeInitB;
     EdgeInitB->next = EdgeInitC;
   // printf("A : %f,%f\n",EdgeInitA->origine->x,EdgeInitA->origine->y);
    //printf("B : %f,%f\n",EdgeInitB->origine->x,EdgeInitB->origine->y);
   //printf("C : %f,%f\n",EdgeInitC->origine->x,EdgeInitC->origine->y);
	
   // ElementLoc *lastElem = ElementLocCreate(NULL);
 	int i =0;
    for (i=3;i<n;i++)
    {
        int *status = malloc(sizeof(int));
        ElementLoc *lastElem = LocatePoint(D->first, P[i],status);
        if (status == 0) //point dans le triangle
        {
            addTreeToLeaf(lastElem,P[i]);
            LegalizeEdge(P[i], lastElem->T->E,lastElem);
            LegalizeEdge(P[i], lastElem->T->E->next,lastElem);
            LegalizeEdge(P[i], lastElem->T->E->next->next,lastElem);
        }
        else
        {
            //point sur un edge
        }

    }
    //extract and return the array of triangles
    
}


/*
 
*/
void LegalizeEdge(meshPoint *R, meshEdge *E,ElementLoc *currentElement)
{
    int stat = isInsideGen(E->origine,E->next->origine,R,E->twin->next->next->origine);
    if (stat==1) //pivoter + appel de LegelizeEdge
    {
        //pivot, creation des nouveaux edges
        meshEdge *Enew11 = meshEdgeCreate(NULL,NULL,NULL,E->twin->next->next->origine);
        meshEdge *Enew12 = meshEdgeCreate(NULL,E->next->next->twin,NULL,E->next->next->origine);
        meshEdge *Enew13 = meshEdgeCreate(NULL,E->twin->next->twin,NULL,E->twin->next->origine);
        meshEdge *Enew21 = meshEdgeCreate(NULL,Enew11,NULL,E->next->next->origine);
        meshEdge *Enew22 = meshEdgeCreate(NULL,E->twin->next->next->twin,NULL,E->twin->next->next->origine);
        meshEdge *Enew23 = meshEdgeCreate(NULL,E->next->twin,NULL,E->next->origine);
        
        
        Enew11->twin=Enew21;
        
        Enew11->next=Enew12;
        Enew12->next=Enew13;
        Enew13->next=Enew11;
        
        Enew21->next=Enew22;
        Enew22->next=Enew23;
        Enew23->next=Enew21;
        
        //creations des nouveaux elements
        ElementLoc *T1 = ElementLocCreate(Enew11);
        ElementLoc *T2 = ElementLocCreate(Enew21);
        Enew11->T=T1->T;
        Enew12->T=T1->T;
        Enew13->T=T1->T;
        Enew21->T=T2->T;
        Enew22->T=T2->T;
        Enew23->T=T2->T;
        
        //ajout dans la structure
        currentElement->next1 = T1;
        currentElement->next2 = T2;
        
        //appel de LegalizeEdge sur les deux edges à risques
        LegalizeEdge(R,T1->T->E->next->next,T1);
        LegalizeEdge(R,T2->T->E->next,T2);
    }
    
}


