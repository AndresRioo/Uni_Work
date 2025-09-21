#include "linalg.h"

int resolLinf (int n, double **a, double *b, double *x, double tol, int ind_diag){

double suma = 0.0;

int i,j;
double valorAbs;

for (i = 0; i<n ; i++) {

	valorAbs = fabs(a[i][i]);

	if (valorAbs < tol){

		return 0;

	}

	suma = 0.0;

	for (j = 0; j < i; j++) {

		suma += a[i][j] * x[j];

	}

	
	if (ind_diag == 1){

		x[i] = ( b[i] - suma  );

	} else {

		x[i] = ( b[i] - suma  ) / a[i][i];

	}

}

return 1;

}

int resolUsup (int n, double **a, double *b, double *x, double tol){

double suma = 0.0;

int i,j;

double valorAbs;

for (i = n-1 ; i >= 0; i--) {

	valorAbs = fabs(a[i][i]);

	if (valorAbs < tol){

		return 0;

	}

	suma = 0.0;

	for (j = i; j<n; j++) {

		suma += a[i][j] * x[j];

	}

	x[i] = ( b[i] - suma  ) / a[i][i];

	}

return 1;

}


void residu (int n, double **a, double *b, double *x, double *r){


int i;

prodMatVec ( n, n, a, x, r);

for (i = 0; i<n ; i++ ){

r[i] = b[i] - r[i];

}



}




void prodMatVec ( int n, int m, double **a, double *x, double *y) {

int i,j;

for (i = 0; i < n; i ++) {

y[i] = 0;

for (j = 0; j < m; j ++){

	y[i] += a[i][j] * x[j];

}}}


double prod_esc ( int n, double *x, double* y){

double resultado = 0.0;

int i;

for (i=0; i<n; i++) {

	resultado += x[i] * y[i];

}

return resultado;

}


int lu (int n, double **a, double tol) {


int fila, filaInferior, columna;
double multiplicador;
double valorAbs;


for (fila = 0; fila < n  ; fila ++ ){

for (filaInferior = fila + 1; filaInferior < n ; filaInferior ++) {

	valorAbs = fabs(a[fila][fila]);

	if (valorAbs < tol){

		return 0;

	}

	multiplicador = a[filaInferior][fila] / a[fila][fila];
	a[filaInferior][fila] = multiplicador;

	for (columna = fila + 1; columna < n; columna ++) {

	a[filaInferior][columna] -= a[fila][columna] * multiplicador;

}}}

return 1;

}




int palu(int n,double **a,int *p,double tol) {

int k, i, j, l , signe = -1,temp2;
double multiplicador;
double max_val, temp;


for (k = 0; k < n  ; k ++ ){


	max_val = 0.0;
        l = k;

        /* Encontrar el índice de la fila con el máximo valor en la columna k */
        for (i = k; i < n; i++) {
            if (fabs(a[i][k]) > max_val) {
                max_val = fabs(a[i][k]);
                l = i;
            }
        }

        /* Verificar si el máximo valor es menor que la tolerancia */
        if (max_val < tol) {
            return 0; /* No se puede realizar la factorización LU */
        }

        /* Intercambiar filas en la matriz y actualizar el vector de permutación p */
        if (l != k) {
            for (j = 0; j < n; j++) {
                temp = a[k][j];
                a[k][j] = a[l][j];
                a[l][j] = temp;
            }
            temp2=p[k];
            p[k] = p[l];
            p[l] = temp2;
            signe = -signe ; /* Cambiar la paridad de la permutación */
        }
        

	for (i = k + 1; i < n ; i ++) {
	
	multiplicador = a[i][k] / a[k][k];
	a[i][k] = multiplicador;

	for (j = k + 1; j < n; j ++) {

	a[i][j] -= a[k][j] * multiplicador;

}}}

return signe;

}