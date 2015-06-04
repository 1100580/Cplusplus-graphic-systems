#define HOMER_SCALE 0.05
#define ALT_HOMER 6.63

typedef struct Modelo {
	#ifdef __cplusplus
		tipo_material cor_cubo;
	#else
		enum tipo_material cor_cubo;
	#endif

	GLfloat g_pos_luz1[4];
	GLfloat g_pos_luz2[4];

	GLfloat escala;
	GLUquadric *quad;
	StudioModel homer;
}Modelo;