#ifndef ESTRUTURAS_VP_H
#define ESTRUTURAS_VP_H

#define TAM_NOME 100

/* ===================== CORES ===================== */

typedef enum {
    PRETO = 0,
    VERMELHO = 1
} cor;

/* ===================== DADOS ===================== */

typedef struct {
    int codigo_disciplina;
    char nome_disciplina[TAM_NOME];
    int bloco_disciplina;
    int carga_horaria;
} disciplina;

struct rb_node; // forward declaration

typedef struct {
    int codigo_curso;
    char nome_curso[TAM_NOME];
    int qtd_blocos_curso;
    int semanas_disciplina;
    struct rb_node *raiz_disciplinas; // árvore própria
} curso;

typedef struct {
    int matricula_aluno;
    char nome_aluno[TAM_NOME];
    int codigo_curso;
    int ano_ingresso;
    int semestre_ingresso;
} aluno;

/* ===================== CONTROLE DE TIPO ===================== */

typedef enum {
    TIPO_ALUNO,
    TIPO_CURSO,
    TIPO_DISCIPLINA
} tipo_info;

/* ===================== UNION ===================== */

typedef union {
    aluno aluno;
    curso curso;
    disciplina disciplina;
} info;

/* ===================== NÓ RUBRO-NEGRO ===================== */

typedef struct rb_node {
    info info;
    tipo_info tipo;

    cor cor;
    struct rb_node *esq, *dir;
} rb_node;

#endif