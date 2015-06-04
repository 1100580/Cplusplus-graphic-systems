#define _USE_MATH_DEFINES
#include "grafos.h"
#include <math.h>
#include <iostream>
#include <fstream>


#define __GRAFO__FILE__ "exemplo.grafo"
#define GRAUS(X) (double)((X)*180/M_PI)
#define RAD(X)   (double)((X)*M_PI/180)

No nos[_MAX_NOS_GRAFO];
Arco arcos[_MAX_ARCOS_GRAFO];
Ligacao ligacoes[_MAX_ARCOS_GRAFO*2];
int numNos=0, numArcos=0, numLigacoes=0;

using namespace std;

void addNo(No no){
	if(numNos<_MAX_NOS_GRAFO){
		nos[numNos]=no;
		numNos++;
	}else{
		cout << "Número de nós chegou ao limite" << endl;
	}
}

void deleteNo(int indNo){
	if(indNo>=0 && indNo<numNos){
		for(int i=indNo; i<numNos; nos[i++]=nos[i+i]);
		numNos--;
	}else{
		cout << "Indíce de nó inválido" << endl;
	}
}

void imprimeNo(No no){
	cout << "X:" << no.x << "Y:" << no.y << "Z:" << no.z <<endl;
}

void listNos(){
	for(int i=0; i<numNos; imprimeNo(nos[i++]));
}

No criaNo(float x, float y, float z){
	No no;
	no.x=x;
	no.y=y;
	no.z=z;
	no.largura = 0.0;
	return no;
}

void addArco(Arco arco){
	if(numArcos<_MAX_ARCOS_GRAFO){
		arcos[numArcos]=arco;
		numArcos++;
		
		if(nos[arco.ni].largura < arco.largura)
			nos[arco.ni].largura = arco.largura;
		if(nos[arco.nf].largura < arco.largura)
			nos[arco.nf].largura = arco.largura;
	}else{
		cout << "Número de arcos chegou ao limite" << endl;
	}
}

void deleteArco(int indArco){
	if(indArco>=0 && indArco<numArcos){
		for(int i=indArco; i<numArcos; arcos[i++]=arcos[i+i]);
		numArcos--;
	}else{
		cout << "Indíce de arco inválido" << endl;
	}
}

void imprimeArco(Arco arco){
	cout << "No início(x,y):(" << nos[arco.ni].x << "," << nos[arco.ni].y << ") " << "No final(x,y):(" << nos[arco.nf].x << "," << nos[arco.nf].y << ") " << "Peso:" << arco.peso << "Largura:" << arco.largura << endl <<endl;
}

void listArcos(){
	for(int i=0; i<numArcos; imprimeArco(arcos[i++]));
}

Arco criaArco(int noi, int nof, float peso, float largura){
	Arco arco;
	arco.ni=noi;
	arco.nf=nof;
	arco.peso=peso;
	arco.largura=largura;
	return arco;
}

void gravaGrafo(){
	ofstream myfile;

	myfile.open (__GRAFO__FILE__, ios::out);
	if (!myfile.is_open()) {
		cout << "Erro ao abrir " << __GRAFO__FILE__ << "para escrever" <<endl;
		exit(1);
	}
	myfile << numNos << endl;
	for(int i=0; i<numNos;i++)
		myfile << nos[i].x << " " << nos[i].y << " " << nos[i].z <<endl;
	myfile << numArcos << endl;
	for(int i=0; i<numArcos;i++)
		myfile << arcos[i].ni << " " << arcos[i].nf << " " << arcos[i].peso << " " << arcos[i].largura << endl;
	myfile.close();
}

void leGrafo(){
	ifstream myfile;

	myfile.open (__GRAFO__FILE__, ios::in);
	if (!myfile.is_open()) {
		cout << "Erro ao abrir " << __GRAFO__FILE__ << "para ler" <<endl;
		exit(1);
	}
	myfile >> numNos;
	for(int i=0; i<numNos;i++){
		myfile >> nos[i].x >> nos[i].y >> nos[i].z;
		nos[i].estado=0;//mudar para os premium
	}
	myfile >> numArcos ;

	//inclui ligacoes nos arcos
	numLigacoes=0;
	for(int i=0; i<numArcos;i++){
		if(numLigacoes <= _MAX_ARCOS_GRAFO*2){
			arcos[i].ni=numLigacoes++;
			arcos[i].nf=numLigacoes++;
		}
		myfile >> arcos[i].ni >>arcos[i].nf >> arcos[i].peso >> arcos[i].largura ;
	}
	myfile.close();

	// calcula a cota e largura de cada no = maior largura dos arcos que divergem/convergem desse/nesse no + nº de tags
	for(int i=0; i<numNos;i++){
		nos[i].largura=0;
		nos[i].z=0;
		for(int j=0; j<numArcos; j++){
			if ((arcos[j].ni == i || arcos[j].nf == i)){
				nos[i].z++;
				if(nos[i].largura < arcos[j].largura){
					nos[i].largura = arcos[j].largura;
				}
			}
		}
	}

	for(int i=0; i<numArcos;i++){
		resolveOtherArguments(i);
	}
}

float squareIt(float n)
{
	return n*n;
}

void resolveOtherArguments(int i)
{
	arcos[i].desnivel = nos[arcos[i].nf].z - nos[arcos[i].ni].z + ALTURA_NO/3.0;
	float COXY = sqrt(squareIt(nos[arcos[i].nf].x - nos[arcos[i].ni].x) + squareIt(nos[arcos[i].ni].y - nos[arcos[i].nf].y));
	arcos[i].comprimento = sqrt(squareIt(COXY) + squareIt(arcos[i].desnivel));
	arcos[i].orientacao = atan2((nos[arcos[i].nf].y - nos[arcos[i].ni].y), (nos[arcos[i].nf].x - nos[arcos[i].ni].x));//RAD
	arcos[i].inclinacao = atan2(arcos[i].desnivel, COXY);//RAD
}