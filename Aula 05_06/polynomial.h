/*******************************************************************************

 Ficheiro de interface do Tipo de Dados Abstrato POLY (polynomial.h).
 O polinómio é composto pelo seu grau e pelos seus coeficientes reais, sendo
 que um polinómio com grau n tem n+1 coeficientes. A implementação providencia
 um construtor para criar um polinómio nulo com o grau pretendido. É da
 responsabilidade da aplicação, invocar o destrutor, para libertar a memória
 atribuída ao objecto. O módulo providencia um controlo centralizado de erro,
 disponibilizando uma função para obter o último erro ocorrido, uma função para
 obter uma mensagem de erro elucidativa e uma função para limpar o erro.
 Providencia também operações para armazenar e recuperar polinómios para ficheiros.

 Autor : António Manuel Adrego da Rocha    Data : Março de 2016

*******************************************************************************/

#ifndef _POLY
#define _POLY

/**************** Definição do Tipo Ponteiro para um Polinómio ****************/

typedef struct poly *PtPoly;

/************************ Definição de Códigos de Erro ************************/


#define  OK         0  /* operação realizada com sucesso - operation with success */
#define  NO_POLY    1  /* o(s) polinómio(s) não existe(m) - the polynomial(s) do not exist */
#define  NO_MEM     2  /* memória esgotada - out of memory */
#define  NO_FILE    3  /* o ficheiro não existe - file does not exist */
#define  BAD_DEGREE 4  /* grau do polinómio errado - wrong degree */
#define  BAD_INDEX  5  /* coeficiente inexistente no polinómio - coefficient does not exist in the polynomial */

/*************************** Protótipos das Funções ***************************/

void PolyClearError (void);
/*******************************************************************************
 Inicialização do erro. Error initialization.
*******************************************************************************/

int PolyError (void);
/*******************************************************************************
 Função que devolve o código do último erro ocorrido. Returns the error code.
*******************************************************************************/

char *PolyErrorMessage (void);
/*******************************************************************************
 Função que devolve uma mensagem esclarecedora da causa do último erro ocorrido.
*******************************************************************************/

PtPoly PolyCreate (unsigned int pdegree);
/*******************************************************************************
 Cria um polinómio nulo, de coeficientes reais, com o grau indicado por pdegree.
 Devolve a referência do polinómio criado ou NULL, caso não consiga criar o
 polinómio, por falta de memória. Valores de erro: OK ou NO_MEM.
*******************************************************************************/

void PolyDestroy (PtPoly *ppoly);
/*******************************************************************************
 Destrói o polinómio ppoly, caso ele exista, e coloca a sua referência a NULL.
 Valores de erro: OK ou NO_POLY.
*******************************************************************************/

PtPoly PolyCopy (PtPoly ppoly);
/*******************************************************************************
 Copia do polinómio ppoly, caso ele exista. Devolve a referência do polinómio cópia
 ou NULL, caso não consiga fazer a cópia. Valores de erro: OK, NO_POLY ou NO_MEM.
*******************************************************************************/

int PolyDegree (PtPoly ppoly);
/*******************************************************************************
 Devolve o grau do polinómio ppoly. Valores de erro: OK ou NO_POLY.
*******************************************************************************/

void PolyModifyCoefficient (PtPoly ppoly, unsigned int ppos, double pvalue);
/*******************************************************************************
 Armazena pvalue no índice ppos, do polinómio ppoly. Valores de erro: OK, NO_POLY
 ou BAD_INDEX.
*******************************************************************************/

double PolyObserveCoefficient (PtPoly ppoly, unsigned int ppos);
/*******************************************************************************
 Devolve o valor armazenado no índice ppos do polinómio ppoly. Valores de erro:
 OK, NO_POLY ou BAD_INDEX.
*******************************************************************************/

int PolyIsNull (PtPoly ppoly);
/*******************************************************************************
 Verifica se o polinómio ppoly é um polinómio nulo. Devolve 1 em caso afirmativo
 e 0 em caso contrário. Valores de erro: OK ou NO_POLY.
*******************************************************************************/

PtPoly PolySymmetrical (PtPoly ppoly);
/*******************************************************************************
 Cria o polinómio simétrico do polinómio ppoly, caso ele exista. Devolve a
 referência do polinómio simétrico ou NULL, caso não consiga fazer o simétrico.
 Valores de erro: OK, NO_POLY ou NO_MEM.
*******************************************************************************/

PtPoly PolyAddition (PtPoly ppoly1, PtPoly ppoly2);
/*******************************************************************************
 Soma os dois polinómios ppoly1 e ppoly2. Devolve a referência do polinómio soma
 ou NULL, caso não consiga fazer a adição. Valores de erro: OK, NO_POLY ou NO_MEM.
*******************************************************************************/

PtPoly PolySubtraction (PtPoly ppoly1, PtPoly ppoly2);
/*******************************************************************************
 Subtrai os dois polinómios ppoly1 e ppoly2. Devolve a referência do polinómio
 diferença ou NULL, caso não consiga fazer a subtracção. Valores de erro: OK,
 NO_POLY ou NO_MEM.
*******************************************************************************/

PtPoly PolyMultiplication (PtPoly ppoly1, PtPoly ppoly2);
/*******************************************************************************
 Multiplica os dois polinómios ppoly1 e ppoly2. Devolve a referência do polinómio
 produto ou NULL, caso não consiga fazer a multiplicação. Valores de erro: OK,
 NO_POLY ou NO_MEM.

 Polynomial multiplication returning a new polynomial. Error codes: OK, NO_POLY or NO_MEM.
*******************************************************************************/

int PolyEquals (PtPoly ppoly1, PtPoly ppoly2);
/*******************************************************************************
 Compara se os dois polinómios ppoly1 e ppoly2 são idênticos. Devolve 1 em caso
 afirmativo e 0 em caso contrário. Valores de erro: OK ou NO_POLY.
*******************************************************************************/

void PolyStoreFile (PtPoly ppoly, char *pfname);
/*******************************************************************************
 Armazena o polinómio ppoly, caso ele exista, no ficheiro pfname. O ficheiro tem
 na primeira linha o grau do polinómio, seguido dos coeficientes, um por linha.
 Valores de erro: OK, NO_POLY ou NO_FILE.
*******************************************************************************/

PtPoly PolyCreateFile (char *pfname);
/*******************************************************************************
 Cria um polinómio a partir de informacao lida do ficheiro pfname. Devolve a
 referência do polinómio criado ou NULL, caso não consiga criar o polinómio.
 Valores de erro: OK, NO_FILE ou NO_MEM.
*******************************************************************************/

double PolyEvaluation (PtPoly ppoly, double px);
/*******************************************************************************
 Devolve o valor do polinómio ppoly para o argumento px. Valores de erro:
 OK ou NO_POLY.
*******************************************************************************/

#endif
