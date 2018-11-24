#include <stdio.h>
#include <stdlib.h>

double metodo1 (double, int);
double metodo2 (double, int);

int Count1, Count2, i, j;

int main (void)
{ 

	double x = 0.5; 

	for(i = 0 ; i < 17; i++){ 
		Count1 = 0;
		double res = metodo1(x ,i); 
		printf("Resultado de %1.2f^%d = %1f \n", x, i, res); 
		printf("Numero de multiplicacoes: %d \n", Count1); 
		printf("\n");
	} 

	printf("\n\n");

	for(j = 0 ; j < 17; j++){ 
		Count2 = 0;		
		double res = metodo2(x ,j); 
		printf("Resultado de %1.2f^%d = %1f \n", x, j, res); 
		printf("Numero de multiplicacoes: %d \n", Count2); 
		printf("\n");
	} 

} 

double metodo1(double x, int n)
{ 

	if(n == 0)
		return 1.0;
	
	Count1++; 

	return x * metodo1(x, n-1); 

} 

double metodo2(double x, int n)
{ 

	if(n == 0)
		return 1.0;

	if (n == 1)
		return x;

	if(n % 2 == 0){
		Count2 += 2;
		return metodo2(x * x, n/2);
	}

	else{ 
		Count2 += 3;
		return x * metodo2(x * x, n/2);
	} 

}
