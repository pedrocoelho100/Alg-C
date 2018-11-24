#include <stdio.h>
#include <stdlib.h>

int schroderRec (int);
int schroderIt (int);

int Count1, Count2, i, j;

int main (void)
{ 

	for(i = 0 ; i < 13; i++){ 
		Count1 = 0;
		int res = schroderRec(i); 
		printf("Resultado de %d = %d \n", i, res); 
		printf("Numero de multiplicacoes: %d \n", Count1); 
		printf("\n");
	} 

	printf("\n\n");

	for(i = 0 ; i < 13; i++){ 
		Count2 = 0;
		int res = schroderIt(i); 
		printf("Resultado de %d = %d \n", i, res); 
		printf("Numero de multiplicacoes: %d \n", Count2); 
		printf("\n");
	} 

} 

int schroderRec(int n)
{ 

	int i;
	int soma = 0;

	if(n == 0)
		return 1; 

	for(i = 0; i < n; i++){
		soma += schroderRec(i) * schroderRec(n-i-1);
		Count1++;
	}

	return soma += schroderRec(n-1);

}

int schroderIt(int n)
{ 

	int i, j, res, temp;
	int *sch = (int *) calloc (sizeof(int), n+1);
	
	sch[0] = 1;

	if(n == 0)
		return sch[0];

	for(i = 1; i <= n; i++){

		temp = 0;
		for(j = 0; j < i; j++){
			temp += sch[j] * sch[i-j-1];
			Count2++;
		 }
	
		sch[i] = sch[i-1] + temp;

	}

	res = sch[n];
	free(sch);

	return res;

}
