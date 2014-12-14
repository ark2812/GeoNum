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


/*TODO 
-Meilleur interface graphique
-Robust pour les quad par exemple
-Speed
*/
int main(int argc, char *argv[])
{	

/* ====== Points loading ======== */
	FILE *fp; FILE *fx; FILE *fy;
	fp = fopen("data3.txt", "r");
	fx = fopen("magicdataX.bin", "rb");
	fy = fopen("magicdataY.bin", "rb");
	int length;
	char nPoints[256]; 
	fscanf(fp,"%s %d",nPoints, &length);
	//printf("len:%d\n",length);
	double *X = malloc(sizeof(double)*length);
	double *Y = malloc(sizeof(double)*length);
	double trash0;
	int i = 0;
	/*while (!feof(fp)) {
  		if (fscanf(fp, "%lf,%lf", &trash0, &trash0) != 2)
    		break;
    	fread(&X[i], sizeof(double),1, fx);    	
    	fread(&Y[i], sizeof(double),1, fy);
    	printf("len:%f\n",X[i]);
    	printf("len:%f\n",Y[i]);	
  		i++;
	}*/
	
	for(i=0;i<length;i++) {
    	fread(&X[i], sizeof(double),1, fx);    	
    	fread(&Y[i], sizeof(double),1, fy);
    	//printf("X:%f\n",X[i]);
    	//printf("Y:%f\n",Y[i]);	
	}
	fclose(fx);	
	fclose(fy);
	fclose(fp);
	//free(X); free(Y);

/* ====== Delaunay Computation ======== */

// Create a directory data if it does not exist already
int evol = 1;
if(evol==1)
{
	struct stat st = {0};

	if (stat("data", &st) == -1) {
	    mkdir("data", 0700);
	}
}
/*
	If evol == 1 print the evolution in files
	else write only the final trianges in Triangles.csv
*/

// Compute the time required by Delaunay
    
//double time_spent;
//struct timespec start, finish;
//double elapsed;	
	int lengthTot = length+3;
//	clock_gettime(CLOCK_MONOTONIC, &start);
   	meshPoint **thePoint = initialiseThePoint(X,Y,length);
  	DelaunayTriangulation(thePoint, lengthTot, evol);
//  	clock_gettime(CLOCK_MONOTONIC, &finish);
// elapsed = (finish.tv_sec - start.tv_sec);
//elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
// printf("time:%f\n",elapsed);
//  free(X);
//  free(Y);
    
/* ======= Graphics ======= */
  //int grap = 1;
  if(evol == 1)
  {
   	FILE *fT;
	fT = fopen("Triangles.csv", "r");
	//fT = fopen("Evolution-00000007.txt", "r");
	//TODO marche de securite ou savoir le nombre de triangles
	int lengthTriangle = 0;
	fscanf(fT," %d \n", &lengthTriangle);
	int *A = malloc(sizeof(double)*lengthTriangle);
	int *B = malloc(sizeof(double)*lengthTriangle);
	int *C = malloc(sizeof(double)*lengthTriangle);
	double trash;
	int j=0;
	int a =0;	
	while (!feof(fT)) {
	if(fscanf(fT," %d %d %d\n", &A[j], &B[j], &C[j]) != 3)
  		break;//if (fscanf(fT," %d %d %d\n", &trash, &trash, &trash) != 3)
    		
    	
  		//printf("A[%d] : %d\n",j,A[j]);	
  		//printf("B[%d] : %d\n",j,B[j]);
  		//printf("C[%d] : %d\n",j,C[j]);
  		j++;
	}
	//printf("Y[0] : %f\n",Y[0]);
	fclose(fT);
	int frame = 0;

    //fflush(stdout); 
    glfemInit("MECA2170 : Delaunay Triangulation");
    int iter = 0;
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
    	  
     //UNCOMMENT FROM HERE     
   		char filename[256];
            char basename[256];
            //printf("name1:%s \n",basename);
            
    		char *baseResultName = "data/Evolution" ; 
            sprintf(basename, baseResultName);
           // printf("name2:%s \n",basename);
            strcat(basename, "-%08d.txt");
           // printf("name3:%s \n",basename);
            //const char *basename = "tsunamiTriangleFineOne-%08d.txt";
            sprintf(filename, basename, frame);
            FILE *inp;
            //printf("name2:%s \n",filename);
            inp = fopen(filename, "r");
            if(inp != NULL)
            {
            if(!feof(inp) )
            {
            
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
		//TO HERE if you want to see the evolution (beta test 4.2)	
			else
			{
				FILE *fT;
	fT = fopen("Triangles.csv", "r");
	//fT = fopen("Evolution-00000007.txt", "r");
	//TODO marche de securite ou savoir le nombre de triangles
	lengthTriangle = 0;
	fscanf(fT," %d \n", &lengthTriangle);
	int *A = malloc(lengthTriangle*sizeof(int*));
	int *B = malloc(lengthTriangle*sizeof(int*));	
	int *C = malloc(lengthTriangle*sizeof(int*));
	double trash;
	int j=0;
	int a =0;	
	while (!feof(fT)) {
	if(fscanf(fT," %d %d %d\n", &A[j], &B[j], &C[j]) != 3)
  		break;//if (fscanf(fT," %d %d %d\n", &trash, &trash, &trash) != 3)
    		
    	
  		//printf("A[%d] : %d\n",j,A[j]);	
  		//printf("B[%d] : %d\n",j,B[j]);
  		//printf("C[%d] : %d\n",j,C[j]);
  		j++;
	}
	//printf("Y[0] : %f\n",Y[0]);
	fclose(fT);
			
			
			}
		
            glfemPlotMesh(thePoint,A,B,C,lengthTriangle);
        //free(A);
        //free(B);
        //free(C);              
        glfwSwapBuffers();
        
    } 
    while( glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
           glfwGetWindowParam(GLFW_OPENED) );
           
    // Check if the ESC key was pressed or the window was closed
               
    glfwTerminate(); 
   // femIntegrationFree(theRule);
   // femMeshFree(theMesh);
   // femGridFree(theGrid);   
    exit(EXIT_SUCCESS);
  }  

}

