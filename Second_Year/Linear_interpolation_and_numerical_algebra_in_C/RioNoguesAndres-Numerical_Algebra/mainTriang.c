#include "linalg.h"


int main(void){

int n, ind_diag, i , j , k, transposada;
double **a, *b, *x, *r;
double tol;
double prodEsc = 0;
double temp;

FILE* entrada;
entrada = fopen ( "inputsTriang.dat" , "r" );

if ( entrada == NULL ) {
	printf( "fitxer no existent");
	return -1;
}


fscanf(entrada,"%d",&n);
fscanf(entrada,"%d",&ind_diag);
fscanf(entrada,"%d",&transposada);
fscanf(entrada,"%lf",&tol);

printf("INPUTS \n");
printf("n : %d\n", n );
printf("ind_diag : %d\n", ind_diag );
printf("ind_transposada : %d\n", transposada );
printf("tolerancia : %16.7e\n", tol );
printf("\n");

a = (double **) malloc (n * sizeof (double));
b = (double *) malloc (n * sizeof (double));
x = (double *) malloc (n * sizeof (double));
r = (double *) malloc (n * sizeof (double));

if ( a == NULL || b == NULL || x == NULL || r == NULL){

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



if (transposada == 1) {

for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
        

            temp = a[i][j];
            a[i][j] = a[j][i];
            a[j][i] = temp;
        }
    }


}

/* 
HASTA AQUI LA LECTURA DE DATOS
*/

printf( "Matriu a \n ");

for( i = 0; i<n ; i++ ) {
for( j = 0; j<n ; j++ ) {

	printf(" %16.7e", a[i][j] );

}

printf ("\n");}



printf( "Vector b \n ");

for( j = 0; j<n ; j++ ) {

	printf(" %16.7e", b[j] );

}

printf ("\n");



/*
HASTA AQUI LA IMPRESION DE INPUTS 
*/

if (transposada == 1){

if ( resolUsup(n,a,b,x,tol) == 0 ){

	printf("Error al resol U sup \n");
	return -1;
}

} else {

if ( resolLinf(n,a,b,x,tol,ind_diag) == 0 ){

	printf("Error al resol L inf \n");
	return -1;
}

}

residu (n,a,b,x,r);
prodEsc = prod_esc(n,r,r);
prodEsc = sqrt (prodEsc);


/*
A PARTIR DE AQUI SOLO IMPRIME RESULTADOS
*/


printf(" \n \n *** POST FUNCIONS *** \n \n");



printf("Vector x \n ");

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
free(r);

printf("fin del programa \n ");

return 1;
}