#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_vp.h"
#include "aluno.h"

int inserirAluno(RBNode** raiz_alunos, int mat, char nome[], int cod_curso, int ano, int sem) {
   
    if (buscar_no(*raiz_alunos, mat, TIPO_ALUNO) != NULL) return 0;

    Info info;
    info.aluno.matricula = mat;
    strcpy(info.aluno.nomealuno, nome);
    info.aluno.codigocurso = cod_curso;
    info.aluno.ano_ingresso = ano;
    info.aluno.semestre_ingresso = sem;

    return inserir_no(raiz_alunos, info, TIPO_ALUNO);
}