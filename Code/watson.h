#include <stdio.h>

int LIMIT;
int ROBUST;
int EVOL;
int RAN;

//Declaration of structure
typedef struct {
    double x;
    double y;
    int num;
} meshPoint;

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

// Seach tree
typedef struct {
    ElementLoc *first;
} LocationTree;


// To stock the leaf
typedef struct StackLeaf{
    struct StackLeaf *next;
    struct StackLeaf *previous;
    ElementLoc *Elem;
} StackLeaf;

typedef struct {
    StackLeaf *first;
    int size;
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
void addTreeToLeaf(ElementLoc *leaf,meshPoint *P,TheStack *S);
void addTreeToLeafEdge(ElementLoc *leaf,meshPoint *P,meshEdge *E,TheStack *S);
meshPoint **initialiseThePoint(double *X, double *Y, int length);
int testFunctions(int length);
void randomSwitch(double *X, double *Y, int length);
void DelaunayTriangulation(meshPoint **P, int length, int evol);
void LegalizeEdge(meshPoint *R, meshEdge *E,ElementLoc *currentElement, TheStack *S);
int isBigTriangle(meshTriangle *T);
void EvolutionWriteFile(TheStack *S, int iter);
void EvolutionReadFile(FILE *inp, int *A, int *B, int *C, int iter);
StackLeaf *StackLeafCreate(ElementLoc *Element);
TheStack *TheStackCreate();
void DeleteStackElement(StackLeaf *ElemToDelete, TheStack *S);
void AddStackElement(StackLeaf *ElemToAdd,TheStack *S);
void writeFile2(TheStack *S,char *name);
