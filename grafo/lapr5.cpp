#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>     
#include <GL\glut.h>
#include <AL\alut.h>
#include <iostream>

#include "grafos.h"
#include "mathlib.h"
#include "studio.h"
#include "mdlviewer.h"
#include "camera.h"
#include "ambient.h"
#include "modelo.h"
#include "estado.h"
using namespace std;

#define graus(X) (double)((X)*180/M_PI)
#define rad(X)   (double)((X)*M_PI/180)
//typedef	GLdouble Vector[4];

//variaveis de teste
GLint* n = new GLint[2];

Estado estado;
Modelo modelo;

void setProjection(int x, int y, GLboolean picking){
    glLoadIdentity();
	if (picking) { // se está no modo picking, lê viewport e define zona de picking
		GLint vport[4];
		glGetIntegerv(GL_VIEWPORT, vport);
		gluPickMatrix(x, glutGet(GLUT_WINDOW_HEIGHT)  - y, 4, 4, vport); // Inverte o y do rato para corresponder à jana
	}
	    
	gluPerspective(estado.camera.fov,(GLfloat)glutGet(GLUT_WINDOW_WIDTH) /glutGet(GLUT_WINDOW_HEIGHT) ,1,500);

}

void myReshape(int w, int h){	
	glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
	setProjection(0,0,GL_FALSE);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}

void initEstado(){
	estado.camera.dir_lat=M_PI/4;
	estado.camera.dir_long=-M_PI/4;
	estado.camera.fov=60;
	estado.camera.dist=100;
	estado.eixo[0]=0;
	estado.eixo[1]=0;
	estado.eixo[2]=0;
	estado.camera.center[0]=0;
	estado.camera.center[1]=0;
	estado.camera.center[2]=0;
	estado.light=GL_FALSE;
	estado.apresentaNormais=GL_FALSE;
	estado.lightViewer=1;
	estado.camera.velH = 1;
	estado.camera.velV = 1;
	estado.camera.velTotal = sqrt(estado.camera.velH*estado.camera.velH + estado.camera.velV*estado.camera.velV); //Root(velh(cube) + velv(cube)).
}

void initModelo(){
	modelo.escala=0.2;
	modelo.cor_cubo = brass;
	modelo.g_pos_luz1[0]=-5.0;
	modelo.g_pos_luz1[1]= 5.0;
	modelo.g_pos_luz1[2]= 5.0;
	modelo.g_pos_luz1[3]= 0.0;
	modelo.g_pos_luz2[0]= 5.0;
	modelo.g_pos_luz2[1]= -15.0;
	modelo.g_pos_luz2[2]= 5.0;
	modelo.g_pos_luz2[3]= 0.0;
}

void initMinimap(){
	//Nao desenha, codigo das aulas SGRAI

	glViewport(640-50,0,50,50);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-30,30, -30, 30);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	 
  //desenha bussola 2D
	glPushMatrix();
	glRotatef(graus(estado.camera.dir_long)-90,0,0,-1);
	glColor3f(1,0.4,0.4);
	
	glBegin(GL_TRIANGLES);
	glVertex2d(-12,0);
	glVertex2d(0,12);
	glVertex2d(12,0);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2d(-12,0);
	glVertex2d(0,-12);
	glVertex2d(12,0);
	glEnd();
	glPopMatrix();

	myReshape(640, 480);
}

void InitAudio()
{
	estado.buffer = alutCreateBufferFromFile("C:\\Users\\i100580\\Documents\\Visual Studio 2010\\Projects\\Graphs4Social\\LAPR5\\SilenceAndMotion.wav");
	alGenSources(1, &estado.source);
	alSourcei(estado.source, AL_BUFFER, estado.buffer);
	estado.tecla_s = AL_FALSE;
	alSourcePlay(estado.source);
}
void myInit()
{

	GLfloat LuzAmbiente[]={0.5,0.5,0.5, 0.0};

	glClearColor (0.0, 0.0, 0.0, 0.0);

	glEnable(GL_SMOOTH); /*enable smooth shading */
	glEnable(GL_LIGHTING); /* enable lighting */
	glEnable(GL_DEPTH_TEST); /* enable z buffer */
	glEnable(GL_NORMALIZE);

	glDepthFunc(GL_LESS);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente); 
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, estado.lightViewer); 
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); 

	initModelo();
	initEstado();
	initMinimap();

	modelo.quad=gluNewQuadric();
	gluQuadricDrawStyle(modelo.quad, GLU_FILL);
	gluQuadricNormals(modelo.quad, GLU_OUTSIDE);

	leGrafo();
}

void imprime_ajuda(void)
{
  printf("\n\nDesenho de um labirinto a partir de um grafo\n");
  printf("h,H - Ajuda \n");
  printf("i,I - Reset dos Valores \n");
  printf("******* Diversos ******* \n");
  printf("l,L - Alterna o calculo luz entre Z e eye (GL_LIGHT_MODEL_LOCAL_VIEWER)\n");
  printf("k,K - Alerna luz de camera com luz global \n");
  printf("s,S - PolygonMode Fill \n");
  printf("w,W - PolygonMode Wireframe \n");
  printf("p,P - PolygonMode Point \n");
  printf("c,C - Liga/Desliga Cull Face \n");
  printf("n,N - Liga/Desliga apresentação das normais \n");
  printf("******* grafos ******* \n");
  printf("F1  - Grava grafo do ficheiro \n");
  printf("F2  - Lê grafo para ficheiro \n");
  printf("F6  - Cria novo grafo\n");
  printf("******* Camera ******* \n");
  printf("Botão esquerdo - Arrastar os eixos (centro da camera)\n");
  printf("Botão direito  - Rodar camera\n");
  printf("Botão direito com CTRL - Zoom-in/out\n");
  printf("PAGE_UP, PAGE_DOWN - Altera distância da camara \n");
  printf("ESC - Sair\n");
}


void material(enum tipo_material mat)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient[mat]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse[mat]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular[mat]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess[mat]);
}

const GLfloat red_light[] = {1.0, 0.0, 0.0, 1.0};
const GLfloat green_light[] = {0.0, 1.0, 0.0, 1.0};
const GLfloat blue_light[] = {0.0, 0.0, 1.0, 1.0};
const GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};


void putLights(GLfloat* diffuse)
{
	const GLfloat white_amb[] = {0.2, 0.2, 0.2, 1.0};

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white_amb);
	glLightfv(GL_LIGHT0, GL_POSITION, modelo.g_pos_luz1);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT1, GL_AMBIENT, white_amb);
	glLightfv(GL_LIGHT1, GL_POSITION, modelo.g_pos_luz2);

	/* desenhar luz */
	material(red_plastic);
	glPushMatrix();
		glTranslatef(modelo.g_pos_luz1[0], modelo.g_pos_luz1[1], modelo.g_pos_luz1[2]);
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 1.0);
		glutSolidCube(0.1);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(modelo.g_pos_luz2[0], modelo.g_pos_luz2[1], modelo.g_pos_luz2[2]);
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 1.0);
		glutSolidCube(0.1);
		glEnable(GL_LIGHTING);
	glPopMatrix();

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}

void desenhaSolo(){
}

void CrossProduct (GLdouble v1[], GLdouble v2[], GLdouble cross[])
{
	cross[0] = v1[1]*v2[2] - v1[2]*v2[1];
	cross[1] = v1[2]*v2[0] - v1[0]*v2[2];
	cross[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

GLdouble VectorNormalize (GLdouble v[])
{
	int		i;
	GLdouble	length;

	if ( fabs(v[1] - 0.000215956) < 0.0001)
		i=1;

	length = 0;
	for (i=0 ; i< 3 ; i++)
		length += v[i]*v[i];
	length = sqrt (length);
	if (length == 0)
		return 0;
		
	for (i=0 ; i< 3 ; i++)
		v[i] /= length;	

	return length;
}

void desenhaNormal(GLdouble x, GLdouble y, GLdouble z, GLdouble normal[], tipo_material mat){

	switch (mat){
		case red_plastic:
				glColor3f(1,0,0);
			break;
		case azul:
				glColor3f(0,0,1);
			break;
		case emerald:
				glColor3f(0,1,0);
			break;
		default:
				glColor3f(1,1,0);
	}
	glDisable(GL_LIGHTING);
	glPushMatrix();
		glTranslated(x,y,z);
		glScaled(0.4,0.4,0.4);
		glBegin(GL_LINES);
			glVertex3d(0,0,0);
			glVertex3dv(normal);
		glEnd();
		glPopMatrix();
	glEnable(GL_LIGHTING);
}


void desenhaChao(GLfloat xi, GLfloat yi, GLfloat zi, GLfloat xf, GLfloat yf, GLfloat zf, int orient){
	GLdouble v1[3],v2[3],cross[3];
	GLdouble length;
	v1[0]=xf-xi;
	v1[1]=0;
	v2[0]=0;
	v2[1]=yf-yi;

	switch(orient) {
		case NORTE_SUL:
				glBegin(GL_QUADS);
					//glNormal3dv(cross);
					glVertex3f(xi,yi,zi);
					glVertex3f(xf,yi,zi);
					glVertex3f(xf,yf,zf);
					glVertex3f(xi,yf,zf);
				glEnd();
			break;
		case ESTE_OESTE :
				glBegin(GL_QUADS);
					//glNormal3dv(cross);
					glVertex3f(xi,yi,zi);
					glVertex3f(xf,yi,zf);
					glVertex3f(xf,yf,zf);
					glVertex3f(xi,yf,zi);
				glEnd();
			break;
		default:
			//Desenha Cilindros (Nós) se regular slate se premium brass
			material(slate);
				glPushMatrix();
					glTranslatef(xi,yi,zi);
					gluCylinder(modelo.quad,1,1,0.4,100,100);
					gluDisk(modelo.quad, 0.0f, 1, 50, 1);
					glTranslatef(0.0f, 0.0f, 0.4);
					gluDisk(modelo.quad, 0.0f, 1, 100, 1);
				glPopMatrix();
			break;
	}
}

void desenhaNo(int no){
	switch(nos[no].estado){
		case 0:
			material(slate);
			break;
		case 1:
			material(brass);
			break;
		case 2:
			material(azul);
			break;
		default:
			break;
	}
	glPushMatrix();
		glTranslatef(nos[no].x,nos[no].y,nos[no].z);
		//Plateau
		gluCylinder(modelo.quad,nos[no].largura/2.0,nos[no].largura/2.0,ALTURA_NO,100,100);
		gluDisk(modelo.quad, 0.0f, nos[no].largura/2.0, 100, 1);
		glTranslatef(0.0f, 0.0f, ALTURA_NO);
		gluDisk(modelo.quad, 0.0f, nos[no].largura/2.0, 100, 1);
	glPopMatrix();
}

void desenhaLig(Arco arco, No* noi, No* nof,int orienta)
{
	/*NAO IMPLEMENTADO*/
}

void desenhaPoligono(GLfloat a[], GLfloat b[], GLfloat c[], GLfloat  d[], GLfloat normal[])
{
    glBegin(GL_POLYGON);
        glNormal3fv(normal);
        glVertex3fv(a);
        glVertex3fv(b);
        glVertex3fv(c);
        glVertex3fv(d);
    glEnd();
}

void desenhaCubo()
{
  GLfloat vertices[][3] = { {-0.5,-0.5,-0.5}, 
                            {0.5,-0.5,-0.5}, 
                            {0.5,0.5,-0.5}, 
                            {-0.5,0.5,-0.5}, 
                            {-0.5,-0.5,0.5},  
                            {0.5,-0.5,0.5}, 
                            {0.5,0.5,0.5}, 
                            {-0.5,0.5,0.5}};
  GLfloat normais[][3] = {  {0,0,-1}, 
                            // acrescentar as outras normais...
                         };

  desenhaPoligono(vertices[1],vertices[0],vertices[3],vertices[2],normais[0]);
  desenhaPoligono(vertices[2],vertices[3],vertices[7],vertices[6],normais[0]);
  desenhaPoligono(vertices[3],vertices[0],vertices[4],vertices[7],normais[0]);
  desenhaPoligono(vertices[6],vertices[5],vertices[1],vertices[2],normais[0]);
  desenhaPoligono(vertices[4],vertices[5],vertices[6],vertices[7],normais[0]);
  desenhaPoligono(vertices[5],vertices[4],vertices[0],vertices[1],normais[0]);
}

void desenhaArco(Arco arco){
	material(carbon);
	glPushMatrix();
		glTranslatef(nos[arco.ni].x, nos[arco.ni].y, nos[arco.ni].z);
		glRotatef(graus(arco.orientacao),0,0,1);
		glRotatef(graus(M_PI/2.0 - arco.inclinacao),0,1,0);
		/*Paralelipipedo
		glPushMatrix();
			glScalef(2,1,1);
			desenhaCubo();
		glPopMatrix();*/
		//Cilindro, ideal mas a largura nas ligações fica mal
		gluCylinder(modelo.quad,arco.largura/4.0,arco.largura/4.0,arco.comprimento,100,100);
	glPopMatrix();
}

void desenhaHomer(float x, float y, float z){
	glTranslatef(x,y,z+ALT_HOMER/2.0);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
			glScaled(HOMER_SCALE,HOMER_SCALE,HOMER_SCALE);
			glEnable(GL_COLOR_MATERIAL);
			glEnable(GL_TEXTURE_2D);
			mdlviewer_display(modelo.homer);
		glPopMatrix();
	glPopAttrib();
}

void desenhaLabirinto(){
	glPushMatrix();
		glTranslatef(0,0,0.05);
		glScalef(5,5,5);
		for(int i=0; i<numNos; i++){
			desenhaNo(i);
		}
		material(emerald);
		for(int i=0; i<numArcos; i++){
			//imprimeArco(arcos[i]);
			desenhaArco(arcos[i]);
		}
	glPopMatrix();
}

void desenhaEixo(){
	gluCylinder(modelo.quad,0.5,0.5,20,16,15);
	glPushMatrix();
		glTranslatef(0,0,20);
		glPushMatrix();
			glRotatef(180,0,1,0);
			gluDisk(modelo.quad,0.5,2,16,6);
		glPopMatrix();
		gluCylinder(modelo.quad,2,0,5,16,15);
	glPopMatrix();
}

#define EIXO_X		1
#define EIXO_Y		2
#define EIXO_Z		3

void desenhaPlanoDrag(int eixo){
	glPushMatrix();
		glTranslated(estado.eixo[0],estado.eixo[1],estado.eixo[2]);
		switch (eixo) {
			case EIXO_Y :
					if(abs(estado.camera.dir_lat)<M_PI/4)
						glRotatef(-90,0,0,1);
					else
						glRotatef(90,1,0,0);
					material(red_plastic);
				break;
			case EIXO_X :
					if(abs(estado.camera.dir_lat)>M_PI/6)
						glRotatef(90,1,0,0);
					material(azul);
				break;
			case EIXO_Z :
					if(abs(cos(estado.camera.dir_long))>0.5)
						glRotatef(90,0,0,1);

					material(emerald);
				break;
		}
		glBegin(GL_QUADS);
			glNormal3f(0,1,0);
			glVertex3f(-100,0,-100);
			glVertex3f(100,0,-100);
			glVertex3f(100,0,100);
			glVertex3f(-100,0,100);
		glEnd();
	glPopMatrix();
}

void desenhaEixos(){

	glPushMatrix();
		glTranslated(estado.eixo[0],estado.eixo[1],estado.eixo[2]);
		material(emerald);
		glPushName(EIXO_Z);
			desenhaEixo();
		glPopName();
		glPushName(EIXO_Y);
			glPushMatrix();
				glRotatef(-90,1,0,0);
				material(red_plastic);
				desenhaEixo();
			glPopMatrix();
		glPopName();
		glPushName(EIXO_X);
			glPushMatrix();
				glRotatef(90,0,1,0);
				material(azul);
				desenhaEixo();
			glPopMatrix();
		glPopName();
	glPopMatrix();
}

void setCamera(){

	Vertice eye;
	eye[0]=estado.camera.center[0]+estado.camera.dist*cos(estado.camera.dir_long)*cos(estado.camera.dir_lat);
	eye[1]=estado.camera.center[1]+estado.camera.dist*sin(estado.camera.dir_long)*cos(estado.camera.dir_lat);
	eye[2]=estado.camera.center[2]+estado.camera.dist*sin(estado.camera.dir_lat);

	if(estado.light){
		gluLookAt(eye[0],eye[1],eye[2],estado.camera.center[0],estado.camera.center[1],estado.camera.center[2],0,0,1);
		putLights((GLfloat*)white_light);
	}else{
		putLights((GLfloat*)white_light);
		gluLookAt(eye[0],eye[1],eye[2],estado.camera.center[0],estado.camera.center[1],estado.camera.center[2],0,0,1);
	}
}

void desenhaCamera(){
	glPushMatrix();
	material(brass);
	glTranslatef(estado.camera.center[0],estado.camera.center[1],estado.camera.center[2]);
	glScalef(2,2,2);
	desenhaCubo();
	glPopMatrix();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	setCamera();

	material(slate);
	desenhaSolo();

	//desenhaEixos();
	
	desenhaLabirinto();

	desenhaCamera();

	desenhaHomer(nos[0].x, nos[0].y, nos[0].z);

	if(estado.eixoTranslaccao) {
		// desenha plano de translacção
		cout << "Translate... " << estado.eixoTranslaccao << endl; 
		desenhaPlanoDrag(estado.eixoTranslaccao);

	}

	glFlush();
	glutSwapBuffers();

}



void keyboard(unsigned char key, int x, int y)
{

    switch (key)
    {
		case 27 :
				exit(0);
			break;
		case 'h':
		case 'H':
				imprime_ajuda();
			break;
		case 'l':
		case 'L':
				if(estado.lightViewer)
					estado.lightViewer=0;
				else
					estado.lightViewer=1;
				glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, estado.lightViewer);
				glutPostRedisplay();
			break;
		case 'k':
		case 'K':
				estado.light=!estado.light;
				glutPostRedisplay();
			break;
		case 'w':
		case 'W':
				glDisable(GL_LIGHTING);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glutPostRedisplay();
			break;
		case 'p':
		case 'P':
				glDisable(GL_LIGHTING);
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				glutPostRedisplay();
			break;
		case 's':
		case 'S':
				glEnable(GL_LIGHTING);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glutPostRedisplay();
			break;
		case 'c':
		case 'C':
				if(glIsEnabled(GL_CULL_FACE))
					glDisable(GL_CULL_FACE);
				else
					glEnable(GL_CULL_FACE);
				glutPostRedisplay();
			break;    
		case 'n':
		case 'N':
				estado.apresentaNormais=!estado.apresentaNormais;
				glutPostRedisplay();
			break;    		
		case 'i':
		case 'I':
				initEstado();
				initModelo();
				glutPostRedisplay();
			break;
		case 'v' :
		case 'V' :
				estado.tecla_s = AL_TRUE;
			break;
		case 'g' :
		case 'G' :
				estado.tecla_s = AL_FALSE;
			break;
	}
}

double square2(double n){
	return n*n;
}

float distanciaCameraNo(No no){
	double x = square2((no.x-estado.camera.center[0]));
	double y = square2((no.y-estado.camera.center[1]));
	double z = square2((no.z-estado.camera.center[2]));
	double xyz = x+y+z;
	cout << "sqrt() " << xyz << endl;
	return sqrt(xyz);
}

bool cameraColide(){
	for(int i = 0; i < numNos; i++){
		//Nao implementado
	}
	return false;
}

void moveCamera(string dir){
	if(!cameraColide()){
		if(dir.compare("DOWN")){
			estado.camera.center[0] = estado.camera.center[0] - K_CAMERA * cos(estado.camera.dir_long);
			estado.camera.center[1] = estado.camera.center[1] - K_CAMERA * sin(estado.camera.dir_long);
		}else if(dir.compare("UP")){
			estado.camera.center[0] = estado.camera.center[0] + K_CAMERA * cos(estado.camera.dir_long);
			estado.camera.center[1] = estado.camera.center[1] + K_CAMERA * sin(estado.camera.dir_long);
		}
	}
}

void iluminaCaminho(GLint n[], int nElem){
	for(int i = 0; i < nElem; i++){
		int no = n[i];
		nos[no].estado = 2;
	}

	glutPostRedisplay();
}

void Special(int key, int x, int y){
	switch(key){
		case GLUT_KEY_F1 :
				gravaGrafo();
			break;
		case GLUT_KEY_F2 :
				leGrafo();
				glutPostRedisplay();
			break;	

		case GLUT_KEY_F3 :
				n[0] = 1;
				n[1] = 2;
				iluminaCaminho(n,2);
			break;
		case GLUT_KEY_F6 :
				numNos=numArcos=0;
				addNo(criaNo( 0, 10,0));  // 0
				addNo(criaNo( 0,  5,0));  // 1
				addNo(criaNo(-5,  5,0));  // 2
				addNo(criaNo( 5,  5,0));  // 3
				addNo(criaNo(-5,  0,0));  // 4
				addNo(criaNo( 5,  0,0));  // 5
				addNo(criaNo(-5, -5,0));  // 6
				addArco(criaArco(0,1,1,1));  // 0 - 1
				addArco(criaArco(1,2,1,1));  // 1 - 2
				addArco(criaArco(1,3,1,1));  // 1 - 3
				addArco(criaArco(2,4,1,1));  // 2 - 4
				addArco(criaArco(3,5,1,1));  // 3 - 5
				addArco(criaArco(4,5,1,1));  // 4 - 5
				addArco(criaArco(4,6,1,1));  // 4 - 6
				glutPostRedisplay();
			break;	
		case GLUT_KEY_UP:
				moveCamera("UP");
				glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:
				estado.camera.dir_long-=0.1;
				glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
				estado.camera.dir_long+=0.1;
				glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
				moveCamera("DOWN");
				glutPostRedisplay();
			break;	}
}

void motionRotate(int x, int y){
#define DRAG_SCALE	0.01
	double lim=M_PI/2-0.1;
	estado.camera.dir_long+=(estado.xMouse-x)*DRAG_SCALE;
	estado.camera.dir_lat-=(estado.yMouse-y)*DRAG_SCALE*0.5;
	if(estado.camera.dir_lat>lim)
		estado.camera.dir_lat=lim;
	else 
		if(estado.camera.dir_lat<-lim)
			estado.camera.dir_lat=-lim;
	estado.xMouse=x;
	estado.yMouse=y;
	glutPostRedisplay();
}

void motionZoom(int x, int y){
#define ZOOM_SCALE	0.5
	estado.camera.dist-=(estado.yMouse-y)*ZOOM_SCALE;
	if(estado.camera.dist<5)
		estado.camera.dist=5;
	else 
		if(estado.camera.dist>200)
			estado.camera.dist=200;
	estado.yMouse=y;
	glutPostRedisplay();
}

void motionDrag(int x, int y){
	GLuint buffer[100];
	GLint vp[4];
	GLdouble proj[16], mv[16];
	int n;
	GLdouble newx, newy, newz;

	glSelectBuffer(100, buffer);
	glRenderMode(GL_SELECT);
	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // guarda a projecção
		glLoadIdentity();
		setProjection(x,y,GL_TRUE);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setCamera();
	desenhaPlanoDrag(estado.eixoTranslaccao);
	
	n = glRenderMode(GL_RENDER);
	if (n > 0) {
		glGetIntegerv(GL_VIEWPORT, vp);
		glGetDoublev(GL_PROJECTION_MATRIX, proj);
		glGetDoublev(GL_MODELVIEW_MATRIX, mv);
		gluUnProject(x, glutGet(GLUT_WINDOW_HEIGHT) - y, (double) buffer[2] / UINT_MAX, mv, proj, vp, &newx, &newy, &newz);
		printf("Novo x:%lf, y:%lf, z:%lf\n\n", newx, newy, newz);
		switch (estado.eixoTranslaccao) {
			case EIXO_X :
					estado.eixo[0]=newx;
					//estado.eixo[1]=newy;
				break;
			case EIXO_Y :
					estado.eixo[1]=newy;
					//estado.eixo[2]=newz;
				break;
			case EIXO_Z :
					//estado.eixo[0]=newx;
					estado.eixo[2]=newz;
				break;		
		}
		glutPostRedisplay();
	}


	glMatrixMode(GL_PROJECTION); //repõe matriz projecção
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

int picking(int x, int y){
	int i, n, objid=0;
	double zmin = 10.0;
	GLuint buffer[100], *ptr;

	glSelectBuffer(100, buffer);
	glRenderMode(GL_SELECT);
	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // guarda a projecção
		glLoadIdentity();
		setProjection(x,y,GL_TRUE);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setCamera();
	desenhaEixos();
	
	n = glRenderMode(GL_RENDER);
	if (n > 0)
	{
		ptr = buffer;
		for (i = 0; i < n; i++)
		{
			if (zmin > (double) ptr[1] / UINT_MAX) {
				zmin = (double) ptr[1] / UINT_MAX;
				objid = ptr[3];
			}
			ptr += 3 + ptr[0]; // ptr[0] contem o número de nomes (normalmente 1); 3 corresponde a numnomes, zmin e zmax
		}
	}


	glMatrixMode(GL_PROJECTION); //repõe matriz projecção
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	return objid;
}

void mouse(int btn, int state, int x, int y){
	switch(btn) {
		case GLUT_RIGHT_BUTTON :
					if(state == GLUT_DOWN){
						estado.xMouse=x;
						estado.yMouse=y;
						if(glutGetModifiers() & GLUT_ACTIVE_CTRL)
							glutMotionFunc(motionZoom);
						else
							glutMotionFunc(motionRotate);
						cout << "Left down\n";
					}
					else{
						glutMotionFunc(NULL);
						cout << "Left up\n";
					}
				break;
		case GLUT_LEFT_BUTTON :
					if(state == GLUT_DOWN){
						estado.eixoTranslaccao=picking(x,y);
						if(estado.eixoTranslaccao)
							glutMotionFunc(motionDrag);
						cout << "Right down - objecto:" << estado.eixoTranslaccao << endl;
					}
					else{
						if(estado.eixoTranslaccao!=0) {
							estado.camera.center[0]=estado.eixo[0];
							estado.camera.center[1]=estado.eixo[1];
							estado.camera.center[2]=estado.eixo[2];
							glutMotionFunc(NULL);
							estado.eixoTranslaccao=0;
							glutPostRedisplay();
						}
						cout << "Right up\n";
					}
				break;
	}
}

void main(int argc, char **argv)
{
    glutInit(&argc, argv);
	alutInit (&argc, argv);
	InitAudio();
/* need both double buffering and z buffer */

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Graphs4Social");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(Special);
	glutMouseFunc(mouse);

	mdlviewer_init("homer.mdl", modelo.homer);
	myInit();

	imprime_ajuda();

    glutMainLoop();
}