#ifndef LINALG_H
#define LINALG_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int palu(int n,double **a,int *p,double tol);
int resolLinf (int n, double **a, double *b, double *x, double tol, int ind_diag);
int resolUsup (int n, double **a, double *b, double *x, double tol);
void prodMatVec ( int n, int m, double **a, double *x, double *y);
void residu (int n, double **a, double *b, double *x, double *r);
double prod_esc ( int n, double *x, double* y);
int lu (int n, double **a, double tol) ;



#endif