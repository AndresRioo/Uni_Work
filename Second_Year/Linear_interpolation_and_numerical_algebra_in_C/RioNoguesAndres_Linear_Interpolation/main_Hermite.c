#include "interpolacio.h"

int main(void){

int n, i, a, b, c;
double *x, *f, resultadoDerivada, resultadoAproximado, h, puntoEvaluar, puntoA, puntoB, *pdz;


FILE *fileresultado, *fileEntrada;

fileresultado = fopen ( "resultadoMain2.txt" , "w" );
fileEntrada = fopen ( "inputsMain2.dat" , "r" );

if ( fileresultado == NULL || fileEntrada == NULL ) {
	printf( "\n fitxer no existent \n");
	printf("\n FIN DEL PROGRAMA \n ");
	return -1;
}

fscanf(fileEntrada,"%d",&n);

x = (double *) malloc ( (2*n+2) * sizeof (double));
f = (double *) malloc ( (2*n+2) * sizeof (double));
pdz = (double *) malloc ( (2) * sizeof (double));

if ( x == NULL || f == NULL ||  pdz == NULL ){

	printf( "No hi ha suficient memoria" );
	return -1;
}




for (i = 0; i < 2*n+2 ; i += 2){

	a = fscanf(fileEntrada,"%lf",&x[i]);
	b = fscanf(fileEntrada,"%lf",&f[i]);
	x[i+1] = x[i];
	c = fscanf(fileEntrada,"%lf",&f[i+1]);

if ( a != 1 || b != 1 || c != 1  ){

	printf("Error a la lectura ");
	return -1;

}
}


fclose(fileEntrada);

printf( "Valor N: %d\n", n);

printf( "Vector x\n");

for( i = 0; i< 2*n + 2 ; i++ ) {

	printf("%16.7e", x[i] );

}
printf("\n");
printf( "Vector f\n");

for( i = 0; i< 2*n + 2 ; i++ ) {

	printf("%16.7e", f[i] );

}
printf("\n");
/* 
HASTA AQUI LA LECTURA DE DATOS DE N, X y F
*/

i = difdivherm( x, f, (2*n + 1) );

if ( i == -1 ){

	printf("\n ERROR AL HACER DIFDIV \n ");
	printf("\n FIN DEL PROGRAMA \n ");

	return -1;

}

printf( "Vector de Ci\n");

for( i = 0; i< 2*n + 2 ; i++ ) {

	printf("%16.7e", f[i] );

}
printf("\n");


puntoA = x[0];
puntoB = x[ 2*n + 1 ];

h = (puntoB - puntoA) / (501-1);

printf( "Valor puntoA : %16.7e\n", puntoA);
printf( "Valor puntoB : %16.7e\n", puntoB);

for ( i = 0 ; i < 501 ; i++ ){

	puntoEvaluar = puntoA + (h*i);

	horner2(puntoEvaluar, x, f, pdz, 2*n + 2 );

	resultadoAproximado = pdz[0];
	resultadoDerivada = pdz[1];
	
	fprintf(fileresultado, "%16.7e %16.7e %16.7e\n", puntoEvaluar, resultadoAproximado, resultadoDerivada);
}

fclose(fileresultado);
free(x);
free(f);
free(pdz);

printf("\n RESULTADOS GUARDADOS EN \"resultadoMain2.txt\"  \n ");
printf("\n FIN DEL PROGRAMA. \n ");
return 0;
}
