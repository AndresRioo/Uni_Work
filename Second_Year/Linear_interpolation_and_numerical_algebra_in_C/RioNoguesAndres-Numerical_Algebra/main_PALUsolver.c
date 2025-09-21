#include "linalg.h"



int main(void){

int n, i , j , k, *p, signe;
double **a, *x, *b, *r, *y, **copiaA, *copiaB;
double tol, prodEsc, normaInf = 0, producteLU, normaInfMax = 0;


FILE* entrada;
entrada = fopen ( "inputsPALUsolver.dat" , "r" );

if ( entrada == NULL ) {
	printf( "fitxer no existent");
	return -1;
}


fscanf(entrada,"%d",&n);
fscanf(entrada,"%lf",&tol);

printf("n : %d\n", n );
printf("tol : %16.7e\n", tol );

a = (double **) malloc (n * sizeof (double));
copiaA = (double **) malloc (n * sizeof (double));
b = (double *) malloc (n * sizeof (double));
copiaB = (double *) malloc (n * sizeof (double));
x = (double *) malloc (n * sizeof (double));
y = (double *) malloc (n * sizeof (double));
r = (double *) malloc (n * sizeof (double));
p = (int *) malloc (n * sizeof (int));


if ( a == NULL || b == NULL || copiaA == NULL ||copiaB == NULL ||x == NULL || y == NULL || r == NULL || p == NULL){

	printf( "No hi ha suficient memoria" );
	return -1;
}

for (k = 0; k<n ; k++) {

	a[k] = (double *) malloc (n * sizeof (double));
	copiaA[k] = (double *) malloc (n * sizeof (double));
	
	if ( a[k] == NULL || copiaA[k] == NULL){
		
		printf( "No hi ha suficient memoria" );
		return -1;
}}


for( i = 0; i<n ; i++ ) {
for( j = 0; j<n ; j++ ) {

if ( fscanf(entrada, "%lf", &a[i][j]) != 1 ){

	printf("Error a la lectura de A ");
	return -1;
}}}

for( j = 0; j<n ; j++ ) {

if ( fscanf(entrada, "%lf", &b[j]) != 1 ){

	printf("Error a la lectura de B ");
	return -1;
}}

fclose(entrada);



printf("Matriu a \n" );

for( i = 0; i<n ; i++ ) {
for( j = 0; j<n ; j++ ) {

	copiaA[i][j] = a[i][j];
	printf("%16.7e", a[i][j] );

}

printf ("\n");

}

printf("Vector b \n" );

for( j = 0; j<n ; j++ ) {

	copiaB[j] = b[j];
	printf("%16.7e", b[j] );

}

printf("\n");

/*
Inicializar p con 0,1,2,3,4...
*/
for( i = 0; i<n ; i++ ) {

	p[i] = i ;


}
/*
LECTURA I IMPRESIÓ DE INPUTS 
*/


signe = palu (n,a,p,tol);

if (signe == 0 ){

	printf("Error a PALU");
	return -1;

}

printf ("\n******* EXECUTEM PALU ******\n");



printf( "Matriu LU \n ");

for( i = 0; i<n ; i++ ) {
for( j = 0; j<n ; j++ ) {

	printf("%16.7e", a[i][j] );

}

printf ("\n");

}


/*
PERMUTEM B usando r como vector temporal
*/
for (i = 0; i < n; i++ ){

	r[i] = b[i];
}

for (i = 0; i < n; i++ ){

	b[i] = r[ p[i] ];
}



printf("Matriu b permutada \n" );

for( j = 0; j<n ; j++ ) {

	printf("%16.7e", b[j] );

}


printf ("\n");


if (resolLinf (n, a, b, y, tol, 1) == 0 ){
	printf("Error al resol L inf \n");
	return -1;
	
}
if (resolUsup (n, a, y, x, tol) == 0){
	printf("Error al resol U sup \n");
	return -1;
}
 
residu (n,copiaA,copiaB,x,r);
prodEsc = prod_esc(n,r,r);
prodEsc = sqrt (prodEsc);


printf("\nRESULTADOS \n");

printf( "Vector p \n ");

for( j = 0; j<n ; j++ ) {

	printf ( " %d" , p[j] );

}

printf ("\n");

printf( "Vector x \n ");

for( j = 0; j<n ; j++ ) {

	printf ( " %16.7e" , x[j] );

}

printf ("\n");

printf( "Vector r \n ");

for( j = 0; j<n ; j++ ) {

	printf ( " %16.7e" , r[j] );

}

printf ("\n");

printf( " matriu PA \n");

for( i = 0; i<n ; i++ ) {
for( j = 0; j<n ; j++ ) {

	printf("[%d][%d] = %16.7e |", i,j, copiaA[ p[i] ][j] );

}

printf ("\n");

}

printf("Matriz L * U : ");

printf("\n");

for (i = 0; i < n ; i++){

	normaInf = 0;

	for (j = 0; j < n ; j++){

		producteLU = 0;
		
		for ( k = 0; k < i+1 && k < j+1 ; k++) {
		
		
			if ( i == k ) {			
			
			producteLU += a[k][j];

			} else {

			producteLU += a[i][k] * a[k][j];

			}
			
		
			}
			
		normaInf += fabs(copiaA[ p[i] ][j] - producteLU);
		 
		
		
		printf("[%d][%d] = %16.7e |", i,j, producteLU ); 
		
		}
	
	if (normaInf > normaInfMax ) {
		
		normaInfMax = normaInf;
	
	}
	
	printf("\n");
}

printf ("\n");

printf("Norma 2 : %16.7e\n", prodEsc );

printf("\n");

printf("Norma Infinito : %16.7e\n", normaInfMax );

printf("\n");

/*
ALLIBERAR MEMORIA
*/

for (i = 0; i<n ; i++) {

	free(a[i]);
	free(copiaA[i]);

}

free(a);
free(copiaA);
free(p);
free(b);
free(copiaB);
free(x);
free(y);
free(r);

printf("fin del programa \n" );

return 1;
}