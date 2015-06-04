typedef struct Estado{
	Camera		camera;
	int			xMouse,yMouse;
	GLboolean	light;
	GLboolean	apresentaNormais;
	GLint		lightViewer;
	GLint		eixoTranslaccao;
	GLdouble	eixo[3];
	ALuint buffer, source;
	ALboolean tecla_s;
}Estado;
