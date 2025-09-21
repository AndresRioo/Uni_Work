#include "interpolacio.h"

int main(void){

int i,j, n, iteracions ,numextrap;
double a,b, precisio, h, **T,   valor1, valorTrap, valorTrapAnterior;
FILE *fileEntrada;

fileEntrada = fopen ( "inputsMain3.txt" , "r" );

if ( fileEntrada == NULL ) {
	printf( "\n fitxer no existent \n");
	printf("\n FIN DEL PROGRAMA \n ");
	return -1;
}

fscanf(fileEntrada,"%lf",&a);
fscanf(fileEntrada,"%lf",&b);
fscanf(fileEntrada,"%lf",&precisio);
fscanf(fileEntrada,"%d",&numextrap);

fclose(fileEntrada);

T = (double **) malloc (numextrap * sizeof (double));

if ( T == NULL ) {

	printf( "No hi ha suficient memoria" );
	return -1;
}

for (i = 0; i<numextrap ; i++) {

	T[i] = (double *) malloc ( (i+1) * sizeof (double));
	
	if ( T[i] == NULL ){
		
		printf( "No hi ha suficient memoria" );
		return -1;
}}


printf( "Valor a: %16.7e\n", a);
printf( "Valor b: %16.7e\n", b);
printf( "Valor precisio: %16.7e\n", precisio);
printf( "Valor numextrap: %d\n", numextrap);
printf("\n");

n = 1;
iteracions = 0;
i = 0;
j = 1;

h = b - a;

valor1 = ( 1.0/2.0 * ( f(a) + f(b) ) ) * h;

valorTrapAnterior = 0;
valorTrap = 0;


do {

	iteracions++;
	
	/*
	Regla dels trapezis
	*/

	valorTrapAnterior = valorTrap / 2;	

	valorTrap = trap(n,a,b) + valorTrapAnterior;	
	
	T[i][0] = valorTrap + valor1;
	
	valor1 = valor1 / 2;
	
	/*
	Romberg
	*/

	for (j = 1; j < iteracions ; j++) {


		T[i][j] = ( pow(4, j) * T[i][j-1] - T[i-1][j-1]) / ( pow(4, j) - 1 );
	
	}

	printf( "Iteracion [%d] Valor Integral  :  %16.15e\n", i, T[i][j-1] );

	i ++;
	n = n*2;

} while ( numextrap > iteracions && fabs(T[i-1][j-1] - T[i-1][j-2]) > precisio ) ;


	
printf("\n FIN DEL BUCLE. \n");
printf( "Valor integral : %16.20e\n", T[i-1][j-1]);
printf( "Iteracions fetes : %d\n", iteracions);


for (i = 0; i < numextrap; i++) {

	free(T[i]);

}

free(T);


printf("\n FIN DEL PROGRAMA. \n ");
return 1;

}
