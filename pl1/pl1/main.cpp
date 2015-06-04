#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.1415926
#endif
#define RAD(a) ((a)*M_PI/180.0)

typedef struct{
	float x,y;
} ponto;

typedef struct{
	float raio;
	ponto origem;
}circ;

int main(void){
	float n;
	int i;
	ponto p0,p1;
	printf("n?");
	scanf("%f", &n);

	printf("p0-x?");
	scanf("%f", &p0.x);
	printf("p0-y?");
	scanf("%f", &p0.y);

	printf("p1-x?");
	scanf("%f", &p1.x);
	printf("p1-y?");
	scanf("%f", &p1.y);

	ponto medio;

	medio.x= (p0.x+p1.x)/2.0;
	medio.y= (p0.y+p1.y)/2.0;
	printf("Ponto medio: x=%f y=%f\n", medio.x, medio.y);

	float dist;

	dist=sqrt(pow(p0.x-p1.x,2) + pow(p0.y-p1.y,2));

	printf("distancia: %f\n", dist);

	circ c;
	c.origem = medio;
	c.raio = dist/2.0;

	float num = 360.0/n;
	float var = 0.0;
	for(i=0; i<n; i++){
		float xv= RAD(var);
		ponto p;
		p.x= c.raio*cos(xv)+c.origem.x;
		p.y= c.raio*sin(xv)+c.origem.y;
		var = var+num;
		printf("P%d x=%f y=%f\n",i+1,p.x,p.y); 
	}
	printf("END\n");

	system("PAUSE");
	return 0;
}