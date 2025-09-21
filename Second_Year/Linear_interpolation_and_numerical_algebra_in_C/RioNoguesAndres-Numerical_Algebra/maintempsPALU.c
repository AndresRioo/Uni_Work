#include "linalg.h"
#include <time.h>


int main(void){

int n = 100, N = 1000, m, M,    l=1,o=-1 , i,j,k;
double prodEsc, norma2, norma2Max = 0, tol = 1e-6,total_time;
double **a,*b,*x,*r,*y, **copiaA,*copiaB;
int *p, signe;
clock_t start_time, end_time;
FILE* entrada;


entrada = fopen ( "resultatTemps.txt" , "w" );

if ( entrada == NULL ) {
	printf( "fitxer no existent");
	return -1;
}

for (m = 1 ; m < n ; m++){

	norma2Max = 0;
		
	


	start_time = clock();
	srand(time(NULL));

	for (M = 0 ; M < N ; M++){
	
		a = (double **) malloc (m * sizeof (double));
		copiaA = (double **) malloc (m * sizeof (double));
		b = (double *) malloc (m * sizeof (double));
		copiaB = (double *) malloc (m * sizeof (double));
		x = (double *) malloc (m * sizeof (double));
		y = (double *) malloc (m * sizeof (double));
		r = (double *) malloc (m * sizeof (double));
		p = (int *) malloc (m * sizeof (int));
		
		if ( a == NULL || b == NULL || copiaA == NULL ||copiaB == NULL ||x == NULL || 			y == NULL || r == NULL || p == NULL){

			printf( "No hi ha suficient memoria" );
			return -1;
		}

		for (k = 0; k<m ; k++) {

			a[k] = (double *) malloc (m * sizeof (double));
			copiaA[k] = (double *) malloc (m * sizeof (double));
			
			if ( a[k] == NULL || copiaA[k] == NULL){
				
				printf( "No hi ha suficient memoria" );
				return -1;
		}}
		
		
		for( i = 0; i<m ; i++ ) {
		for( j = 0; j<m ; j++ ) {
		
			a[i][j] = o+(l-o)*((1.*rand())/RAND_MAX);
			copiaA[i][j] = a[i][j];

			}
			b[i] = o+(l-o)*((1.*rand())/RAND_MAX);
			copiaB[i] = b[i];

		}
		
		
		for( i = 0; i<m ; i++ ) {

			p[i] = i ;


		}

		signe = palu(m,a,p,tol);
		
		if (signe == 0 ){

			printf("Error a PALU");
			return -1;

		}
			
	
		/*
		PERMUTEM B
		*/
		
		
		
		
		for (i = 0; i < m; i++ ){

			r[i] = b[i];
		}

		for (i = 0; i < m; i++ ){

			b[i] = r[ p[i] ];
		}

		if (resolLinf (m, a, b, y, tol, 1) == 0 ){
			printf("Error al resol L inf \n");
			return -1;
			
		}
		if (resolUsup (m, a, y, x, tol) == 0){
			printf("Error al resol U sup \n");
			return -1;
		}

		residu (m,copiaA,copiaB,x,r);
		prodEsc = prod_esc(m,r,r);
		norma2 = sqrt(prodEsc);
		
		if (norma2 > norma2Max ){
			norma2Max = norma2;
		}
			
		for (i = 0; i<m ; i++) {

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
		
		}

	end_time = clock();
	total_time = end_time - start_time;

	fprintf(entrada, "%d %16.7e %16.7e\n", m, norma2Max, total_time );
	
	

}
printf("fin del programa \n");
return 1;
}