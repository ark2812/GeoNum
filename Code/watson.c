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
    double d = (B->x - A->x)*(R->y - A->y) - (B->y - A->y)*(R->x - A->x);
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
        s1 = leftRightSegment(T->E->origine,T->E->next->origine,P);
        s2 = leftRightSegment(T->E->next->origine,T->E->next->next->origine,P);
        s3 = leftRightSegment(T->E->next->next->origine,T->E->origine,P);
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
ElementLoc *LocatePoint(ElementLoc *currentElement,meshPoint *P, int *status)
{
    int inOut = InOutTriangle(P, currentElement->next1->T);
    if (inOut>=0)
    {
        *status = inOut;
        LocatePoint(currentElement->next1,P,status);
    }
    else
    {
        inOut = InOutTriangle(P, currentElement->next2->T);
        if (inOut>=0)
        {
            *status = inOut;
            LocatePoint(currentElement->next2,P,status);
        }
        else
        {
            inOut = InOutTriangle(P, currentElement->next3->T);
            if (inOut>=0)
            {
                *status = inOut;
                LocatePoint(currentElement->next3,P,status);
            }
            else //leaf
            {
                addTreeToLeaf(currentElement,P);
                return currentElement;
            }
        }
    }
	return currentElement;
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



void randomSwitch()
{
	int i=0;
    srand(time(NULL));
    //random permutation ...
    for (i=length-1;i>=0;i--)
      {
        //printf("%f,%f\n",thePoint[i]->x,thePoint[i]->y);
      }
      meshPoint *a = malloc(sizeof(meshPoint));
    for (i=length-1;i>=0;i--)
      {
      	double inter = (rand() / ( RAND_MAX / (i+1) ) ) ;
      	//double inter = (double)rand() / (double)RAND_MAX * (i-1);
        int j =  (int)(inter);
        printf("%d",j);
       	a = thePoint[i];
        thePoint[i] = thePoint[j];
        thePoint[j] = a;
      }
      for (i=length-1;i>=0;i--)
      {
      	//printf("coucou\n");
        //printf("%f,%f\n",thePoint[i]->x,thePoint[i]->y);
      }
      


}

//Mesh instead of void
void DelaunayTriangulation(meshPoint *P, int n)
{
      

    //initialisation de D et T
	LocationTree *D = malloc(sizeof(LocationTree));
	
	D->first = NULL;

    ElementLoc *lastElem = ElementLocCreate(NULL);
   	
    meshTriangle *InitialTriangle = meshTriangleCreate(NULL);
    meshEdge *EdgeInitA = meshEdgeCreate(InitialTriangle, NULL, NULL, thePoint[0]);
     meshEdge *EdgeInitB = meshEdgeCreate(InitialTriangle, NULL, NULL, thePoint[1]);
     meshEdge *EdgeInitC = meshEdgeCreate(InitialTriangle, NULL, EdgeInitA, thePoint[2]);
     InitialTriangle->E = EdgeInitA; 
     EdgeInitA->next = EdgeInitB;
     EdgeInitB->next = EdgeInitC;
    printf("A : %f,%f",EdgeInitA->origine->x,EdgeInitA->origine->y);
    printf("B : %f,%f",EdgeInitB->origine->x,EdgeInitB->origine->y); 
    printf("C : %f,%f",EdgeInitC->origine->x,EdgeInitC->origine->y);
 	randomSwitch();
    
    //initialize structure D
    for (i=0;i<n;i++)
    {
        int *status;
        ElementLoc *lastElem = LocatePoint(D.first, P[i],status);
        if (status == 0) //point dans le triangle
        {
            addTreeToLeaf(lastElem,P);
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
    int stat = isInsideGen(E->origine,E->next->origine,R,E->twin->Oposite);
    if (stat==1) //pivoter + appel de LegelizeEdge
    {
        //pivot, creation des nouveaux edges
        meshEdgeCreate *Enew11 = meshEdgeCreate(NULL,NULL,NULL,E->twin->next->next->origine);
        meshEdgeCreate *Enew12 = meshEdgeCreate(NULL,E->next->next->twin,NULL,E->next->next->origine);
        meshEdgeCreate *Enew13 = meshEdgeCreate(NULL,E->twin->next->twin,NULL,E->twin->next->origine);
        meshEdgeCreate *Enew21 = meshEdgeCreate(NULL,Enew11,NULL,E->next->next->origine);
        meshEdgeCreate *Enew22 = meshEdgeCreate(NULL,E->twin->next->next->twin,NULL,E->twin->next->next->origine);
        meshEdgeCreate *Enew23 = meshEdgeCreate(NULL,E->next->twin,NULL,E->next->origine);
        
        
        Enew11->twin=Enew21;
        
        Enew11->next=Enew12;
        Enew12->next=Enew13;
        Enew13->next=Enew11;
        
        Enew21->next=Enew22;
        Enew22->next=Enew23;
        Enew23->next=Enew21;
        
        //creations des nouveaux elements
        ElementLoc *T1 = *ElementLocCreate(Enew11);
        ElementLoc *T2 = *ElementLocCreate(Enew21);
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
        LegalizeEdge(R,T1->E->next->next,T1);
        LegalizeEdge(R,T2->E->next,T2);
    }
    
}

















