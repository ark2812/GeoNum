/*
 *  glfem.h
 *  Library for MECA1120 : Finite Elements for dummies
 *
 *  Copyright (C) 2013 UCL-IMMC : Vincent Legat
 *  All rights reserved.
 *
 *  Pour GLFW (version utilisée 2.7.7)
 *  Pour l'installation de la librairie, voir http://www.glfw.org/
 *
 */

#ifndef _GLFEM_H_
#define _GLFEM_H_

#include <GL/glfw.h>
#include "watson.h"

void glfemDrawColorElement(float *x, float *y, double *u, int n);
void glfemDrawElement(float *x, float *y, int n);
void glfemDrawNodes(double* x, double* y,int n);

void glfemReshapeWindows(meshPoint **thePoint, int width, int heigh, int LengthNoBigTriangle);
//void glfemPlotField(meshPoint **thePoint, double *u);
void glfemPlotMesh(meshPoint **thePoint, int *A, int *B, int *C, int lengthTot);
void glfemMessage(char *message);

void glfemDrawMessage(int h, int v, char *message);
void glfemSetRasterSize(int width, int height);
void glfemInit(char *windowName);

#endif
