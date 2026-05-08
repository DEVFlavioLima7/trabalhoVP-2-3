#ifndef CURSO_H
#define CURSO_H
#include "arvore_vp.h"


int inserirCurso(RBNode** raiz, int codigo, char nome[], int blocos, int semanas);
int inserirDisciplinaNoCurso(RBNode* raiz_cursos, int cod_curso, int cod_disc, char nome[], int bloco, int carga);

#endif