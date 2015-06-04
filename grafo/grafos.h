#ifndef _GRAFO_INCLUDE
#define _GRAFO_INCLUDE

#define _MAX_NOS_GRAFO 100
#define _MAX_ARCOS_GRAFO 200

#define NORTE_SUL	0
#define ESTE_OESTE	1
#define PLANO		2
#define COTAMAX     100
#define COTAMIN		0
#define ALTURA_NO	1
#define ARCO_BASE	0.2

typedef struct No{
	float x,y,z,largura;
	int estado;/*0=normal,1=premium,2=light*/;
}No;

typedef struct Arco{
	int ni,nf;
	float peso,desnivel,largura,comprimento,orientacao,inclinacao;
}Arco;

typedef struct Ligacao{
	float comprimento;
	int no;
}Ligacao;

extern No nos[];
extern Arco arcos[];
extern Ligacao ligacoes[];
extern int numNos, numArcos,numLigacoes;

void addNo(No);
void deleteNo(int);
void imprimeNo(No);
void listNos();
No criaNo(float, float, float);

void addArco(Arco);
void deleteArco(int);
void imprimeArco(Arco);
void listArcos();
Arco criaArco(int, int, float, float);

void gravaGrafo();
void leGrafo();

void resolveOtherArguments(int i);
float squareit(float);
#endif