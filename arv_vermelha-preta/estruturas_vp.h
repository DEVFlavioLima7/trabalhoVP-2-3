#ifndef ESTRUTURAS_VP_H
#define ESTRUTURAS_VP_H

#define TAM_NOME 100

/* ===================== CORES ===================== */

typedef enum {
    PRETO = 0,
    VERMELHO = 1
} Cor;

/* ===================== DADOS ===================== */

typedef struct {
    int codigodisciplina;
    char nomedisciplina[TAM_NOME];
    int blocodisciplina;
    int carga_horaria;
} Disciplina;

struct rbnode; // forward declaration

typedef struct {
    int codigocurso;
    char nomecurso[TAM_NOME];
    int qtd_blocoscurso;
    int semanas_disciplina;
    struct rbnode *raiz_disciplinas; // árvore própria
} Curso;

typedef struct {
    int matricula;
    char nomealuno[TAM_NOME];
    int codigocurso;
    int ano_ingresso;
    int semestre_ingresso;
} Aluno;

/* ===================== CONTROLE DE TIPO ===================== */

typedef enum {
    TIPO_ALUNO,
    TIPO_CURSO,
    TIPO_DISCIPLINA
} TipoInfo;

/* ===================== UNION ===================== */

typedef union {
    Aluno aluno;
    Curso curso;
    Disciplina disciplina;
} Info;

/* ===================== NÓ RUBRO-NEGRO ===================== */

typedef struct rbnode {
    Info info;
    TipoInfo tipo;

    Cor cor;
    struct rbnode *esq, *dir;
} RBNode;

#endif