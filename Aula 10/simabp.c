#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "abp.h"

int main (void)
{
	PtABPNode abp, node; int value, error, sum; unsigned int count; char op;

	system ("clear");
	abp = ABPCreate ();

  	do
	{
		printf ("Inserir-Inserting/Remover-Removing/Terminar-Terminating-> ");
		scanf ("%c", &op);
		scanf ("%*[^\n]"); scanf ("%*c");
		op = toupper (op);
	} while (op != 'I' && op != 'R' && op != 'T');

	while (op != 'T')
	{
  		printf ("Valor (Value) -> ");
		scanf ("%d", &value);
		scanf ("%*[^\n]"); scanf ("%*c");

		if (op == 'I')
		{
			ABPInsertRec (&abp, value);
			if ((error = ABPErrorCode ()) != OK) printf ("Erro a inserir (Error in inserting) -> %s\n", ABPErrorMessage ());
		}
		else	if (op == 'R')
			{
				ABPDeleteRec (&abp, value);
				if ((error = ABPErrorCode ()) != OK) printf ("Erro a remover (Error in removing) -> %s\n", ABPErrorMessage ());
			}
			else
			{
				printf ("Erro na operacao escolhida - Error on the chosen operation\n");
				error = 1;
			}

		if (!error)
		{
			printf ("--------------------------------------------------------------------------------------------\n");
			if (ABPEmpty (abp)) printf ("\nArvore vazia! - Empty tree!\n");
			else ABPDisplay (abp);
  			printf ("--------------------------------------------------------------------------------------------\n");
		}

  		printf ("Inserir-Inserting/Remover-Removing/Terminar-Terminating-> ");
		scanf ("%c", &op);
		scanf ("%*[^\n]"); scanf ("%*c");
		op = toupper (op);
	}

	printf ("--------------------------------------------------------------------------------------------\n");
	if (ABPEmpty (abp)) printf ("Arvore vazia! - Empty tree!\n");
	else 
	{
		ABPDisplay (abp);
		printf ("--------------------------------------------------------------------------------------------\n");
		printf ("Numero de nos da arvore (number of nodes) = %d\n", ABPSize (abp));
		printf ("Altura da arvore (tree height) = %d\n", ABPHeight (abp));
	}
	printf ("--------------------------------------------------------------------------------------------\n");

	node = ABPMinRep (abp);
	if (ABPErrorCode ()) printf ("Menor elemento da arvore (minimum element) = %s\n", ABPErrorMessage ());
	else printf ("Menor elemento da arvore (minimum element) = %d\n", ABPElement (node));

	node = ABPMaxRep (abp);
	if (ABPErrorCode ()) printf ("Maior elemento da arvore (maximum element) = %s\n", ABPErrorMessage ());
	else printf ("Maior elemento da arvore (maximum element) = %d\n", ABPElement (node));

	sum = ABPTotalSum (abp);
	if (ABPErrorCode ()) printf ("Soma dos elementos da arvore (sum of the elements) = %s\n", ABPErrorMessage ());
	else printf ("Soma dos elementos da arvore (sum of the elements) = %d\n", sum);

	count = ABPMultCount (abp, 7);
	if (ABPErrorCode ()) printf ("Contagem dos elementos da arvore multiplos de 7 (number of elements multiple of 7) = %s\n", ABPErrorMessage ());
	else printf ("Contagem dos elementos da arvore multiplos de 7 (number of elements multiple of 7) = %d\n", count);

	count = ABPOddCount (abp);
	if (ABPErrorCode ()) printf ("Contagem dos elementos impares da arvore (number of odd elements) = %s\n", ABPErrorMessage ());
	else printf ("Contagem dos elementos impares da arvore (number of odd elements) = %d\n", count);

	sum = ABPEvenSum (abp);
	if (ABPErrorCode ()) printf ("Soma dos elementos pares da arvore (sum of even elements) = %s\n", ABPErrorMessage ());
	else printf ("Soma dos elementos pares da arvore (sum of even elements) = %d\n", sum);

	printf ("--------------------------------------------------------------------------------------------\n");
	if (ABPErrorCode ()) printf ("Arvore listada em ordem (in-order traversal) %s\n", ABPErrorMessage ());
	else
	{
		printf ("Arvore listada em ordem (in-order traversal) ");
		ABPInOrderRec (abp);
		printf ("\n");
	}
	printf ("--------------------------------------------------------------------------------------------\n");

	sum = ABPOddOrderSum (abp);
	if (ABPErrorCode ()) printf ("Soma dos elementos com numero de ordem impar da arvore (sum of elements with odd order number) = %s\n", ABPErrorMessage ());
	else printf ("Soma dos elementos com numero de ordem impar da arvore (sum of elements with odd order number) = %d\n", sum);

	printf ("\n");

	ABPDestroy (&abp);

	return 0;
}

