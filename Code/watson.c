#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include "watson.h"

// include the robust methods as explained in the report
#include "robustFunctions.c"

//Declaration of global variables
static meshPoint **thePoint;
int COUNT;

/* ======================================== Initialisation of data structures ================================================ */

 /*
 createPoints creates a list of meshPoint pointers based on the lists X and Y received 
 */
meshPoint **initialiseThePoint(double *X, double *Y, int len)
{
//Tableau de pointeur c'est un pointeur de pointeur
	int length = len;
	int lengthTot = length+3; // +3 to add the big triangle
	if(RAN==1){
	randomSwitch(X,Y,length);}
	double minX, minY, maxX, maxY;
	minX = minY = maxX = maxY = 0;
	thePoint = (meshPoint**) malloc(lengthTot * sizeof(meshPoint*));
	int i =0;
    
	for(i=0;i<length;i++)
	{
	
	thePoint[i+3] =(meshPoint*)malloc(sizeof(meshPoint));
    thePoint[i+3] = meshPointCreate(X[i], Y[i],i+3);
    
    if(X[i]>maxX){
    	maxX = X[i];}
    if(X[i]<minX){
    	minX = X[i];}
    if(Y[i]>maxY){
    	maxY = Y[i];}
    if(Y[i]<minY){
    	minY = Y[i];}
	}
    
    thePoint[0] = meshPointCreate(minX - 100000*(maxX-minX),minY-10*(maxY-minY),0);
    thePoint[1] = meshPointCreate(maxX + 10000*(maxX-minX),minY-10*(maxY-minY),1);
    thePoint[2] = meshPointCreate((maxX+minX)/2,maxY+10000*(maxY-minY),2);
	
	
	return thePoint;	
}


/* 
	Random permutation of the set of points received
*/
void randomSwitch(double *X, double *Y, int length)
{
	int i=0;
    srand(time(NULL));
      double a = 0;      
      double b = 0;
    for (i=length-1;i>=0;i--)
      {
      	int inter = (rand() % (i+1));
        int j =  inter;
       	a = X[i];
       	b = Y[i];
        X[i] = X[j];  
        Y[i] = Y[j];
        X[j] = a;     
        Y[j] = b;
      }
}

/* 
	Create a meshPoint structure
*/
meshPoint *meshPointCreate(double x, double y, int numR)
{
  meshPoint *thePoint =  (meshPoint*)malloc(sizeof(meshPoint));
  
  thePoint->x = x;
  thePoint->y = y;
  thePoint->num = numR;
  
  return thePoint;

}

/* 
	Create a meshTriangle structure 
*/
meshTriangle *meshTriangleCreate(meshEdge *ER, ElementLoc *E)
{
	meshTriangle *theTriangle =  malloc(sizeof(meshTriangle));
	theTriangle->E = ER;
    theTriangle->Elem=E;
	return theTriangle;
}

/* 
	Create a meshEdge structure 
*/
meshEdge *meshEdgeCreate(meshTriangle *TR, meshEdge *twinR, meshEdge *nextR, meshPoint *origineR)
{
	meshEdge *theEdge =  malloc(sizeof(meshEdge));
	theEdge->T = TR;
	theEdge->twin = twinR;
	theEdge->next = nextR;
	theEdge->origine = origineR;
	return theEdge;

}

/* 
	Create a ElementLoc structure 
*/
ElementLoc *ElementLocCreate(meshEdge *ER)
{
    ElementLoc *E = malloc(sizeof(ElementLoc));
    E->T=meshTriangleCreate(ER,E);
    E->SLeaf = StackLeafCreate(E);
    E->next1=NULL;
    E->next2=NULL;
    E->next3=NULL;
    return E;
}




/* ======================================== MAIN FUNCTIONS ================================================ */ 

/*
	Delaunay Triangulation launches the actual triangulation
*/
void DelaunayTriangulation(meshPoint **P, int length, int evol)
{
    // D initialisation 
    int n = length;
	LocationTree *D = malloc(sizeof(LocationTree));
	meshEdge *EdgeInitA = meshEdgeCreate(NULL, NULL, NULL, thePoint[0]);	
	ElementLoc *FirstElem = ElementLocCreate(EdgeInitA);	
	D->first = FirstElem;
	
    meshEdge *EdgeInitB = meshEdgeCreate(D->first->T, NULL, NULL, thePoint[1]);
    meshEdge *EdgeInitC = meshEdgeCreate(D->first->T, NULL, EdgeInitA, thePoint[2]);
    EdgeInitA->next = EdgeInitB;
    EdgeInitB->next = EdgeInitC;
    
    TheStack *S = TheStackCreate();
    
 	int i =0;
    ElementLoc *lastElem = malloc(sizeof(ElementLoc));
    
    for (i=3;i<n;i++)
    {
        int status=0;
        
        lastElem = LocatePoint(D->first, P[i],&status);
       
        
        if (status == 0) //point in the triangle
        {
          
            
            addTreeToLeaf(lastElem,P[i],S);
            
          LegalizeEdge(P[i], lastElem->next1->T->E,lastElem->next1,S);
          LegalizeEdge(P[i], lastElem->next3->T->E,lastElem->next3,S);
          LegalizeEdge(P[i], lastElem->next2->T->E,lastElem->next2,S);
            

        }
        else //point on an edge
        {
            //status=1 -> it's on the edge E of the currentElement
            //status=2 -> it's on the edge E->next of the currentElement
            //status=3 -> it's on the edge E->next->next of the currentElement
            if (status==1)
            {
                addTreeToLeafEdge(lastElem,P[i],lastElem->T->E,S);
                LegalizeEdge(P[i], lastElem->next1->T->E,lastElem->next1,S);
                LegalizeEdge(P[i], lastElem->next2->T->E,lastElem->next2,S);
                
                LegalizeEdge(P[i], lastElem->T->E->twin->T->Elem->next1->T->E ,lastElem->T->E->twin->T->Elem->next1,S);
                LegalizeEdge(P[i], lastElem->T->E->twin->T->Elem->next1->T->E ,lastElem->T->E->twin->T->Elem->next1,S);
            }
            else if (status==2)
            {
                addTreeToLeafEdge(lastElem,P[i],lastElem->T->E->next,S);
                
                LegalizeEdge(P[i], lastElem->next1->T->E,lastElem->next1,S);
                LegalizeEdge(P[i], lastElem->next2->T->E,lastElem->next2,S);
                
                LegalizeEdge(P[i], lastElem->T->E->next->twin->T->Elem->next1->T->E,lastElem->T->E->next->twin->T->Elem->next1,S);
                LegalizeEdge(P[i], lastElem->T->E->next->twin->T->Elem->next2->T->E,lastElem->T->E->next->twin->T->Elem->next2,S);
            }
            else if (status==3)
            {
                addTreeToLeafEdge(lastElem,P[i],lastElem->T->E->next->next,S);
                
                LegalizeEdge(P[i], lastElem->next1->T->E,lastElem->next1,S);
                LegalizeEdge(P[i], lastElem->next2->T->E,lastElem->next2,S);
                
                LegalizeEdge(P[i], lastElem->T->E->next->next->twin->T->Elem->next1->T->E ,lastElem->T->E->next->next->twin->T->Elem->next1,S);
                LegalizeEdge(P[i], lastElem->T->E->next->next->twin->T->Elem->next2->T->E ,lastElem->T->E->next->next->twin->T->Elem->next2,S);
            }
            else
            {
                printf("ERROR :( \n");
            }
            
        }
        if (evol == 1){
        EvolutionWriteFile(S,i-3);
        }
    }
    
    //extract and return the array of triangles
    writeFile2(S,"Triangles.csv");
    
}


/*
	LegalizeEdge performs the legalization of the illegal edges.
 */
void LegalizeEdge(meshPoint *R, meshEdge *E,ElementLoc *currentElement, TheStack *S)
{
    if (E->twin != NULL) {// we stop switching if we are on a border edge 
    
        int stat = isInsideGen(E->origine,E->next->origine,R,E->twin->next->next->origine);
        
        if (stat==1) //Switch and call to LegalizeEdge
        {
            meshEdge *Enew11 = meshEdgeCreate(NULL,NULL,NULL,E->twin->next->next->origine);
            meshEdge *Enew21 = meshEdgeCreate(NULL,Enew11,NULL,E->next->next->origine);
            //Creation of the new elements
            ElementLoc *T1 = ElementLocCreate(Enew11);
            ElementLoc *T2 = ElementLocCreate(Enew21);
            //add in the structure
            currentElement->next1 = T1;
            currentElement->next2 = T2;
            //Add 2 link in the neighbour triangle which also have some change
           E->twin->T->Elem->next1 = T1;
           E->twin->T->Elem->next2 = T2;
                    
            //Creation of new edges
            meshEdge *Enew12 = meshEdgeCreate(NULL,E->next->next->twin,NULL,E->next->next->origine);
            meshEdge *Enew13 = meshEdgeCreate(NULL,E->twin->next->twin,NULL,E->twin->next->origine);
            meshEdge *Enew22 = meshEdgeCreate(NULL,E->twin->next->next->twin,NULL,E->twin->next->next->origine);
            meshEdge *Enew23 = meshEdgeCreate(NULL,E->next->twin,NULL,E->next->origine);
            
            
            Enew11->twin=Enew21;
            
            Enew11->next=Enew12;
            Enew12->next=Enew13;
            Enew13->next=Enew11;
            
            Enew21->next=Enew22;
            Enew22->next=Enew23;
            Enew23->next=Enew21;

            Enew11->T=T1->T;
            Enew12->T=T1->T;
            Enew13->T=T1->T;
            
            Enew21->T=T2->T;
            Enew22->T=T2->T;
            Enew23->T=T2->T;
            
            //twin of twin 
            if (E->next->next->twin != NULL) {
                E->next->next->twin->twin = Enew12;
            }
            if (E->twin->next->twin != NULL) {
                E->twin->next->twin->twin = Enew13;
            }
            if (E->twin->next->next->twin != NULL) {
                E->twin->next->next->twin->twin = Enew22;
            }
            if (E->next->twin != NULL) {
                E->next->twin->twin = Enew23;
            }
            
            
            // leafs stack modification
            DeleteStackElement(E->twin->T->Elem->SLeaf,S);
            DeleteStackElement(currentElement->SLeaf,S);
            AddStackElement(T1->SLeaf,S);
            AddStackElement(T2->SLeaf,S);
                       
            
            // LegalizeEdge son the two risky edges
            LegalizeEdge(R,T1->T->E->next->next,T1, S);
            LegalizeEdge(R,T2->T->E->next,T2, S);
          
        }
    }
}






/* ======================================== NODE INSERTION FUNCTIONS ================================================ */ 

/*
 Locate the point P, create the new elements and add them in the tree structure and return a pointer
 towards the triangle element.
*/
ElementLoc *LocatePoint(ElementLoc *currentElement,meshPoint *P, int *status)
{    
    if (currentElement->next1==NULL) { //leaf

        return currentElement;
    }
    else {
        int inOut = InOutTriangle(P, currentElement->next1);
        if (inOut>=0)
        {
            *status = inOut;
            return LocatePoint(currentElement->next1,P,status);
        }
        else
        {
            inOut = InOutTriangle(P, currentElement->next2);
            if (inOut>=0)
            {
                *status = inOut;
                return LocatePoint(currentElement->next2,P,status);
            }
            else
            {
                inOut = InOutTriangle(P, currentElement->next3);
                if (inOut>=0)
                {
                    *status = inOut;
                    return LocatePoint(currentElement->next3,P,status);
                }
                else{
                	printf("We do not deal this type of event yet \n");
                    *status = -1;
                }
            }
        }
        
    	return NULL;
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
	addTreeToLeaf adds the point P to the research tree. It creates the new triangles/edges and add the three "children" to the parent elements leaf.
*/
void addTreeToLeaf(ElementLoc *leaf,meshPoint *P,TheStack *S)
{
    //Create the new edges
    meshEdge *E11 = meshEdgeCreate(NULL,NULL,NULL,P);
    meshEdge *E12 = meshEdgeCreate(NULL,leaf->T->E->twin,NULL,leaf->T->E->origine);
    meshEdge *E13 = meshEdgeCreate(NULL,NULL,NULL,leaf->T->E->next->origine);
    meshEdge *E21 = meshEdgeCreate(NULL,NULL,NULL,P);
    meshEdge *E22 = meshEdgeCreate(NULL,leaf->T->E->next->twin,NULL,leaf->T->E->next->origine);
    meshEdge *E23 = meshEdgeCreate(NULL,NULL,NULL,leaf->T->E->next->next->origine);
    meshEdge *E31 = meshEdgeCreate(NULL,NULL,NULL,P);
    meshEdge *E32 = meshEdgeCreate(NULL,leaf->T->E->next->next->twin,NULL,leaf->T->E->next->next->origine);
    meshEdge *E33 = meshEdgeCreate(NULL,NULL,NULL,leaf->T->E->origine);
    
    //Modification of the edges  of the adjacent triangles in order to be the associated with the new edges 
    if (E12->twin != NULL) {
        E12->twin->twin = E12;
    }
    if (E22->twin != NULL) {
        E22->twin->twin = E22;
    }
    if (E32->twin != NULL) {
        E32->twin->twin = E32;
    }
    
    //Connect
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
    
    //Creation of the new elements
    ElementLoc *T1 = ElementLocCreate(E12);
    ElementLoc *T2 = ElementLocCreate(E22);
    ElementLoc *T3 = ElementLocCreate(E32);
    
    //Link between the new edge and the triangle
    E11->T=T1->T;
    E12->T=T1->T;
    E13->T=T1->T;
    
    E21->T=T2->T;
    E22->T=T2->T;
    E23->T=T2->T;
    
    E31->T=T3->T;
    E32->T=T3->T;
    E33->T=T3->T;
    
    //New edges in the search tree
    leaf->next1 = T1;
    leaf->next2 = T2;
    leaf->next3 = T3;
    
    //leafs stack
    DeleteStackElement(leaf->SLeaf,S);
    AddStackElement(T1->SLeaf,S);
    AddStackElement(T2->SLeaf,S);
    AddStackElement(T3->SLeaf,S);
}



/*
	addTreeToLeafEdge adds the point P to the research tree in the case where P is located on a edge. It creates the new triangles/edges and add the two "children" to the parent elements leaf.
*/
void addTreeToLeafEdge(ElementLoc *leaf,meshPoint *P,meshEdge *E,TheStack *S)
{
    // Creation of the new edges
    meshEdge *Ecurrent11 = meshEdgeCreate(NULL,NULL,NULL,P);
    meshEdge *Ecurrent12 = meshEdgeCreate(NULL,NULL,NULL,E->next->origine);
    meshEdge *Ecurrent13 = meshEdgeCreate(NULL,NULL,NULL,E->next->next->origine);
    meshEdge *Ecurrent21 = meshEdgeCreate(NULL,NULL,NULL,E->next->next->origine);
    meshEdge *Ecurrent22 = meshEdgeCreate(NULL,NULL,NULL,E->origine);
    meshEdge *Ecurrent23 = meshEdgeCreate(NULL,NULL,NULL,P);
    
    meshEdge *Etwin11 = meshEdgeCreate(NULL,NULL,NULL,E->twin->origine);
    meshEdge *Etwin12 = meshEdgeCreate(NULL,NULL,NULL,P);
    meshEdge *Etwin13 = meshEdgeCreate(NULL,NULL,NULL,E->twin->next->next->origine);
    meshEdge *Etwin21 = meshEdgeCreate(NULL,NULL,NULL,P);
    meshEdge *Etwin22 = meshEdgeCreate(NULL,NULL,NULL,E->twin->next->origine);
    meshEdge *Etwin23 = meshEdgeCreate(NULL,NULL,NULL,E->twin->next->next->origine);
    
    //Connect
    Ecurrent11->next = Ecurrent12;
    Ecurrent12->next = Ecurrent13;
    Ecurrent13->next = Ecurrent11;
    
    Ecurrent21->next = Ecurrent22;
    Ecurrent22->next = Ecurrent23;
    Ecurrent23->next = Ecurrent21;
    
    Etwin11->next = Etwin12;
    Etwin12->next = Etwin13;
    Etwin13->next = Etwin11;
    
    Etwin21->next = Etwin22;
    Etwin22->next = Etwin23;
    Etwin23->next = Etwin21;
    
    //twin
    Ecurrent11->twin = Etwin11;
    Ecurrent12->twin = E->next->twin;
    Ecurrent13->twin = Ecurrent23;
    
    Ecurrent21->twin = E->next->next->twin;
    Ecurrent22->twin = Etwin21;
    Ecurrent23->twin = Ecurrent13;
    
    Etwin11->twin = Ecurrent11;
    Etwin12->twin = Etwin23;
    Etwin13->twin = E->twin->next->next->twin;
    
    Etwin21->twin = Ecurrent22;
    Etwin22->twin = E->twin->next->twin;
    Etwin23->twin = Etwin12;
    
    //twin of twin
    if (Ecurrent12->twin != NULL) {
        Ecurrent12->twin->twin=Ecurrent12;
    }
    if (Ecurrent21->twin != NULL) {
        Ecurrent21->twin->twin = Ecurrent21;
    }
    if (Etwin13->twin != NULL) {
        Etwin13->twin->twin = Etwin13;
    }
    if (Etwin22->twin != NULL) {
        Etwin22->twin->twin = Etwin22;
    }
    
    //Creation of new elements
    //with edge the ones we're gonna legalize 
    ElementLoc *T11 = ElementLocCreate(Ecurrent12);
    ElementLoc *T12 = ElementLocCreate(Ecurrent21);
    ElementLoc *T21 = ElementLocCreate(Etwin13);
    ElementLoc *T22 = ElementLocCreate(Etwin22);
    
    //Link new edge with triangle
    Ecurrent11->T = T11->T;
    Ecurrent12->T = T11->T;
    Ecurrent13->T = T11->T;
    
    Ecurrent21->T = T12->T;
    Ecurrent22->T = T12->T;
    Ecurrent23->T = T12->T;
    
    Etwin11->T = T21->T;
    Etwin12->T = T21->T;
    Etwin13->T = T21->T;
    
    Etwin21->T = T22->T;
    Etwin22->T = T22->T;
    Etwin23->T = T22->T;
    
    //next of element
    leaf->next1 = T11;
    leaf->next2 = T12;
    
    E->twin->T->Elem->next1 = T21;
    E->twin->T->Elem->next2 = T21;
    
    // leafs stack
    DeleteStackElement(leaf->SLeaf,S);
    DeleteStackElement(E->twin->T->Elem->SLeaf,S);
    AddStackElement(T11->SLeaf,S);
    AddStackElement(T12->SLeaf,S);
    AddStackElement(T21->SLeaf,S);
    AddStackElement(T22->SLeaf,S);
    
}





/* ======================================== PRIMITE GEOMETRIC FUNCTIONS ================================================ */ 

/*
 isInsideGen returns 1 if the point R is inside a circle made of the 3
 points 1, 2 and 3 in conterclock order. 0 if outside of the circle and -1 if there is a
 problem (3 points aligned etc.).
 */
int isInsideGen( meshPoint *thePoint1,  meshPoint *thePoint2,
                meshPoint *thePoint3,  meshPoint *thePointR)
{
	
	
    // put the points 1,2,3 in conterclock configuartion :
    double x1,y1,x2,y2,x3,y3,xR,yR;
    int inside=0;
    int sens = leftRightSegment(thePoint1, thePoint2, thePoint3);
    if (sens>0)//3 right to 12 we switch 2 and 3 in order to have them in conterclock sens.
    {
        x1 = thePoint1->x;
        y1 = thePoint1->y;
        x3 = thePoint2->x;
        y3 = thePoint2->y;
        x2 = thePoint3->x;
        y2 = thePoint3->y;
        xR = thePointR->x;
        yR = thePointR->y;
    }
    else
    {
        x1 = thePoint1->x;
        y1 = thePoint1->y;
        x2 = thePoint2->x;
        y2 = thePoint2->y;
        x3 = thePoint3->x;
        y3 = thePoint3->y;
        xR = thePointR->x;
        yR = thePointR->y;
    }
    
    if (ROBUST==0) //non-robust implementation 
    {
            
    double det1 = (x1-xR)*(y2-yR)*((x3-xR)*(x3-xR) + (y3-yR)*(y3-yR));
    double det2 = (y1-yR)*((x2-xR)*(x2-xR) + (y2-yR)*(y2-yR))*(x3-xR);
    double det3 = (x2-xR)*(y3-yR)*((x1-xR)*(x1-xR) + (y1-yR)*(y1-yR));
    double det4 = (x3-xR)*(y2-yR)*((x1-xR)*(x1-xR) + (y1-yR)*(y1-yR));
    double det5 = (x2-xR)*(y1-yR)*((x3-xR)*(x3-xR) + (y3-yR)*(y3-yR));
    double det6 = (y3-yR)*((x2-xR)*(x2-xR) + (y2-yR)*(y2-yR))*(x1-xR);
    
    int tolerance = 1e-6;
    if(det1+det2+det3-det4-det5-det6 + tolerance> 0)
    {
        inside = 1;
    }
    else
    {
        inside = 0;
    }

    return inside ;
    }
    else 
    {
    
     //robust implementation 
     
    double A[2];
    double B[2];
    double C[2];
    double D[2];
    
    A[0] = x1;
    A[1] = y1;

    B[0] = x2;
    B[1] = y2;
    
    C[0] = x3;
    C[1] = y3;
    
    D[0] = xR;
    D[1] = yR;
    
    double robust = incircle(A, B, C, D);
    if(robust > 0)
    {
        inside = 1;
    }
    else
    {
        inside = 0;
    }
    
    return inside ;
    }
}


/*
 This function tests if the point R lies at the right or at the left of segment (oriented) AB.
 Return  1 if it lies on the right
 -1 if it lies on the left
 0 if it lies on the segment
 */
int leftRightSegment(meshPoint *Origin, meshPoint *Dest, meshPoint *R)
{   
    // == Sign of a determinant  ==
    if (ROBUST==0) // non-robust implementation 
    {
    double d = (Dest->x - Origin->x)*(R->y - Origin->y) - (Dest->y - Origin->y)*(R->x - Origin->x);

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
    else 
    {
     //robust implementation 
     
    double A[2];
    double B[2];
    double C[2];
    
    A[0]=Origin->x;
    A[1]=Origin->y;
    
    B[0]=Dest->x;
    B[1]=Dest->y;
    
    C[0]=R->x;
    C[1]=R->y;
    
    double robust = orient2d(A,B,C);
    if (robust>0)
    {
        return -1;  //left
    }
    else if (robust<0)
    {
        return 1;   //right
    }
    else
    {
        return 0;  //on the segment
    }
    }
}





/* ========================================  STACK FUNCTIONS ================================================ */ 

/* Crerate a leaf in the stack */
StackLeaf *StackLeafCreate(ElementLoc *Element)
{
    StackLeaf *StackElem =  malloc(sizeof(StackLeaf));
    StackElem->Elem = Element;
    StackElem->next = NULL;
    StackElem->previous = NULL;
    return StackElem;
}


/* Crerate a new stack */
TheStack *TheStackCreate()
{
    TheStack *myStack = malloc(sizeof(TheStack));
    myStack->first=NULL;
    myStack->size = 0;
    return myStack;
}


/*
Function which delete the StackElem ElemToDelete from the pile
*/
void DeleteStackElement(StackLeaf *ElemToDelete, TheStack *S)
{
    //"normal" case
    if (ElemToDelete->previous != NULL && ElemToDelete->next != NULL)
    {
        ElemToDelete->previous->next = ElemToDelete->next;
        ElemToDelete->next->previous = ElemToDelete->previous;
    }
    //1e element of the stack
    else if (ElemToDelete->previous == NULL && ElemToDelete->next != NULL)
    {
        ElemToDelete->next->previous = NULL;
        S->first = ElemToDelete->next;
    }
    //last element of the stack
    else if (ElemToDelete->previous != NULL && ElemToDelete->next == NULL)
    {
        ElemToDelete->previous->next = NULL;
    }
    //empty stack
    else
    {
        S->first = NULL;
    }
    S->size--;
}


/*
 Function which add the StackElem ElemToAdd to the pile
 */
void AddStackElement(StackLeaf *ElemToAdd,TheStack *S)
{
    ElemToAdd->next = S->first;
    if (S->first != NULL) {
        S->first->previous = ElemToAdd;
    }
    S->first = ElemToAdd;
    S->size++;
}





/* ======================================== FILE FUNCTIONS ================================================ */

/*
	writeFile2 writes the Final structure to the file Triangles.csv
*/
void writeFile2(TheStack *S,char name[256])
{
    FILE *finalF;
    finalF = fopen(name,"w");
    
    StackLeaf *currentElement = S->first;
    int p=0;

    fprintf(finalF,"%d \n",S->size);
    int counter=0;
    while (currentElement->next != NULL) {
        if (isBigTriangle(currentElement->Elem->T)==1)
        {
            counter++;
        
            fprintf(finalF,"%d %d %d \n", currentElement->Elem->T->E->origine->num,currentElement->Elem->T->E->next->origine->num,currentElement->Elem->T->E->next->next->origine->num);
        p=p+1;
        }
        currentElement = currentElement->next;
    }
    fclose(finalF);
    FILE *finalF2;
    finalF2 = fopen(name,"r+");
    fprintf(finalF2,"%d \n",counter);
    fclose(finalF2);
    
}


/*
	EvolutionWriteFile writes the evolution of leafs in the stack structure
*/
void EvolutionWriteFile(TheStack *S, int iter)
{
	const char *basename = "%s-%08d.txt";
    const char *baseResultName = "data/Evolution";
    char filename[256];        
    sprintf(filename,basename,baseResultName,iter);
    FILE* evolution = fopen(filename,"w");
    writeFile2(S,filename);
	fclose(evolution);
}

void EvolutionReadFile(FILE *inp, int *A, int *B, int *C, int iter)
{
	
    int j =0;
    while (!feof(inp)) {
  		if (fscanf(inp," %d %d %d\n", &A[j], &B[j], &C[j]) != 3)
    		break;
    	
  		j++;
	}
    
	fclose(inp);
}


/*
 return 1 if it is not a big triangle (not to throw)
 return 0 if it is a big triangle (to throw)
*/
int isBigTriangle(meshTriangle *T)
{
    if (T->E->origine != thePoint[0] && T->E->next->origine != thePoint[0] && T->E->next->next->origine != thePoint[0])
    {
        if (T->E->origine != thePoint[2] && T->E->next->origine != thePoint[2] && T->E->next->next->origine != thePoint[2])
        {
            if (T->E->origine != thePoint[1] && T->E->next->origine != thePoint[1] && T->E->next->next->origine != thePoint[1])
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
