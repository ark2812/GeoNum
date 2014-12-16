#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <GL/glfw.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include "glfem.h"
#include "glfem.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


meshPoint **loadPoints(int LIMIT);
void makeDataDir();
void computeDelaunay(meshPoint **thePoint, int length);
void graphicEvolution(meshPoint **thePoint, int length);
int length = 0;
 
int main(int argc, char *argv[])
{	
	if(argc < 3)
	{
	 fprintf(stderr, "myDelaunay requires 3 arguments, you only gave %d,\ncf. main.c or report for explanation\n", argc-1);
	 return EXIT_FAILURE;
	}
/* ==================== 3 arg au main : LIMIT ROBUST EVOL ==================
	====== Cas limite (carrés) (LIMIT = 1) ou random (LIMIT = 0) 	======= 
	====== Implementation robuste (ROBUST = 1) ou pas (ROBUST = 0) 	======= 
	====== Graphe d'évolution (EVOL = 1) ou pas (EVOL = 0) 			======= */
	LIMIT = atoi(argv[1]);
    ROBUST = atoi(argv[2]);
    EVOL =  atoi(argv[3]);
 
/* ====== Points loading ======== */ 
	meshPoint **thePoint = loadPoints(LIMIT);
	
/* ====== Create a data directory ======== */
	if(EVOL==1){
		makeDataDir();
	}

/* ====== Delaunay Computation ======== */
	computeDelaunay(thePoint, length);   

/* ======= Graphics ======= */ 
	if(EVOL == 1){
	graphicEvolution(thePoint,length); 
 	}
 
return  0;
}


/* LOADPOINTS loads the points from the magicdata binary files and the number of points from txt file, create and return a meshPoint structure */
meshPoint **loadPoints(int LIMIT){

FILE *fp; FILE *fx; FILE *fy;
	if(LIMIT ==1){
	fp = fopen("datasLimit.txt", "r");
	fx = fopen("magicdataLimitX.bin", "rb");
	fy = fopen("magicdataLimitY.bin", "rb");}
	else{
	fp = fopen("datas.txt", "r");
	fx = fopen("magicdataX.bin", "rb");
	fy = fopen("magicdataY.bin", "rb");
	}
	char nPoints[256]; 
	fscanf(fp,"%s %d",nPoints, &length);
	double *X = malloc(sizeof(double)*length);
	double *Y = malloc(sizeof(double)*length);
	int i = 0;

	for(i=0;i<length;i++) {
    	fread(&X[i], sizeof(double),1, fx);    	
    	fread(&Y[i], sizeof(double),1, fy);
    	//printf("X:%f\n",X[i]);
	}
	fclose(fx);	
	fclose(fy);
	fclose(fp);
	
	meshPoint **thePoint = initialiseThePoint(X,Y,length);
	
	return thePoint;	
}	


/*MAKEDATADIR creates a directory data if it does not exist */
void makeDataDir(){
struct stat st = {0};
if (stat("data", &st) == -1) {
	    mkdir("data", 0700);
	}
}

/*COMPUTEDELAUNAY launches the Delaunay Triangulation and print the time required for this computation */
void computeDelaunay(meshPoint **thePoint, int length){

	int lengthTot = length+3;
	struct timespec start, finish;
	double elapsed;	
	clock_gettime(CLOCK_MONOTONIC, &start);
	DelaunayTriangulation(thePoint, lengthTot, EVOL);
	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("%f\n",elapsed);
}

/*GRAPHICEVOLUTION loads the evolution files in the data directory, launches a opengl window and  draws the evolution of DelaunayTriangulation */
void graphicEvolution(meshPoint **thePoint, int length){

 	int lengthTot = length+3;
   	FILE *fT;
	fT = fopen("Triangles.csv", "r");
	int lengthTriangle = 0;
	fscanf(fT," %d \n", &lengthTriangle);
	int *A = malloc(sizeof(double)*lengthTriangle);
	int *B = malloc(sizeof(double)*lengthTriangle);
	int *C = malloc(sizeof(double)*lengthTriangle);
	int j=0;	
	while (!feof(fT)) {
	if(fscanf(fT," %d %d %d\n", &A[j], &B[j], &C[j]) != 3)
  		break;
  		j++;
	}
	fclose(fT);
	int frame = 0;

    glfemInit("MECA2170 : Delaunay Triangulation");
    double t = 0;
    do
    {
        int w,h;
        t = glfwGetTime();
    	frame = (int) (t * 2);
    	glfwEnable( GLFW_STICKY_KEYS );
        glfwSwapInterval( 1 );
        glfwGetWindowSize(&w,&h);
        glfemReshapeWindows(thePoint,w,h, lengthTot);
		glfemDrawNodes(thePoint, lengthTot);
    	     
   			char filename[256];
            char basename[256];            
    		char *baseResultName = "data/Evolution" ; 
            sprintf(basename, baseResultName);
            strcat(basename, "-%08d.txt");
            sprintf(filename, basename, frame);
            FILE *inp;
            inp = fopen(filename, "r");
            if(inp != NULL)
            {
            	if(!feof(inp) ){
            		fscanf(inp," %d \n", &lengthTriangle);
					int *A = malloc(lengthTriangle*sizeof(int*));
					int *B = malloc(lengthTriangle*sizeof(int*));	
					int *C = malloc(lengthTriangle*sizeof(int*));
            
              	EvolutionReadFile(inp,A,B,C,frame);
              	printf("name:%s\n",filename);
              	glColor3f(0.0f, 0.0f, 1.0f);
    			glEnable(GL_POINT_SMOOTH);
    			glPointSize(7.0);
    			glBegin(GL_POINTS);    
        		glVertex2f(thePoint[frame]->x,thePoint[frame]->y); 
    			glEnd();
			}
			}
			else
			{
				FILE *fT;
				fT = fopen("Triangles.csv", "r");
				lengthTriangle = 0;
				fscanf(fT," %d \n", &lengthTriangle);
				int *A = malloc(lengthTriangle*sizeof(int*));
				int *B = malloc(lengthTriangle*sizeof(int*));	
				int *C = malloc(lengthTriangle*sizeof(int*));
				int j=0;	
				while (!feof(fT)) {
					if(fscanf(fT," %d %d %d\n", &A[j], &B[j], &C[j]) != 3)
  						break;
  						j++;
				}
				fclose(fT);
			}
		
        glfemPlotMesh(thePoint,A,B,C,lengthTriangle);              
        	glfwSwapBuffers();
        
    } 
    while( glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
           glfwGetWindowParam(GLFW_OPENED) );    
    // Check if the ESC key was pressed or the window was closed
               
    glfwTerminate();  
    exit(EXIT_SUCCESS);
}
