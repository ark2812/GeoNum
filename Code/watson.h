#include <stdio.h>

//Declaration of structure
typedef struct {
    double x;
    double y;
    int num;
} meshPoint;

//sens anti-horlogique
typedef struct {
    struct  meshEdge *E;
    struct ElementLoc *Elem;
} meshTriangle;

typedef struct meshEdge{
     meshTriangle *T;
    struct meshEdge *twin;
    struct meshEdge *next;
    meshPoint *origine;
}meshEdge;

typedef struct  ElementLoc{
    meshTriangle *T;
    struct ElementLoc *next1;
    struct ElementLoc *next3;
    struct ElementLoc *next2;
    struct StackLeaf *SLeaf;
}ElementLoc;

// structure de donnée, arbre de recherche
typedef struct {
    ElementLoc *first;
} LocationTree;


// structure de donnée pour stocker les leafs
typedef struct StackLeaf{
    struct StackLeaf *next;
    struct StackLeaf *previous;
    ElementLoc *Elem;
} StackLeaf;

typedef struct {
    StackLeaf *first;
} TheStack;



//Declaration of functions
meshPoint *meshPointCreate(double x, double y, int numR);
meshTriangle *meshTriangleCreate(meshEdge *ER, ElementLoc *E);
meshEdge *meshEdgeCreate(meshTriangle *TR, meshEdge *twinR, meshEdge *nextR, meshPoint *origineR);
int isInsideGen( meshPoint *thePoint1,  meshPoint *thePoint2,
                meshPoint *thePoint3,  meshPoint *thePointR);
ElementLoc *ElementLocCreate();
int leftRightSegment(meshPoint *A, meshPoint *B, meshPoint *R);
int InOutTriangle(meshPoint *P,ElementLoc *currentElement);
ElementLoc *LocatePoint(ElementLoc *currentElement,meshPoint *P, int *status);

void addTreeToLeafEdge(ElementLoc *leaf,meshPoint *P,meshEdge *E,TheStack *S);
meshPoint **initialiseThePoint(double *X, double *Y, int length);
int testFunctions(int length);
void randomSwitch();
void DelaunayTriangulation(meshPoint **P, int length);
void LegalizeEdge(meshPoint *R, meshEdge *E,ElementLoc *currentElement, TheStack *S);
void writeFile(ElementLoc *Element, FILE *finalF, int count);
int isBigTriangle(meshTriangle *T);
void EvolutionWriteFile(TheStack *S, int iter);
void EvolutionReadFile(int *A, int *B, int *C, int iter);
StackLeaf *StackLeafCreate(ElementLoc *Element);
TheStack *TheStackCreate();
void DeleteStackElement(StackLeaf *ElemToDelete, TheStack *S);
void AddStackElement(StackLeaf *ElemToAdd,TheStack *S);
void writeFile2(TheStack *S,FILE *test);
