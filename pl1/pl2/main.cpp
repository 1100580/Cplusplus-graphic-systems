#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define DEBUG 1

/* VARIAVEIS GLOBAIS */

typedef struct {
  GLboolean   doubleBuffer;
  GLint       delay;
}Estado;

typedef struct {
  GLint       hor,min,seg;
}Horas;

typedef struct {
  GLint       numLados;
  GLfloat     raio;
  GLfloat     tamLado;
  GLenum      tipoPoligono;
  Horas       hora;
}Modelo;


Estado estado;
Modelo modelo;




/* Inicialização do ambiente OPENGL */
void Init(void)
{

  struct tm *current_time;
  time_t timer = time(0);

  //delay para o timer
  estado.delay=10;

  modelo.tamLado=1;
  modelo.numLados=32;
  modelo.raio=0.5;
  modelo.tipoPoligono=GL_TRIANGLE_FAN;

  
  // Lê hora do Sistema
  current_time = localtime(&timer);
  modelo.hora.hor = current_time->tm_hour;
  modelo.hora.min = current_time->tm_min;
  modelo.hora.seg = current_time->tm_sec;

  glClearColor(0.0, 0.0, 0.0, 0.0);

  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);

}

/**************************************
***  callbacks de janela/desenho    ***
**************************************/

// CALLBACK PARA REDIMENSIONAR JANELA

void Reshape(int width, int height)
{
  GLint size;

  if (width < height)
    size = width;
  else
    size = height;

  // glViewport(botom, left, width, height)
  // define parte da janela a ser utilizada pelo OpenGL

  glViewport(0, 0, (GLint) size, (GLint) size);
  

  // Matriz Projecção
  // Matriz onde se define como o mundo e apresentado na janela
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // gluOrtho2D(left,right,bottom,top); 
  // projecção ortogonal 2D, com profundidade (Z) entre -1 e 1
  gluOrtho2D(-1, 1, -1, 1);

  // Matriz Modelview
  // Matriz onde são realizadas as tranformacões dos modelos desenhados
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


// ... definicao das rotinas auxiliares de desenho ...

void mostrador(GLfloat tamanho, GLint numLados)
{
  GLfloat cores[3][3]={{1.0f, 0.0f, 0.0f},{0.0f, 1.0f, 0.0f},{0.0f, 0.0f, 1.0f}};
  GLfloat ang=2*M_PI/numLados;
  GLint i;

  glLineWidth(1*tamanho);

  if(modelo.tipoPoligono==GL_TRIANGLE_FAN)
  {
    glBegin(GL_TRIANGLE_FAN);
      glColor3f(0.0f, 0.0f, 0.0f);
      glVertex2f(0,0);
      for(i=0;i<=numLados;i++)
      {
        glColor3fv(cores[i%3]);
        glVertex2f(tamanho*cos(i*ang),tamanho*sin(i*ang));
      }
    glEnd();
  }
  else
  {
    glBegin(modelo.tipoPoligono);
    for(i=0;i<numLados;i++)
    {
      glColor3fv(cores[i%3]);
      glVertex2f(tamanho*cos(i*ang),tamanho*sin(i*ang));
    }
  glEnd();
  }

  ang=2*M_PI/60;
  glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    for(i=0;i<60;i++)
    {
      glVertex2f(tamanho*cos(ang*i),tamanho*sin(ang*i));
      if(i%5)
        glVertex2f(0.95*tamanho*cos(ang*i),0.95*tamanho*sin(ang*i));
      else
        glVertex2f(0.8*tamanho*cos(ang*i),0.8*tamanho*sin(ang*i));
    }
  glEnd();


}

void ponteiros(GLfloat tamanho)
{
  GLfloat ang;
  
  glLineWidth(1*tamanho);
  glBegin(GL_LINES);
    ang=modelo.hora.seg*2*M_PI/60;
    glColor3f(1.0f, 1.0f, 0.0f);

    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.95 * tamanho * cos(M_PI/2-ang),0.95 * tamanho *sin(M_PI/2-ang));
  glEnd();

  glLineWidth(2*tamanho);
  glBegin(GL_LINES);
    ang=modelo.hora.min*2*M_PI/60;

    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.8 * tamanho * cos(M_PI/2-ang),0.8 * tamanho *sin(M_PI/2-ang));
  glEnd();


  glLineWidth(3*tamanho);
  glBegin(GL_LINES);
    ang=modelo.hora.hor*2*M_PI/12;

    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.6 * tamanho * cos(M_PI/2-ang),0.6 * tamanho *sin(M_PI/2-ang));
  glEnd();

  glPointSize(10*tamanho);
  glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
  glEnd();


}


// Callback de desenho

void Draw(void)
{

  glClear(GL_COLOR_BUFFER_BIT);
// ... chamada das rotinas auxiliares de desenho ...

  mostrador(modelo.raio,modelo.numLados);

  ponteiros(modelo.raio);

  glFlush();
  if (estado.doubleBuffer)
    glutSwapBuffers();
}

/*******************************
***   callbacks timer/idle   ***
*******************************/

// Callback Idle

void Idle(void)
{

  // accoes a tomar quando o glut está idle 

  // redesenhar o ecra 
  // glutPostRedisplay();
}

// Callback de temporizador

void Timer(int value)
{
  glutTimerFunc(estado.delay, Timer, 0);
  // ... accoes do temporizador ... 

  modelo.hora.seg++;
  if(modelo.hora.seg>59)
  {
    modelo.hora.seg=0;
    modelo.hora.min++;
    if(modelo.hora.min>59)
    {
      modelo.hora.min=0;
      modelo.hora.hor++;
      if(modelo.hora.hor>23)
        modelo.hora.hor=0;
    }
  }

  // redesenhar o ecra 
  glutPostRedisplay(); 
}


void imprime_ajuda(void)
{
  printf("\n\nDesenho de um quadrado\n");
  printf("h,H - Ajuda \n");
  printf("p,P - Mostrador desenhado com GL_POLYGON\n");
  printf("t,T - Mostrador desenhado com GL_TRIANGLE_FAN\n");
  printf("l,L - Mostrador desenhado com GL_LINE_LOOP\n");
  printf("A   - Aumentar delay do timer\n");
  printf("d   - Diminuir delay do timer\n");
  printf("+   - Aumentar tamanho numero de Lados\n");
  printf("-   - Diminuir tamanho numero de Lados\n");
  printf("R   - Aumentar tamanho raio\n");
  printf("r   - Diminuir tamanho raio\n");
  printf("ESC - Sair\n");
}

/*******************************
***  callbacks de teclado    ***
*******************************/

// Callback para interacção via teclado (carregar na tecla)

void Key(unsigned char key, int x, int y)
{
  switch (key) {
    case 27:
      exit(1);
    // ... accoes sobre outras teclas ... 

    case 'h' :
    case 'H' :
                imprime_ajuda();
            break;
    case '+' :
                if(modelo.numLados<32)
                {
                  modelo.numLados++;
                  glutPostRedisplay(); // redesenhar o ecrã
                }
            break;
    case '-' :
                if(modelo.numLados>3)
                {
                  modelo.numLados--;
                  glutPostRedisplay(); // redesenhar o ecrã
                }
            break;
    case 'D' :
                if(estado.delay<1000)
                {
                  estado.delay+=50;
                }
            break;
    case 'd' :
                if(estado.delay>=100)
                {
                  estado.delay-=50;
                }
            break;
    case 'p' :
    case 'P' :
                modelo.tipoPoligono=GL_POLYGON;
                glutPostRedisplay(); // redesenhar o ecrã
            break;
    case 't' :
    case 'T' :
                modelo.tipoPoligono=GL_TRIANGLE_FAN;
                glutPostRedisplay(); // redesenhar o ecrã
            break;
    case 'l' :
    case 'L' :
                modelo.tipoPoligono=GL_LINE_LOOP;
                glutPostRedisplay(); // redesenhar o ecrã
            break;
    case 'R' :
                if(modelo.raio<0.9)
                {
                  modelo.raio+=0.05;
                  glutPostRedisplay(); // redesenhar o ecrã
                }
            break;
    case 'r' :
                if(modelo.raio>.2)
                {
                  modelo.raio-=0.05;
                  glutPostRedisplay(); // redesenhar o ecrã
                }
            break;

  }

  if(DEBUG)
    printf("Carregou na tecla %c\n",key);

}

// Callback para interacção via teclado (largar a tecla)

/*void KeyUp(unsigned char key, int x, int y)
{

  if(DEBUG)
    printf("Largou a tecla %c\n",key);
}/*/

// Callback para interacção via teclas especiais  (carregar na tecla)

/*void SpecialKey(int key, int x, int y)
{
  // ... accoes sobre outras teclas especiais ... 
  //    GLUT_KEY_F1 ... GLUT_KEY_F12
  //    GLUT_KEY_UP
  //    GLUT_KEY_DOWN
  //    GLUT_KEY_LEFT
  //    GLUT_KEY_RIGHT
  //    GLUT_KEY_PAGE_UP
  //    GLUT_KEY_PAGE_DOWN
  //    GLUT_KEY_HOME
  //    GLUT_KEY_END
  //    GLUT_KEY_INSERT 

  switch (key) {

  // redesenhar o ecra 
  //glutPostRedisplay();
  }


  if(DEBUG)
    printf("Carregou na tecla especial %d\n",key);
}/*/

// Callback para interacção via teclas especiais (largar a tecla)

/*void SpecialKeyUp(int key, int x, int y)
{

  if(DEBUG)
    printf("Largou a tecla especial %d\n",key);

}/*/


int main(int argc, char **argv)
{
  estado.doubleBuffer=GL_TRUE;

  glutInit(&argc, argv);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(300, 300);
  glutInitDisplayMode(((estado.doubleBuffer) ? GLUT_DOUBLE : GLUT_SINGLE) | GLUT_RGB);
  if (glutCreateWindow("Exemplo") == GL_FALSE)
    exit(1);

  Init();

  imprime_ajuda();

// Registar callbacks do GLUT
  
  // callbacks de janelas/desenho
  glutReshapeFunc(Reshape);
  glutDisplayFunc(Draw);

  // Callbacks de teclado
  glutKeyboardFunc(Key);
  //glutKeyboardUpFunc(KeyUp);
  //glutSpecialFunc(SpecialKey);
  //glutSpecialFunc(SpecialKeyUp);

  // callbacks timer/idle
  glutTimerFunc(estado.delay, Timer, 0);
  //glutIdleFunc(Idle);

  
  // COMECAR...
  glutMainLoop();

  return 0;
}