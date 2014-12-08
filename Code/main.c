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

/*TODO 
-Meilleur interface graphique
-Robust pour les quad par exemple
-Speed
*/
int main(int argc, char *argv[])
{	

/* ====== Points loading ======== */
	FILE *fp;
	fp = fopen("data3.txt", "r");
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
	fclose(fp);

/* ====== Delaunay Computation ======== */	
	int lengthTot = length+3;
   	meshPoint **thePoint = initialiseThePoint(X,Y,length);
  	DelaunayTriangulation(thePoint, lengthTot);
 
       
/* ======= Graphics ======= */
   	FILE *fT;
	fT = fopen("Triangles.csv", "r");
	//fT = fopen("Evolution-00000007.txt", "r");
	//TODO marche de securite ou savoir le nombre de triangles
	int lengthTriangle = 0;
	fscanf(fT," %d \n", &lengthTriangle);
	int A[lengthTriangle];
	int B[lengthTriangle];	
	int C[lengthTriangle];
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
	printf("j: %d\n",j);
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
		int B[lengthTriangle];	
		int C[lengthTriangle];
            
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
	int A[lengthTriangle];
	int B[lengthTriangle];	
	int C[lengthTriangle];
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
	printf("j: %d\n",j);
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

