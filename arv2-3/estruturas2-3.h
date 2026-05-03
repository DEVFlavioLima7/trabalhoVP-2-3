#ifndef ESTRUTURAS_23_H
#define ESTRUTURAS_23_H

#define TAM_NOME 100

/* ===================== DADOS ===================== */

typedef struct {
    int codigodisciplina;
    char nomedisciplina[TAM_NOME];
    int blocodisciplina;
    int carga_horaria;
} Disciplina;

/* Forward declaration da árvore 2-3 */
typedef struct arv23 Arv23;

typedef struct {
    int codigocurso;
    char nomecurso[TAM_NOME];
    int qtd_blocoscurso;
    int semanas_disciplina;
    Arv23 *raiz_disciplinas; // mantém padrão com árvore
} Curso;

typedef struct {
    int matriculaaluno;
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


/* ===================== INFO COM TIPO ===================== */

typedef struct {
    Info dado;
    TipoInfo tipo;
} InfoComTipo;


/* ===================== ÁRVORE 2-3 ===================== */

struct arv23 {
    InfoComTipo info[2];  // cada posição sabe seu tipo
    int nInfos;           // 1 ou 2

    struct arv23 *esq;
    struct arv23 *cen;
    struct arv23 *dir;
};

#endif