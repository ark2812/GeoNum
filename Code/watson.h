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
}ElementLoc;

// structure de donnée, arbre de recherche
typedef struct {
    ElementLoc *first;
} LocationTree;


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
void addTreeToLeaf(ElementLoc *leaf,meshPoint *P);
void addTreeToLeafEdge(ElementLoc *leaf,meshPoint *P,meshEdge *E);
void initialiseThePoint(double *X, double *Y, int length);
int testFunctions(int length);
void randomSwitch();
void DelaunayTriangulation(meshPoint **P, int length);
void LegalizeEdge(meshPoint *R, meshEdge *E,ElementLoc *currentElement);
void writeFile(ElementLoc *Element, FILE *test, FILE *evolution, int count);
int isBigTriangle(meshTriangle *T);
