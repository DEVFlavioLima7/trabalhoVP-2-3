#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_vp.h"
#include "curso.h"

/* Insere um novo curso na árvore principal */
int inserirCurso(RBNode** raiz, int codigo, char nome[], int blocos, int semanas) {
    // Verifica se o curso já existe
    if (buscar_no(*raiz, codigo, TIPO_CURSO) != NULL) return 0; 

    Info info;
    info.curso.codigocurso = codigo;
    strcpy(info.curso.nomecurso, nome);
    info.curso.qtd_blocoscurso = blocos;
    info.curso.semanas_disciplina = semanas;
    
    // Todo curso nasce com sua árvore de disciplinas vazia
    info.curso.raiz_disciplinas = NULL; 

    return inserir_no(raiz, info, TIPO_CURSO);
}