//Declaration of structure
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
meshPoint *meshPointCreate(double x, double y);
int isInsideGen( meshPoint *thePoint1,  meshPoint *thePoint2,
                meshPoint *thePoint3,  meshPoint *thePointR);
void LegalizeEdge(meshPoint *R, meshPoint *I, meshPoint *J, meshPoint *K);
ElementLoc *ElementLocCreate();
int leftRightSegment(meshPoint *A, meshPoint *B, meshPoint *R);
int InOutTriangle(meshPoint *P,meshTriangle *T);
ElementLoc *LocatePoint(ElementLoc *currentElement,meshPoint *P, int status);
void addTreeToLeaf(ElementLoc *leaf,meshPoint *P);
