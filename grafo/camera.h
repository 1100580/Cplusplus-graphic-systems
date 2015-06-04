typedef	GLdouble Vertice[3];

#define DIMENSAO_CAMARA	1 //voo livre
#define DISTANCIA_SOLO	3 //voo rasante
#define K_CAMERA		1

typedef struct Camera{
	GLfloat x,y,z;
	GLfloat dir;//Radianos
	GLfloat velH; //??
	GLfloat velV; //??/voo livre
	GLfloat velTotal; //Root(velh(cube) + velv(cube)).

	GLfloat fov;
	GLdouble dir_lat;
	GLdouble dir_long;
	GLfloat dist;
	Vertice center;
}Camera;