/*******************************************************************************

 Ficheiro de implementação do Tipo de Dados Abstrato POLINOMIO (polynomial.c).
 A estrutura de dados de suporte do polinómio é uma estrutura, constituída pelo
 campo de tipo inteiro Degree para armazenar o grau do polinómio e o campo de
 tipo ponteiro Poly, para representar a sequência atribuída dinamicamente, que
 vai armazenar os seus coeficientes reais.
 
 Nome : Pedro Miguel André Coelho               NMec: 68803

 Implementation file of the abstract data type Poly (polynomial.c). The data
 structure to store the polynomial is composed with the integer field Degree for
 keeping the polynomial's degree and the pointer field Poly, that represents the
 allocated array in dynamic memory, for storing the polynomial's real coefficients.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "polynomial.h"    /* Ficheiro de interface do TDA - ADT Interface file */

/************ Definição da Estrutura de Dados Interna do POLINOMIO ************/

struct poly
{
  unsigned int Degree;  /* grau do polinómio - polynomial degree */
  double *Poly;      /* ponteiro para os coeficientes do polinómio - pointer to polynomial's coefficients */
};

/*********************** Controlo Centralizado de Erro ************************/

static unsigned int Error = OK;  /* inicialização do erro */

static char *ErrorMessages[] = {
                                 "sem erro - Without Error",
                                 "polinomio(s) inexistente(s) - Polynomial(s) do not exist",
                                 "memoria esgotada - Out of memory",
                                 "ficheiro inexistente - File does not exist",
                                 "grau do polinomio errado - Wrong degree",
                                 "coeficiente inexistente no polinomio - Coefficient does not exist"
                               };

static char *AbnormalErrorMessage = "erro desconhecido - Unknown error";

/************** Número de mensagens de erro previstas no módulo ***************/

#define N (sizeof (ErrorMessages) / sizeof (char *))

/************************ Alusão às Funções Auxiliares ************************/

static int ValidPolys (PtPoly, PtPoly);
static void ReducePoly (PtPoly);

/*************************** Definição das Funções ****************************/

void PolyClearError (void)
{
  Error = OK;
}

int PolyError (void)
{
  return Error;
}

char *PolyErrorMessage (void)
{
  if (Error < N) return ErrorMessages[Error];
  else return AbnormalErrorMessage;    /* não há mensagem de erro - no error message */
}

PtPoly PolyCreate (unsigned int pdegree)
{

  	PtPoly Poly;

	/* Valida o grau do polinómio */
	if((Poly = (PtPoly) malloc(sizeof (struct poly))) == NULL){
		Error = NO_MEM;
		return NULL;
	}

	/* Cria o polinómio nulo */
	if((Poly->Poly = (double *) calloc(pdegree+1, (sizeof (double)))) == NULL){
		free(Poly);
		Error = NO_MEM;
		return NULL;
	}

	/* Guarda o grau do polinómio */
	Poly->Degree = pdegree;

	/* Não ocorreram erros */
	Error = OK;

	/* Devolve o polinómio criado */
	return Poly;

}

void PolyDestroy (PtPoly *ppol)
{

 	PtPoly TmpPoly = *ppol;

	/* Verifica se o polinómio existe */
	if(TmpPoly == NULL){
		Error = NO_POLY;
		return;
	}

	/* Liberta memória alocada */
	free(TmpPoly->Poly);
	free(TmpPoly);

	Error = OK;
	*ppol = NULL;

}

PtPoly PolyCopy (PtPoly ppol)
{

  	PtPoly Copy;
	unsigned int i;

	/* Verifica se o polinómio existe */
	if(ppol == NULL){
		Error = NO_POLY;
		return NULL;
	}

	/* Cria o polinómio nulo */
	if((Copy = PolyCreate(ppol->Degree)) == NULL){
		Error = NO_MEM;
		return NULL;
	}
	
	/* Copia o polinómio */
	for(i = 0; i < ppol->Degree+1; i++){
		Copy->Poly[i] = ppol->Poly[i];
	}

	Error = OK;

	/* Devolve o polinómio */
	return Copy;

}

int PolyDegree (PtPoly ppol)
{
  	/* Verifica se o polinómio existe */
	if(ppol == NULL){
		Error = NO_POLY;
		return 0;
	}

	Error = OK;

	/* Devolve o grau do polinómio */
	return ppol->Degree;

}

void PolyModifyCoefficient (PtPoly ppol, unsigned int ppos, double pvalue)
{

  	/* Verifica se o polinómio existe */
	if(ppol == NULL){
		Error = NO_POLY;
		return;
	}

	/* Valida o elemento pretendido */
	if(ppos < 0 || ppos > ppol->Degree){
		Error = BAD_INDEX;
		return;
	}

	Error = OK;
	/* Armazena o pvalue no índice ppos, do polinómio ppol */
	ppol->Poly[ppos] = pvalue;

}

double PolyObserveCoefficient (PtPoly ppol, unsigned int ppos)
{
  	
	/* Verifica se o polinómio existe */
	if(ppol == NULL){
		Error = NO_POLY;
		return 0.0;
	}

	/* Valida o elemento pretendido */
	if(ppos < 0 || ppos > ppol->Degree){
		Error = BAD_INDEX;
		return 0.0;
	}

	Error = OK;
	/* Devolve o valor armazenado no índice ppos do polinómio ppol */
	return ppol->Poly[ppos];

}

int PolyIsNull (PtPoly ppol)
{
  	
	unsigned int i;

	/* Verifica se o polinómio existe */
	if(ppol == NULL){
		Error = NO_POLY;
		return 0;
	}

	/* Verifica se o polinómio não é nulo */
	for(i = 0; i < ppol->Degree+1; i++) {
		if(ppol->Poly[i]) 
			return 0;
	}

	Error = OK;
	/* O polinómio é nulo */
	return 1;

}

PtPoly PolyAddition (PtPoly ppol1, PtPoly ppol2)
{
  	
	PtPoly Add;
	unsigned int i, Degree;

	/* Valida os polinómios */
	if(!ValidPolys(ppol1, ppol2)){
		Error = NO_POLY;
		return NULL;
	}

	/* Determina o grau do polinómio soma */
	if(ppol1->Degree > ppol2->Degree)
		Degree = ppol1->Degree;
	else
		Degree = ppol2->Degree;

	/* Cria o polinómio soma nulo */
	if((Add = PolyCreate(Degree)) == NULL){
		Error = NO_MEM;
		return NULL;
	}

	/* Soma os dois polinómios */
	for(i = 0; i < Degree+1; i++){
		if(i < ppol1->Degree+1)
			Add->Poly[i] += ppol1->Poly[i];
		if(i < ppol2->Degree+1)
			Add->Poly[i] += ppol2->Poly[i];
	}

	ReducePoly(Add);
	Error = OK;	
	/* Devolve o polinómio soma */
	return Add;

}

PtPoly PolySubtraction (PtPoly ppol1, PtPoly ppol2)
{
 
	PtPoly Sub;
	unsigned int i, Degree;

	/* Valida os polinómios */
	if(!ValidPolys(ppol1, ppol2)){
		Error = NO_POLY;		
		return NULL;
	}

	/* Determina o grau do polinómio subtração */
	if(ppol1->Degree > ppol2->Degree)
		Degree = ppol1->Degree;
	else
		Degree = ppol2->Degree;

	/* Cria o polinómio subtração nulo */
	if((Sub = PolyCreate(Degree)) == NULL){
		Error = NO_MEM;		
		return NULL;	
	}

	/* Subtrai os dois polinómios */
	for(i = 0; i < Degree+1; i++){
		if(i < ppol1->Degree+1)
			Sub->Poly[i] += ppol1->Poly[i];		
		if(i < ppol2->Degree+1)
			Sub->Poly[i] -= ppol2->Poly[i];
	}

	ReducePoly(Sub);
	Error = OK;
	/* Devolve o polinómio subtração */
	return Sub;

}

PtPoly PolyMultiplication (PtPoly ppol1, PtPoly ppol2)
{
  
	PtPoly Mul;
	unsigned int i, j, Degree;

	/* Valida os polinómios */
	if(!ValidPolys(ppol1, ppol2)){
		Error = NO_POLY;		
		return NULL;
	}

	Degree = ppol1->Degree + ppol2->Degree;

	/* Cria o polinómio multiplicação nulo */
	if((Mul = PolyCreate(Degree)) == NULL){
		Error = NO_MEM;	
		return NULL;
	}

	/* Multiplica os dois polinómios */
	for(i = 0; i < ppol1->Degree+1; i++){
		for(j = 0; j < ppol2->Degree+1; j++){
			Mul->Poly[i+j] += ppol1->Poly[i] * ppol2->Poly[j];
		}
	}
	
	ReducePoly(Mul);
	Error = OK;
	/* Devolve o polinómio multiplicação */
	return Mul;

}

int PolyEquals (PtPoly ppol1, PtPoly ppol2)
{

  	unsigned int i;

	/* Valida os polinómios */
	if(!ValidPolys(ppol1, ppol2)){
		Error = NO_POLY;
		return 0;
	}

	/* Compara os graus dos polinómios */
	if(ppol1->Degree != ppol2->Degree)
		return 0;

	/* Compara os polinómios */
	for(i = 0; i < ppol1->Degree+1; i++){
		if(ppol1->Poly[i] != ppol2->Poly[i])
			return 0;
	}

	Error = OK;
	/* Os polinómios são iguais */
	return 1;

}

void PolyStoreFile (PtPoly ppol, char *pnomef)
{
 
	FILE *PtF;
	unsigned int i;

	/* Verifica se o polinómio existe */
	if (ppol == NULL){ 
		Error = NO_POLY;
		return ;
	}

	/* Abertura com validação do ficheiro para escrita */
	if ((PtF = fopen (pnomef, "w")) == NULL){
		Error = NO_FILE;
		return;
	}

	/* Escrita do grau do polinómio no ficheiro */
	fprintf (PtF, "%u\n", ppol->Degree);

	/* Escrita dos coeficientes do polinómio no ficheiro */
	for (i = 0; i < ppol->Degree+1; i++) 
		fprintf (PtF, "%lf\n", ppol->Poly[i]);

	Error = OK;
	/* Fecho do ficheiro */
	fclose (PtF);

}

PtPoly PolyCreateFile (char *pnomef)
{
 
	PtPoly Poly;
	FILE *PtF;
	unsigned int Degree, i;

	/* Abertura com validação do ficheiro para leitura */
	if ( (PtF = fopen (pnomef, "r")) == NULL){
		Error = NO_FILE;
		return NULL;
	}

	/* Leitura do grau do polinómio do ficheiro */
	fscanf (PtF, "%u", &Degree);
	if (Degree < 0){
		fclose (PtF);			
		Error = BAD_DEGREE;
		return NULL;
	}

	/* Criação do polinómio nulo */
	if ((Poly = PolyCreate (Degree)) == NULL){
		fclose (PtF);
		Error = NO_MEM;
		return NULL;
	}

	/* Leitura dos coeficientes do polinómio do ficheiro */
	for (i = 0; i < Degree+1; i++){
		fscanf (PtF, "%lf", Poly->Poly+i);
	}

	/* Fecho do ficheiro */
	fclose (PtF);
	Error = OK;
	
	/* Devolve o polinómio criado */
	return Poly;

}

double PolyEvaluation (PtPoly ppoly, double px)
{
 	
	unsigned int i;
	double resultado = ppoly->Poly[0];

	/* Verifica se o polinómio é nulo */
	if (PolyIsNull (ppoly)){
		Error = NO_POLY;
		return 0.0;
	}

	/* Faz os cálculos para o polinómio com o argumento dado */
	for(i = 1; i <= ppoly->Degree; i++)
		resultado += ppoly->Poly[i] * pow(px, i);

	Error = OK;
	/* Devolve o resultado */
	return resultado;

}

/*******************************************************************************
 Função auxiliar que verifica se os dois polinómios existem. Devolve 1 em caso
 afirmativo e 0 em caso contrário. Valores de erro: OK ou NO_POLY.
  
 Auxiliary function to verify if the two polynomials exist. Returns 1 in 
 affirmative case and 0 otherwise. Error codes: OK or NO_POLY. 
*******************************************************************************/

static int ValidPolys (PtPoly ppoly1, PtPoly ppoly2)
{
  /* verifica se os dois polinómios existem - verifies if the two polynomials exist */
  if ((ppoly1 == NULL) || (ppoly2 == NULL)) { Error = NO_POLY; return 0; }

  Error = OK;
  return 1;  /* os dois polinómios existem - they exist */
}

/*******************************************************************************
 Função auxiliar que reduz o polinómio ao menor grau possível. Valores de erro:
 NO_POLY.
  
 Auxiliary function to reduce the polynomial to its least degree. Error codes: NO_POLY. 
  
*******************************************************************************/

static void ReducePoly (PtPoly ppoly)
{
  unsigned int Degree;

  /* verifica se o polinómio existe - verifies if the polynomial exists */
  if (ppoly == NULL) { Error = NO_POLY; return ; }

  Degree = ppoly->Degree;

  while (Degree > 0 && ppoly->Poly[Degree] == 0.0) Degree--;

  if (Degree != ppoly->Degree)
     ppoly->Poly = (double *) realloc (ppoly->Poly, (Degree+1) * sizeof (double));

  ppoly->Degree = Degree;
  Error = OK;
}

