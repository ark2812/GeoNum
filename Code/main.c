#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
//#include <GL/glfw.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include "glfem.h"
#include "glfem.h"

/*TODO 
-3 points extremes
-Meilleur interface graphique
-Robust pour les quad par exemple

*/
//int test();
int main(int argc, char *argv[])
{
	//int ans  = test();	
	FILE *fp;
	fp = fopen("datas.txt", "r");
	int length;
	char nPoints[10]; 
	fscanf(fp,"%s %d",nPoints, &length);
	double X[length];
	double Y[length];
	int i = 0;
	while (!feof(fp)) {
  		if (fscanf(fp, "%lf,%lf", &X[i], &Y[i]) != 2)
    		break;
  		i++;
	}
	//printf("X[0] : %f\n",X[0]);
	//printf("Y[0] : %f\n",Y[0]);
	fclose(fp);
   meshPoint **thePoint = initialiseThePoint(X,Y,length);
   //test();
   int ins = testFunctions(length+3);
    
   printf("coucou");
   //printf("%d\n",ins);
       
    // ======= Graphics 
   FILE *fT;
	fT = fopen("Triangles.csv", "r");
	length = 176; //352 176
	int A[length+3];
	int B[length+3];	
	int C[length+3];
	int j=0;
	int a =0;	
	while (!feof(fT)) {
  		if (fscanf(fT," %d %d %d\n", &A[j], &B[j], &C[j]) != 3)
    		break;
    	
  		//printf("A[%d] : %d\n",j,A[j]);	
  		//printf("B[%d] : %d\n",j,B[j]);
  		//printf("C[%d] : %d\n",j,C[j]);
  		j++;
	}
	
	//printf("A[0] : %d\n",A[100]);
	//printf("Y[0] : %f\n",Y[0]);
	fclose(fT);
	
   // char theMessage[256];
   // double *theField = malloc(sizeof(double)*theMesh->nNode); 
    //int i;
    //for (i=0; i < length+3; ++i)
     //   theField[i]  = -fmin(0.0,earthGridInterpolate(theGrid,theMesh->X[i],theMesh->Y[i]));          
    //sprintf(theMessage,"Sea volume = %10.3e m^3 \n",I);
    //fflush(stdout);
     char *baseResultName = "Evolution" ;  
    glfemInit("MECA2170 : Delaunay Triangulation");
    int iter = 0;
    do
    {
        int w,h;
           glfwEnable( GLFW_STICKY_KEYS );
            glfwSwapInterval( 1 );
        glfwGetWindowSize(&w,&h);
        glfemReshapeWindows(thePoint,w,h);
   		const char *basename = "%s-%08d.txt";
    	char filename[256];
    	sprintf(filename,basename,baseResultName,iter);
    	
    	FILE *file = fopen(filename,"r");
    	j=0;
    	while (!feof(file)) {
  		if (fscanf(file," %d %d %d\n", &A[j], &B[j], &C[j]) != 3)
    		break;
    	
  		//printf("A[%d] : %d\n",j,A[j]);	
  		//printf("B[%d] : %d\n",j,B[j]);
  		//printf("C[%d] : %d\n",j,C[j]);
  		j++;
	}
    	glfemPlotMesh(thePoint,A,B,C,length);
        //glfemPlotField(theMesh,theField); 
      //  glfemMessage(theMessage);              
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

