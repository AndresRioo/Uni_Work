#include "linalg.h"


int main(void){

int n, i , j , k;
double **a, *b, *x, *r, *y;
double tol;
double prodEsc = 0;

FILE* entrada;
entrada = fopen ( "inputsLU.dat" , "r" );

if ( entrada == NULL ) {
	printf( "fitxer no existent");
	return -1;
}

printf("INPUTS \n");
fscanf(entrada,"%d",&n);
fscanf(entrada,"%lf",&tol);

printf("n : %d\n", n );
printf("tol : %16.7e\n", tol );

a = (double **) malloc (n * sizeof (double));
b = (double *) malloc (n * sizeof (double));
x = (double *) malloc (n * sizeof (double));
y = (double *) malloc (n * sizeof (double));
r = (double *) malloc (n * sizeof (double));

if ( a == NULL || b == NULL || x == NULL || r == NULL || y == NULL){

	printf( "No hi ha suficient memoria" );
	return -1;
}

for (k = 0; k<n ; k++) {

	a[k] = (double *) malloc (n * sizeof (double));
	
	if ( a[k] == NULL ){
		
		printf( "No hi ha suficient memoria" );
		return -1;
}}


for( i = 0; i<n ; i++ ) {
for( j = 0; j<n ; j++ ) {

if ( fscanf(entrada, "%le", &a[i][j]) != 1 ){

	printf("Error a la lectura de A ");
	return -1;
}}}

for( j = 0; j<n ; j++ ) {

if ( fscanf(entrada, "%le", &b[j]) != 1 ){

	printf("Error a la lectura de B ");
	return -1;

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

printf ("\n");}



printf( "Vector b \n ");

for( j = 0; j<n ; j++ ) {

	printf(" %16.7e", b[j] );

}

printf ("\n");


/*
HASTA AQUI IMPRESION DE INPUTS
*/

/*
CRIDA DE FUNCIONS
*/

printf ("\n");
printf(" ********* LU ******** \n ");
printf ("\n");

if ( lu(n,a,tol) == 0 ){
	printf("Error al LU de a \n");
	return -1;
}

printf( " matriu a amb forma LU \n" );


for( i = 0; i<n ; i++ ) {
for( j = 0; j<n ; j++ ) {

	printf(" %16.7e", a[i][j] );

}

printf ("\n");}


if (resolLinf (n, a, b, y, tol, 1) == 0 ){
	printf("Error al resol L inf \n");
	return -1;
	
}
if (resolUsup (n, a, y, x, tol) == 0){
	printf("Error al resol U sup \n");
	return -1;
}
  
residu (n,a,b,x,r);
prodEsc = prod_esc(n,r,r);
prodEsc = sqrt (prodEsc);



/*
IMPRESION DE RESULTADOS
*/


printf(" \n *** POST FUNCIONS ***\n\n");



printf( "Vector x \n ");

for( j = 0; j<n ; j++ ) {

	printf(" %16.7e", x[j] );

}

printf ("\n");

printf( "Vector r \n ");

for( j = 0; j<n ; j++ ) {

	printf(" %16.7e", r[j] );

}

printf ("\n");


printf("Norma 2 : %16.7e\n", prodEsc );




for (i = 0; i<n ; i++) {

	free(a[i]);

}

free(a);
free(b);
free(x);
free(y);
free(r);

printf("fin del programa \n ");

return 1;
}