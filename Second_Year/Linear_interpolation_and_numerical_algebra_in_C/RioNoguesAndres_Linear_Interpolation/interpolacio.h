#ifndef INTERPOLACIO_H 
#define INTERPOLACIO_H 

#define M_PI 3.14159265358979323846
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double hornerNewton(double z, double *x, double *c, int n );
int difdiv(double *x, double *f, int n);

int  difdivherm(double *x, double *f, int m);
void horner2(double z, double *x, double *c, double *pdz, int n);

double f(double x);
double trap(int n, double a, double b);


#endif
