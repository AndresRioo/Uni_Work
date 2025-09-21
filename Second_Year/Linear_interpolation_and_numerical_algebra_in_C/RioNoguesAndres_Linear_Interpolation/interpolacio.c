#include  "interpolacio.h" 



double hornerNewton(double z, double *x, double *c, int n ) {

int i;
double sum ;

sum = c[n-1];

for (i = n-2; i >= 0; i --){
sum = sum * (z-x[i]) + c[i];
}
return sum ;
}



int difdiv(double *x, double *f, int n) {

int k,i;
double valorAbs;

for (k = 1; k <= n - 1; k++){

	for (i = n - 1; i >= k ; i-- ){

		valorAbs = fabs(x[i] - x[i-k]);

		if (valorAbs < 1e-12 ){

			return -1;

		}

		f[i] = ( f[i] - f[i-1] ) / ( x[i] - x[i-k] );


}}

return 0;

}


int  difdivherm(double *x, double *f, int m)  {

int k,i;
double valorAbs, temporal1, temporal2;

temporal2 = f[0];

for (i = 2; i < m; i++){

if ( i % 2 == 0 ){
	
	valorAbs = fabs(x[i] - x[i-2]);

	if (valorAbs < 1e-12 ){

		return -1;

	}
		
	temporal1 = f[i];
	f[i] = ( f[i] - temporal2 ) / ( x[i] - x[i-2] );
	temporal2 = temporal1;
}} 

for (k = 2; k <= m ; k++){

	for (i = m ; i >= k ; i-- ){

		valorAbs = fabs(x[i] - x[i-k]);

		if (valorAbs < 1e-12 ){
			return -1;
		}

		f[i] = ( f[i] - f[i-1] ) / ( x[i] - x[i-k] );

}}
return 0;
}

void horner2(double z, double *x, double *c, double *pdz, int n) {

int i;
double p, dp;

dp = 0;
p = c[n-1];

for (i = n-2; i >= 0; i --){

dp = p   + (z-x[i])*dp;
p = c[i] + (z-x[i])*p;

}

pdz[0] = p;
pdz[1] = dp;

}


double trap(int n, double a, double b){

int i;
double t, xi, h;

h = ( b - a ) / n;
t = 0;

for ( i=1 ; i <= n - 1; i += 2) {

	xi = a + i*h;
	t += f(xi);	

}

return t * h;

}

double f(double x) {

/*Funcio per a la integral*/

return exp(x);


}




