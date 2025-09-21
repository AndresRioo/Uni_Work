
#include "linalg.h"

int main(void){

int n, i , j , k, *p, signe;
double **a, **copiaA;
double tol ,producteLU;


FILE* entrada;
entrada = fopen ( "inputsPALU.dat" , "r" );

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
p = (int *) malloc (n * sizeof (int));


if ( a == NULL || p == NULL){

	printf( "No hi ha suficient memoria" );
	return -1;
}

for (k = 0; k<n ; k++) {

	a[k] = (double *) malloc (n * sizeof (double));
	copiaA[k] = (double *) malloc (n * sizeof (double));
	
	if ( a[k] == NULL || copiaA[k] == NULL ){
		
		printf( "No hi ha suficient memoria" );
		return -1;
}}


for( i = 0; i<n ; i++ ) {
for( j = 0; j<n ; j++ ) {

if ( fscanf(entrada, "%le", &a[i][j]) != 1 ){

	printf("Error a la lectura de A ");
	return -1;
}}}

for( i = 0; i<n ; i++ ) {
for( j = 0; j<n ; j++ ) {

copiaA[i][j] = a[i][j];

}}


fclose(entrada);

/* 
HASTA AQUI LA LECTURA DE DATOS
*/

printf( "Matriu a \n ");


for( i = 0; i<n ; i++ ) {
for( j = 0; j<n ; j++ ) {

	printf("%16.7e", a[i][j] );

}

printf ("\n");

}


/*
Inicializar p con 0,1,2,3,4...
*/
for( i = 0; i<n ; i++ ) {

	p[i] = i ;

}

printf( "Vector p \n ");

for( j = 0; j<n ; j++ ) {

	printf ( " %d" , p[j] );

}

printf ("\n\n");

printf("EXECUTEM LA FUNCIO PALU \n\n");

signe = palu(n,a,p,tol);

if (signe == 0) {

	printf ("Error al PALU \n");
	return -1; 

}


printf( " matriu PA \n ");

for( i = 0; i<n ; i++ ) {
for( j = 0; j<n ; j++ ) {

	printf("%16.7e", copiaA[ p[i] ][j] );

}

printf ("\n");

}

printf( " matriu LU \n ");

for( i = 0; i<n ; i++ ) {
for( j = 0; j<n ; j++ ) {

	printf("%16.7e", a[i][j] );

}

printf ("\n");

}



printf( " vector p \n ");

for( j = 0; j<n ; j++ ) {

	printf ( " %d" , p[j] );

}



printf ("\n paritat permutacio %d\n", signe);



printf("Producte  L * U  \n");


for (i = 0; i < n ; i++){
	for (j = 0; j < n ; j++){

		producteLU = 0;
		
		for ( k = 0; k < i+1 && k < j+1 ; k++) {
		
		
			if ( i == k ) {			
			
			producteLU += a[k][j];

			} else {

			producteLU += a[i][k] * a[k][j];

			}
		
		
			}
			printf(" %16.7e", producteLU );
			}
			printf("\n");
}

for (i = 0; i<n ; i++) {

	free(a[i]);
	free(copiaA[i]);

}

free(copiaA);
free(a);
free(p);

printf("fin del programa \n" );

return 1;
}