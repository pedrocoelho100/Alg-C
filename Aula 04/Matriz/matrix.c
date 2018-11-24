/*******************************************************************************

 Ficheiro de implementação do Tipo de Dados Abstracto MATRIZ (matrix.c).
 A estrutura de dados de suporte da matriz é uma estrutura, constituída pelo
 campo NL para armazenar o número de linhas da matriz, o campo NC para armazenar
 o número de colunas da matriz e o campo de tipo ponteiro para ponteiro Matrix
 para armazenar os seus NLxNC elementos reais, que vão ser armazenados numa
 sequência bidimensional atribuída dinamicamente.

 Nome :  Pedro Miguel André Coelho             NMec:  68803

 Implementation file of the abstract data type MATRIX (matrix.c). The data
 structure to store the matrix is composed with the integer field NL for keeping
 the matrix's number of lines, the integer field NC for keeping the matrix's number
 of columns and the pointer field Matrix, that represents the allocated bi-dimensional
 array in dynamic memory, for storing the matrix's NLxNC real elements.

*******************************************************************************/

#include <stdio.h>  
#include <stdlib.h>

#include "matrix.h"  /* Ficheiro de interface do TDA - ADT Interface file */

/************* Definição da Estrutura de Dados Interna da MATRIZ **************/

struct matrix
{
  unsigned int NL;  /* numero de linhas da matriz - matrix's number of lines */
  unsigned int NC;  /* numero de colunas da matriz - matrix's number of columns */
  double **Matrix;  /* ponteiro para a matriz a ser alocada - pointer to matrix's elements */
};

/*********************** Controlo Centralizado de Erro ************************/

static unsigned int Error = OK;  /* inicialização do erro */

static char *ErrorMessages[] = {
                                 "sem erro - Without error",
                                 "matriz(es) inexistente(s) - Matrix(es) do not exist",
                                 "memoria esgotada - Out of memory",
                                 "ficheiro inexistente - File does not exist",
                                 "dimensao errada - Wrong size", 
                                 "elemento inexistente na matriz - Element does not exist",
                                 "matrizes com dimensoes diferentes - Matrixes with different sizes",
                                 "matrizes nao encadeadas - Matrixes not chained",
								 "ponteiro nulo - Null pointer",
								 "matriz nao quadrada - Matrix not square",
								 "linha inexistente - Line does not exist",
								 "coluna inexistente - Column does not exist"
                               };

static char *AbnormalErrorMessage = "erro desconhecido - Unknown error";

/************** Número de mensagens de erro previstas no módulo ***************/

#define N (sizeof (ErrorMessages) / sizeof (char *))

/************************ Alusão às Funções Auxiliares ************************/

static int EqualSizeMatrixes (PtMatrix, PtMatrix);
static int ChainedMatrixes (PtMatrix, PtMatrix);
static double Determinant (double *, unsigned int, unsigned int);

/*************************** Definição das Funções ****************************/

void MatrixClearError (void)
{
  Error = OK;
}

int MatrixError (void)
{
  return Error;
}

char *MatrixErrorMessage (void)
{
  if (Error < N) return ErrorMessages[Error];
  else return AbnormalErrorMessage;  /* não há mensagem de erro - no error message */
}

PtMatrix MatrixCreate (unsigned int pnl, unsigned int pnc)
{
  	PtMatrix Matrix;
	unsigned int i, j;

	/* Valida a matriz */
	if((Matrix = (PtMatrix) malloc(sizeof (struct matrix))) == NULL) {
		Error = NO_MEM;
		return NULL;
	}

	/* Cria a matriz nula */
	if((Matrix->Matrix = (double **) calloc(pnl, sizeof(double *))) == NULL) {
		Error = NO_MEM;
		free(Matrix);
		return NULL;
	}

	/* Cria a matriz */
    	for(i = 0; i < pnl; i++){
		if((Matrix->Matrix[i] = (double *) calloc(pnc, sizeof(double))) == NULL){
			for(j = 0; j < i; j++)
				free(Matrix->Matrix[j]);
			
			free(Matrix->Matrix);
			free(Matrix);
			Error = NO_MEM;
			return NULL;		
		}
    	}

	Matrix->NL = pnl;
    	Matrix->NC = pnc;

	Error = OK;
	/* Devolve a matriz criada */
   	return Matrix;
}

PtMatrix MatrixCreateArray (unsigned int pnl, unsigned int pnc, double *array)
{
  	PtMatrix Matrix;
	unsigned int i, j;
	int rowBeggining = 0;

	/* Verifica a dimensão da matriz */
	 if(pnl < 1 || pnc < 1) {
		Error = BAD_SIZE;
		return NULL;
	}	

	/* Valida a matriz */
	if((Matrix = (PtMatrix) malloc(sizeof (struct matrix))) == NULL) {
		Error = NULL_PTR;
		return NULL;

	}

	/* Cria a matriz nula */
	if((Matrix->Matrix = (double **) calloc(pnl, sizeof(double *))) == NULL) {
		Error = NO_MEM;
		free(Matrix);
		return NULL;
	}

	/* Cria a matriz */
	for(i = 0; i < pnl; i++) {
		if((Matrix->Matrix[i] = (double *) calloc(pnc, sizeof(double))) == NULL) {
			for(j = 0; j < i; j++) {
				free(Matrix->Matrix[j]);
			}
			free(Matrix->Matrix);
			free(Matrix);
			Error = NO_MEM;
			return NULL;
		}
		for(i = 0; i < pnl; i++) {				
			for(j = 0; j < pnc; j++) {		
				Matrix->Matrix[i][j] = array[rowBeggining+j];
			}
			/* Avança pmat->pnc colunas */
			rowBeggining = i * pnc;
		}
	}

	Matrix->NL = pnl;
    	Matrix->NC = pnc;

	Error = OK;
	/* Devolve a matriz criada */
	return Matrix;
}

void MatrixDestroy (PtMatrix *pmat)
{
  	PtMatrix TmpMatrix = *pmat;
	unsigned int i;

	/* Verifica se a matriz existe */
	if(TmpMatrix == NULL) {
		Error = NO_MATRIX;
		return;
	}

	/* Liberta espaço */
	for(i = 0; i < TmpMatrix->NL; i++)                      		
		free(TmpMatrix->Matrix[i]);

	free(TmpMatrix->Matrix);
	free(TmpMatrix);

	Error = OK;
	/* Referência do ponteiro a NULL */
	*pmat = NULL;
}

PtMatrix MatrixCopy (PtMatrix pmat)
{
  	PtMatrix Copy;
	unsigned int i, j;

	/* Verifica se a matriz existe */
	if(pmat == NULL) {
		Error = NO_MATRIX;
		return NULL;
	}

	/* Cria uma nova matriz */
	if((Copy = MatrixCreate(pmat->NL, pmat->NC)) == NULL) {
		Error = NO_MEM;		
		return NULL;
	}

	/* Copia os valores da matriz pmat para a Copy */
	for(i = 0; i < pmat->NL; i++) {				
		for(j = 0; j < pmat->NC; j++) {		
			Copy->Matrix[i][j] = pmat->Matrix[i][j];
		}
	}

	Error = OK;
	/* Devolve a matriz copiada */
	return Copy;
}

void MatrixSize (PtMatrix pmat, unsigned int *pnl, unsigned int *pnc)
{
  /* verifica se a matriz existe - verifies if matrix exist */
  if (pmat == NULL)
  {
    Error = NO_MATRIX; 
    *pnl = *pnc = 0;
  }
  else
  {
    Error = OK;
    *pnl = pmat->NL;
    *pnc = pmat->NC;
  }
}

void MatrixModifyElement (PtMatrix pmat, unsigned int pl, unsigned int pc, double pval)
{
  /* verifica se a matriz existe - verifies if matrix exist */
  if (pmat == NULL) { Error = NO_MATRIX; return ; }

  /* validação do elemento pretendido - validating the element position */
  if ((pl < 0) || (pl >= pmat->NL) || (pc < 0) || (pc >= pmat->NC))
  { Error = BAD_INDEX; return ; }

  Error = OK;
  /* escrita do valor na componente pretendida da matriz - storing the new value in the element */
  pmat->Matrix[pl][pc] = pval;
}

double MatrixObserveElement (PtMatrix pmat, unsigned int pl, unsigned int pc)
{
  /* verifica se a matriz existe - verifies if matrix exist */
  if (pmat == NULL) { Error = NO_MATRIX; return 0; }

  /* validação do elemento pretendido - validating the element position */
  if ((pl < 0) || (pl >= pmat->NL) || (pc < 0) || (pc >= pmat->NC))
  { Error = BAD_INDEX; return 0; }

  Error = OK;
  /* devolve o valor armazenado na componente pretendida da matriz - returning the element value */
  return pmat->Matrix[pl][pc];
}

PtMatrix MatrixTranspose (PtMatrix pmat)
{
 	PtMatrix Transpose;
	unsigned int i, j;

	/* Verifica se a matriz existe */
	if(pmat == NULL) {
		Error = NO_MATRIX;
		return NULL;
	}

	/* Cria uma nova matriz com o mesmo tamanho de pmat */
	if((Transpose = MatrixCreate(pmat->NC, pmat->NL)) == NULL) {
		Error = NO_MEM;				
		return NULL;
	}

	/* Troca linhas por colunas */
	for(i = 0; i < pmat->NL; i++) {							
		for(j = 0; j < pmat->NC; j++) {						
			Transpose->Matrix[j][i] = pmat->Matrix[i][j];
		}
	}

	Error = OK;
	/* Devolve a matriz transposta */
	return Transpose;
}

PtMatrix MatrixAddition (PtMatrix pmat1, PtMatrix pmat2)
{
  	PtMatrix Add;
	unsigned int i, j;

	/* Verifica se as matrizes existem */
	if ((pmat1 == NULL) || (pmat2 == NULL)) { 
		Error = NO_MATRIX; 
		return 0; 
	}

	/* Verifica se têm a mesma dimensão */
	if(!EqualSizeMatrixes(pmat1, pmat2)) {
		Error = DIF_SIZE;
		return NULL;
	}

	/* Cria uma nova matriz com o mesmo tamanho de pmat1 */
	if((Add = MatrixCreate(pmat1->NL, pmat1->NC)) == NULL) {	
		Error = NO_MEM;				
		return NULL;
	}
	/* Adiciona cada uma das linhas e colunas de pmat1 e pmat2 */
	for(i = 0; i < pmat1->NL; i++) {							
		for(j = 0; j < pmat1->NC; j++) {		
			Add->Matrix[i][j] = pmat1->Matrix[i][j] + pmat2->Matrix[i][j];
		}	
	}

	Error = OK;
	/* Devolve a matriz soma */
	return Add;
}

PtMatrix MatrixSubtraction (PtMatrix pmat1, PtMatrix pmat2)
{
 	PtMatrix Sub;
	unsigned int i, j;

	/* Verifica se as matrizes existem */
	if ((pmat1 == NULL) || (pmat2 == NULL)) { 
		Error = NO_MATRIX; 
		return 0; 
	}

	/* Verifica se têm a mesma dimensão */
	if(!EqualSizeMatrixes(pmat1, pmat2)) {
		Error = DIF_SIZE;
		return NULL;
	}

	/* Cria uma nova matriz com o mesmo tamanho de pmat1 */
	if((Sub = MatrixCreate(pmat1->NL, pmat1->NC)) == NULL) {
		Error = NO_MEM;				
		return NULL;
	}

	/* Subtrai cada uma das linhas e colunas de pmat1 e pmat2 */
	for(i = 0; i < pmat1->NL; i++) {				
		for(j = 0; j < pmat1->NC; j++) {					
			Sub->Matrix[i][j] = pmat1->Matrix[i][j] - pmat2->Matrix[i][j];
		}
	}

	Error = OK;
	/* Devolve a matriz subtração */
	return Sub;
}

PtMatrix MatrixMultiplication (PtMatrix pmat1, PtMatrix pmat2)
{
  	PtMatrix Mul;
	unsigned int i, j, k;

	/* Verifica se as matrizes existem */
	if ((pmat1 == NULL) || (pmat2 == NULL)) { 
		Error = NO_MATRIX; 
		return 0; 
	}

	/* Verifica se as duas matrizes são encadeadas */
	if(!ChainedMatrixes(pmat1, pmat2)){
		Error = NO_CHAINED;		
		return 0;
	}

	/* Cria uma nova matriz com o mesmo tamanho de pmat1 e pmat2*/
	if((Mul = MatrixCreate(pmat1->NL, pmat2->NC)) == NULL) {
		Error = NO_MEM;					
		return NULL;
	}

	/* Multiplica cada linha do pmat1 por cada coluna do pmat2 e armazena na matriz Mul */
	for(i = 0; i < Mul->NL; i++) {						
		for(j = 0; j < Mul->NC; j++) {	
			for(k = 0; k < pmat1->NC; k++) {	
				Mul->Matrix[i][j] += pmat1->Matrix[i][k] * pmat2->Matrix[k][j];
			}
		}
	}

	Error = OK;
	/* Devolve a matriz multiplicação */
	return Mul;
}

PtMatrix MatrixMultByScalar (PtMatrix pmat, double pvalue)
{
  	PtMatrix Matrix;
	unsigned int i, j;

	/* Verifica se a matriz existe */
	if (pmat == NULL){
		Error = NO_MATRIX;
		return NULL;
	}

	/* Cria uma nova matriz com o mesmo tamanho */
	if((Matrix = MatrixCreate(pmat->NL, pmat->NC)) == NULL) {		
		Error = NO_MEM;    		
		return NULL;
	}

	/* Multiplica cada linha e coluna do pmat por pvalue */
	for (i = 0; i < pmat->NL; i++) {
		for (j = 0; j < pmat->NC; j++) {
			Matrix->Matrix[i][j] = pvalue * pmat->Matrix[i][j];
		}
	}

	Error = OK;
	/* Devolve a matriz multiplicada pelo valor escalar */
	return Matrix;
}

double MatrixDeterminant (PtMatrix pmat)
{
  	unsigned int i, j; 
	double array[pmat->NL*pmat->NC]; 

	/* Verifica se a matriz existe */
	if(pmat == NULL){ 
		Error = NO_MATRIX; 
		return 0.0; 
	} 

	/* Verifica se a matriz é quadrada */
	if(!MatrixIsSquare(pmat)){ 
		Error = NO_SQUARE; 
		return 0.0; 
	} 

	/* Coloca a matriz num array */
	for(i = 0; i < pmat->NL; i++){ 
		for (j = 0; j < pmat->NC; j++){ 
			array[i*pmat->NL+j] = pmat->Matrix[i][j]; 
		} 
	} 

	Error = OK;
	/* Devolve o valor do determinante */
	return Determinant(array, pmat->NL, pmat->NC);
}

int MatrixIsSquare (PtMatrix pmat)
{
	/* Verifica se a matriz existe */
	if(pmat == NULL) {
		Error = NO_MATRIX;
		return 0;
	}

	/* Verifica se a matriz é quadrada */
	if(pmat->NL != pmat->NC)
		return 0;
	
	Error = OK;
	/* A matriz é quadrada */	
	return 1;
}

int MatrixEquals (PtMatrix pmat1, PtMatrix pmat2)
{
  unsigned int l, c;

  /* validação das matrizes - validating the existence of the two matrixes */
  if (!EqualSizeMatrixes (pmat1, pmat2)) return 0;

  Error = OK;
  /* comparação dos elementos das duas matrizes - comparing the respective elements */
  for (l = 0; l < pmat1->NL; l++)
    for (c = 0; c < pmat1->NC; c++)
      if (pmat1->Matrix[l][c] != pmat2->Matrix[l][c]) return 0;

  return 1;  /* as matrizes são iguais - the two matrixes are equal */
}

void MatrixExchangeRow (PtMatrix pmatrix, unsigned int pk, unsigned int pl)
{
 	unsigned int i;
	double temp;	

	/* Verifica se a matriz existe */
	if (pmatrix == NULL){
		Error = NO_MATRIX;
		return;
	}

	/* Verifica as linhas da matriz */
	if(pk < 1 || pk > pmatrix->NL || pl < 1 || pl > pmatrix->NL){
		Error = BAD_ROW;
		return;
	} 

	/* Troca as linhas da matriz */
	for (i = 0; i < pmatrix->NL; i++){
		temp = pmatrix->Matrix[pl-1][i];
		pmatrix->Matrix[pl-1][i] = pmatrix->Matrix[pk-1][i];
		pmatrix->Matrix[pk-1][i] = temp;
	}

	Error = OK;
}

void MatrixExchangeColumn (PtMatrix pmatrix, unsigned int pk, unsigned int pc)
{
  	unsigned int i;
	double temp;	

	/* Verifica se a matriz existe */
	if (pmatrix == NULL){
		Error = NO_MATRIX;
		return;
	}

	/* Verifica as colunas da matriz */
	if(pk < 1 || pk > pmatrix->NC || pc < 1 || pc > pmatrix->NC){
		Error = BAD_COLUMN;
		return;
	} 

	/* Troca as colunas da matriz */
	for (i = 0; i < pmatrix->NC; i++){
		temp = pmatrix->Matrix[i][pc-1];
		pmatrix->Matrix[i][pc-1] = pmatrix->Matrix[i][pk-1];
		pmatrix->Matrix[i][pk-1] = temp;
	}

	Error = OK;
}

void MatrixStoreFile (PtMatrix pmat, char *pnomef)
{
  FILE *PtF; unsigned int l, c;

  /* verifica se a matriz existe - verifies if matrix exists */
  if (pmat == NULL) { Error = NO_MATRIX; return ; }

  /* abertura com validacao do ficheiro para escrita - opening the text file for writing */
  if ((PtF = fopen (pnomef, "w")) == NULL) { Error = NO_FILE; return ; }

  /* escrita da dimensão da matriz no ficheiro - writing the matrix's size */
  fprintf (PtF, "%d\t%d\n", pmat->NL, pmat->NC);

  /* escrita dos elementos da matriz no ficheiro - writing the matrix's elements */
  for (l= 0; l < pmat->NL; l++)
  {
    for (c = 0; c < pmat->NC; c++) fprintf (PtF, "%lf\t", pmat->Matrix[l][c]);
    fprintf (PtF, "\n");
  }

  Error = OK;
  fclose (PtF);  /* fecho do ficheiro - closing the text file */
}

PtMatrix MatrixCreateFile (char *pnomef)
{
  PtMatrix Mat; FILE *PtF; unsigned int NL, NC, l, c;

  /* abertura com validacao do ficheiro para leitura - opening the text file for reading */
  if ( (PtF = fopen (pnomef, "r")) == NULL)
  { Error = NO_FILE; return NULL; }

  /* leitura da dimensão da matriz do ficheiro - reading the matrix's size from the text file */
  fscanf (PtF, "%d%d", &NL, &NC);
  if ((NL < 1) || (NC < 1)) { Error = BAD_SIZE; fclose (PtF); return NULL; }

  /* criação da matriz nula - creating an empty matrix */
  if ((Mat = MatrixCreate (NL, NC)) == NULL)
  { fclose (PtF); return NULL; }

  /* leitura dos elementos da matriz do ficheiro - reading the matrix's elements from the text file */
  for (l = 0; l < NL; l++) 
  {
    for (c = 0; c < NC; c++) fscanf (PtF, "%lf", &Mat->Matrix[l][c]);
    fscanf (PtF, "%*[^\n]"); fscanf (PtF, "%*c");
  }

  fclose (PtF);  /* fecho do ficheiro - closing the text file */

  return Mat;  /* devolve a matriz criada - returning the new matrix */
}

/*******************************************************************************
 Função auxiliar que verifica se as duas matrizes podem ser operadas (com excepção
 da operação de multiplicação), ou seja, se existem e se têm a mesma dimensão.
 Devolve 1 em caso afirmativo e 0 em caso contrário. Valores de erro: OK, NO_MATRIX
 ou DIF_SIZE.
 
 Auxiliary function to verify if the two matrixes can be operated (with the exception
 of the multiplication), that is, if they exist and have the same size. Returns 1
 in affirmative case and 0 otherwise. Error codes: OK, NO_MATRIX or DIF_SIZE. 
*******************************************************************************/
static int EqualSizeMatrixes (PtMatrix pmat1, PtMatrix pmat2)
{
  /* verifica se as duas matrizes existem - verifies if the two matrixes exist */
  if ((pmat1 == NULL) || (pmat2 == NULL)) { Error = NO_MATRIX; return 0; }

  /* verifica se a dimensão das duas matrizes é igual - verifies if they have the same size */
  if ((pmat1->NL != pmat2->NL) || (pmat1->NC != pmat2->NC))
  { Error = DIF_SIZE; return 0; }

  Error = OK;
  return 1;  /* as duas matrizes existem e têm a mesma dimensão - they exist and have the same size */
}

/*******************************************************************************
 Função auxiliar que verifica se as duas matrizes podem ser multiplicadas, ou
 seja, se existem e se são encadeadas (o número de colunas da primeira é igual ao 
 número de linhas da segunda). Devolve 1 em caso afirmativo e 0 em caso contrário.
 Valores de erro: OK, NO_MATRIX ou NO_CHAINED.
 
 Auxiliary function to verify if the two matrixes can be multiplied, that is, if they
 exist and are chained (the number of columns of the first is equal to the number
 of lines of the second). Returns 1 in affirmative case and 0 otherwise. Error 
 codes: OK, NO_MATRIX or DIF_SIZE. 
*******************************************************************************/
static int ChainedMatrixes (PtMatrix pmat1, PtMatrix pmat2)
{
  /* verifica se as duas matrizes existem - verifies if the two matrixes exist */
  if ((pmat1 == NULL) || (pmat2 == NULL)) { Error = NO_MATRIX; return 0; }

  /* verifica se as duas matrizes são encadeadas - verifies if they are chained */
  if (pmat1->NC != pmat2->NL) { Error = NO_CHAINED; return 0; }

  Error = OK;
  return 1;  /* as duas matrizes existem e são encadeadas - they exist and are chained */
}

/*******************************************************************************
 Função auxiliar que calcula o determinante de uma matriz quadrada usando o 
 algoritmo de eliminação de Gauss. Transforma a matriz numa matriz triangular
 superior. Recebe um array unidimensional que armazena os elementos da matriz,
 linha a linha.
 
 Auxiliary function that calculates the determinant of a square matrix. It uses 
 the Gauss elimination algorithm that transforma the matrix in a superior triangular
 matrix. It receives a one-dimension array that stores the matrix elements, line by line.
*******************************************************************************/
static double Determinant (double *pmatrix, unsigned int psize, unsigned int pn)
{
	unsigned int AuxCol, NC, NL, LC = pn-1; double Elem;

	if (pn == 1) return *pmatrix;	/* condição de paragem - stop recursion condition */
	else
	{   /* procurar coluna com último elemento não nulo - search for column with last element not zero */
		AuxCol = LC;
 		while (AuxCol >= 0 && *(pmatrix+LC*psize+AuxCol) == 0) AuxCol--;

		if (AuxCol >= 0)	/* se existir tal coluna - if such column exists */
		{
			if (AuxCol != LC)	/* se não for a última coluna - if it is not the last column */
				for (NL = 0; NL < pn; NL++)	/* trocar as colunas - change columns */
				{
					Elem = *(pmatrix+NL*psize+LC);
					*(pmatrix+NL*psize+LC) = *(pmatrix+NL*psize+AuxCol);
					*(pmatrix+NL*psize+AuxCol) = -Elem;
				}

			/* dividir a última coluna pelo último elemento - divide the last column by the last element */
			for (NL = 0; NL < LC; NL++) *(pmatrix+NL*psize+LC) = *(pmatrix+NL*psize+LC) / *(pmatrix+LC*psize+LC);

			/* subtrair todas as colunas menos a última pela última coluna */
			/* multiplicada pelo último elemento da coluna a processar */
			/* process all other columns in order to eliminate the last element of the column */
			for (NC = 0; NC < LC; NC++)
				for (NL = 0; NL < LC; NL++)
					*(pmatrix+NL*psize+NC) = *(pmatrix+NL*psize+NC) - (*(pmatrix+NL*psize+LC) * *(pmatrix+LC*psize+NC));

			/* invocação recursiva para a matriz de dimensão N-1 */
			/* invocação recursiva para a matriz de dimensão N-1 */
			return *(pmatrix+LC*psize+LC) * Determinant (pmatrix, psize, pn-1);
		}
		else return 0.0;
	}
}

