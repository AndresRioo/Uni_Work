#include "interpolacio.h"

double func(double x);

int main(void){

int n, i, Chebyshev;
double *x, *f, resultado, resultadoAproximado, error, h, puntoEvaluar, puntoA, puntoB;

FILE *fileresultado, *fileEntrada;

fileresultado = fopen ( "resultados.txt" , "w" );
fileEntrada = fopen ( "inputsMain1.dat" , "r" );

if ( fileresultado == NULL || fileEntrada == NULL ) {
	printf( "\n fitxer no existent \n");
	printf("\n FIN DEL PROGRAMA \n ");
	return -1;
}

fscanf(fileEntrada,"%d",&n);
fscanf(fileEntrada,"%d",&Chebyshev);
fscanf(fileEntrada,"%lf",&puntoA);
fscanf(fileEntrada,"%lf",&puntoB);


fclose(fileEntrada);


x = (double *) malloc ( (n + 1) * sizeof (double));
f = (double *) malloc ( (n + 1) * sizeof (double));

if ( x == NULL || f == NULL){

	printf( "No hi ha suficient memoria" );
	return -1;
}

printf( "Valor puntoA : %16.7e\n", puntoA);
printf( "Valor puntoB : %16.7e\n", puntoB);
printf( "Valor N: %d\n", n);
printf( "Con Chebyshev? 1 si 0 no  %d\n", Chebyshev);
/* 
HASTA AQUI LA LECTURA DE DATOS DE N, PUNTOA Y PUNTOB
*/

h = (puntoA - puntoB) / (n);

for (i = 0; i < n + 1 ; i++ ) {

	x[i] = puntoB + (i*h);
	
	if (Chebyshev == 1){

	x[i] = cos( ((2.0 * i + 1.0 ) * M_PI ) / (2.0 * (n + 1.0)) );
	
	}
	
	f[i] = func( x[i] );

}
/*
Creamos el polinomio caracteristico con valores arbitrarios de x dentro de [a,b]
*/

printf( "Vector x\n");

for( i = 0; i< n+1 ; i++ ) {

	printf("%16.7e", x[i] );

}
printf("\n");
printf( "Vector f(x) \n");

for( i = 0; i< n+1 ; i++ ) {

	printf("%16.7e", f[i] );

}
printf("\n");

i = difdiv(x,f,n+1);

if ( i == -1 ){

	printf("\n ERROR AL HACER DIFDIV \n ");
	printf("\n FIN DEL PROGRAMA \n ");

	return -1;

}

printf( "Vector de Ci\n");

for( i = 0; i<n+1 ; i++ ) {

	printf("%16.7e", f[i] );

}

/*
HASTA AQUI TENEMOS EL POLINOMIO INTERPOLADOR
*/


h = (puntoA - puntoB) / (1000-1);

for ( i = 0 ; i < 1000 ; i++ ){

	puntoEvaluar = puntoB + (h*i);

	resultadoAproximado = hornerNewton( puntoEvaluar ,x,f,n+1);
	resultado = func( puntoEvaluar );
	
	error = fabs( resultado - resultadoAproximado);

	fprintf(fileresultado, "%16.7e %16.7e %16.7e %16.7e\n", puntoEvaluar, resultado, resultadoAproximado, error);
}

fclose(fileresultado);
free(x);
free(f);

printf("\n FIN DEL PROGRAMA. \n ");
return 1;
}
