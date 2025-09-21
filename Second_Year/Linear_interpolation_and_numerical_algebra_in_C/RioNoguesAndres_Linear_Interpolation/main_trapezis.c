#include "interpolacio.h"

int main(void){

int n, iteracions ,maxit;
double a,b,precisio, h,  valor1, valorTotal, valorTotalAnterior, valorTrap, valorTrapAnterior;
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
fscanf(fileEntrada,"%d",&maxit);

fclose(fileEntrada);

printf( "Valor a: %16.7e\n", a);
printf( "Valor b: %16.7e\n", b);
printf( "Valor precisio: %16.7e\n", precisio);
printf( "Valor maxit: %d\n", maxit);
printf("\n");

/*
caso de n=1
*/
h = b - a;

valor1 = ( 1.0/2.0 * ( f(a) + f(b) ) ) * h;

n = 1;
iteracions = 0;
valorTotal = 1; 
valorTotalAnterior = 0;
valorTrapAnterior = 0;
valorTrap = 0;

printf("\n INICIO DEL BUCLE. \n");

while (iteracions < maxit && fabs(valorTotal - valorTotalAnterior) > precisio){

	valorTotalAnterior = valorTotal;

	iteracions++;	

	valorTrapAnterior = valorTrap / 2;	
	
	valorTrap = trap(n,a,b) + valorTrapAnterior;	
	
	valorTotal = valorTrap + valor1;
	
	printf( "Iterat [%d] Valor integral : %16.15e\n", iteracions , valorTotal);
	
	n = n * 2;
	valor1 = valor1 / 2;
		
}
	
printf("\n FIN DEL BUCLE. \n");
printf( "Valor integral : %16.20e\n", valorTotal);
printf( "Iteracions fetes : %d\n", iteracions);


printf("\n FIN DEL PROGRAMA. \n ");
return 1;
}
