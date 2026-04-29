#ifndef ESTRUTURAS_23_H
#define ESTRUTURAS_23_H


typedef struct no_disciplina23 {

    struct {
        int codigo;
        char nome[100];
        int bloco;
        int carga_horaria;
    } info1, info2;

    int num_infos; 
    struct no_disciplina23 *esq, *cen, *dir;
} NoDisciplina23;

typedef struct no_curso23 {
    struct {
        int codigo;
        char nome[100];
        int qtd_blocos;
        int semanas_disciplina;
        NoDisciplina23 *raiz_disciplinas; 
    } info1, info2;

    int num_infos;
    struct no_curso23 *esq, *cen, *dir;
} NoCurso23;


typedef struct no_aluno23 {
    struct {
        int matricula;
        char nome[100];
        int codigo_curso;
        int ano_ingresso;
        int semestre_ingresso;
    } info1, info2;

    int num_infos;
    struct no_aluno23 *esq, *cen, *dir;
} NoAluno23;

#endif