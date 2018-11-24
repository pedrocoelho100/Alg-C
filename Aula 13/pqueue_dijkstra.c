/******* Implementa��o da Fila com Prioridade com um Amontoado Bin�rio ********/
/***** Fila com Prioridade orientada aos m�nimos Nome: pqueue_dijkstra.c  *****/

#include <stdio.h>
#include <stdlib.h>

#include "pqueue_dijkstra.h"  /* Interface */

/* Pedro Miguel Andr� Coelho N 68803 */

/******* Defini��o do Estrutura de Dados da Fila com Prioridade ********/

struct pqueue  /* defini��o da Fila com Prioridade */
{
  unsigned int HeapSize;  /* capacidade de armazenamento da Fila - capacity of the priority queue */
  unsigned int NumElem;  /* n�mero de elementos armazenados na Fila - number of elements stored in the priority queue */
  VERTEX *Heap;  /* ponteiro para o monte a alocar dinamicamente - pointer to the priority queue array */
};

/********************** Defini��o dos Subprogramas *********************/

PtPQueue PQueueCreate (unsigned int pdim)
{

	PtPQueue PQueue;

  	if (pdim == 0) return NULL;

 	/* alocar mem�ria para a Fila com Prioridade */
  	if ((PQueue = (PtPQueue) malloc (sizeof (struct pqueue))) == NULL) return NULL;

	if ((PQueue->Heap = (VERTEX *) calloc (pdim, sizeof (struct dijkstra))) == NULL){ 
		free (PQueue);
		return NULL;
	}

  	PQueue->HeapSize = pdim;  /* armazenar a dimens�o da Fila */
  	PQueue->NumElem = 0;  /* inicializar o n�mero de elementos da Fila */

  	return PQueue;  /* devolver a refer�ncia da Fila acabada de criar */

}

int PQueueDestroy (PtPQueue *ppqueue)
{

	PtPQueue TmpPQueue = *ppqueue;

 	if (TmpPQueue == NULL) return NO_PQUEUE;

  	free (TmpPQueue->Heap);  /* libertar a mem�ria ocupada pelo monte */
  	free (TmpPQueue);  /* libertar a mem�ria ocupada pela Fila */

  	*ppqueue = NULL;  /* colocar a refer�ncia da Fila a NULL */

  	return OK;

}

int PQueueInsert (PtPQueue ppqueue, VERTEX *pelem)
{

	unsigned int i;

  	if (ppqueue == NULL) return NO_PQUEUE;
	if (ppqueue->NumElem == ppqueue->HeapSize) return PQUEUE_FULL;
	if (pelem == NULL) return NULL_PTR;

  	/* ajustar o monte, descendo os pais menores do que o elemento */
  	for (i = ppqueue->NumElem; i > 0 && ppqueue->Heap[(i-1)/2].Cost > pelem->Cost; i = (i-1)/2)
		/* descer o pai menor do que o novo elemento */    		
		ppqueue->Heap[i] = ppqueue->Heap[(i-1)/2];
		
  	ppqueue->Heap[i] = *pelem;  /* colocar o novo elemento no monte */
  	ppqueue->NumElem++;  /* incrementar o n�mero de elementos do monte */

  	return OK;

}

int PQueueDeleteMin (PtPQueue ppqueue, VERTEX *pelem)
{

	unsigned int i, filho;  /* posi��o do pai e do filho */

  	if (ppqueue == NULL) return NO_PQUEUE;  	
	if (ppqueue->NumElem == 0) return PQUEUE_EMPTY;
  	if (pelem == NULL) return NULL_PTR;

  	*pelem = ppqueue->Heap[0];  /* retirar a ra�z do amontoado */
  	ppqueue->NumElem--;  /* decrementar o n�mero de elementos do amontoado */

  	/* ajustar o amontoado, subindo os filhos maiores do que o elemento */
  	for (i = 0; i*2+1 <= ppqueue->NumElem; i = filho){

		filho = 2*i+1;  /* primeiro filho */

    		/* determinar o maior dos filhos */
    		if (filho < ppqueue->NumElem && ppqueue->Heap[filho].Cost > ppqueue->Heap[filho+1].Cost)
      			filho++;

    		/* subir o filho maior do que o �ltimo elemento */
    		if (ppqueue->Heap[filho].Cost < ppqueue->Heap[ppqueue->NumElem].Cost)
         		ppqueue->Heap[i] = ppqueue->Heap[filho];
    		
		else 
			break;
  	}

  	/* recolocar o �ltimo elemento no amontoado */
  	ppqueue->Heap[i] = ppqueue->Heap[ppqueue->NumElem];

  	return OK;

}

int PQueueDecrease (PtPQueue ppqueue, VERTEX *pelem)
{

	unsigned int i;  /* posi��o do pai e do filho */

  	if (ppqueue == NULL) return NO_PQUEUE;
  	if (ppqueue->NumElem == 0) return PQUEUE_EMPTY;
	if (pelem == NULL) return NULL_PTR;

  	/* procurar o elemento na fila com prioridade */  
  	for (i = 0; i < ppqueue->NumElem; i++)
		if (ppqueue->Heap[i].Vertex == pelem->Vertex) break;

  	if (i == ppqueue->NumElem) return NO_ELEM;

  	/* ajustar o amontoado, subindo os filhos maiores do que o elemento */
  	for ( ; i > 0 && ppqueue->Heap[(i-1)/2].Cost > pelem->Cost; i = (i-1)/2)
		ppqueue->Heap[i] = ppqueue->Heap[(i-1)/2];

  	ppqueue->Heap[i] = *pelem;	/* colocar o novo valor do elemento */

	return OK;

}
 
int PQueueIsEmpty (PtPQueue ppqueue)
{
  if (ppqueue == NULL) return NO_PQUEUE;
  if (ppqueue->NumElem == 0) return PQUEUE_EMPTY;
  return OK;  
}

int PQueueDisplay (PtPQueue ppqueue)
{
  int I;

  if (ppqueue == NULL) return NO_PQUEUE;
  if (ppqueue->NumElem == 0) return PQUEUE_EMPTY;

  for (I = 0; I < ppqueue->NumElem; I++)
    printf ("(%d-%d)  ", ppqueue->Heap[I].Vertex, ppqueue->Heap[I].Cost);
  printf ("\n");

  return OK;
}

