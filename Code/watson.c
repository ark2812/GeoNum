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



 
void addTreeToLeaf(ElementLoc *leaf,meshPoint *P)
{
    //on cree les nouveaux elements
    ElementLoc *T1 = ElementLocCreate(leaf->T->E);
    ElementLoc *T2 = ElementLocCreate(leaf->T->E->next);
    ElementLoc *T3 = ElementLocCreate(leaf->T->E->next->next);
    
    //on cree les nouveaux edges
    meshEdge *E11 = meshEdgeCreate(T1->T,NULL,T1->T->E,P);
    meshEdge *E13 = meshEdgeCreate(T1->T,NULL,E11,T1->T->E->next->origine);
    meshEdge *E21 = meshEdgeCreate(T2->T,NULL,T2->T->E,P);
    meshEdge *E23 = meshEdgeCreate(T2->T,NULL,E21,T2->T->E->next->origine);
    meshEdge *E31 = meshEdgeCreate(T3->T,NULL,T3->T->E,P);
    meshEdge *E33 = meshEdgeCreate(T3->T,NULL,E31,T3->T->E->next->origine);
    
    //on les relie
    E11->twin = E33;
    E13->twin = E21;
    E21->twin = E13;
    E23->twin = E31;
    E31->twin = E23;
    E33->twin = E11;
    
    // on modifie les anciens edges
    //on les associe aux nouveaux triangles
    T1->T->E->T = T1->T;
    T2->T->E->T = T2->T;
    T3->T->E->T = T3->T;
    //on les relie aux nouveaux edges
    T1->T->E->next = E13;
    T2->T->E->next = E23;
    T3->T->E->next = E33;
    //fini :)
    
    
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
 /*   
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
}*/
}


 /*
 //mouai bof c'est pas terrible...
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
                    //changer les triangles
                    ElementLoc *T1 = ElementLocCreate();
                    ElementLoc *T2 = ElementLocCreate();
                    T1->T->A=E->left->C;
                    T1->T->B=E->A;
                    T1->T->C=R;
                    
                    T2->T->A=E->left->C;
                    T2->T->B=R;
                    T2->T->C=E->B;
                    
                    currentElement->next1=T1;
                    currentElement->next2=T2;
                    E->left->next1=T1;
                    E->left->next2=T2;
                    
                    //pivoter
                    meshEdge *E2=malloc(sizeof(meshEdge));
                    E2->A=E->left->C;
                    E2->B=E->B;
                    
                    meshEdge *E3=malloc(sizeof(meshEdge));
                    E3->A=E->left->C;
                    E3->B=E->A;
                    //appeler LegalizeEdge sur les 2 edges
                    LegalizeEdge(R, E2, T2);
                    LegalizeEdge(R, E3, T1);
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



/*
void LegalizeEdge(meshPoint *R, meshEdge *E)
{
    int stat = isInsideGen(E->A,E->B,R,E->twin->Oposite);
    if (stat==1) //pivoter + appel de LegelizeEdge
    {
        //pivot
        meshEdgeCreate Enew1 = meshEdgeCreate(NULL,NULL,E->twin->Oposite,R,E->A);
        meshEdgeCreate Enew2 = meshEdgeCreate(NULL,Enew1,R,E->twin->Oposite,E->B);
        Enew1->twin=Enew2;
        meshTriangle T1 = meshTriangleCreate(Enew1);
    }
    
}*/


