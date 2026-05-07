#ifndef ESTRUTURAS_23_H
#define ESTRUTURAS_23_H

#define TAM_NOME 100

/* ===================== DADOS ===================== */

typedef struct {
    int codigo_disciplina;
    char nome_disciplina[TAM_NOME];
    int bloco_disciplina;
    int carga_horaria;
} disciplina;

/* Forward declaration da árvore 2-3 */
typedef struct arv_2_3 arv_2_3;

typedef struct {
    int codigo_curso;
    char nome_curso[TAM_NOME];
    int qtd_blocos_curso;
    int semanas_disciplina;
    arv_2_3 *raiz_disciplinas; // mantém padrão com árvore
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


/* ===================== INFO COM TIPO ===================== */

typedef struct {
    info dado;
    tipo_info tipo;
} info_com_tipo;


/* ===================== ÁRVORE 2-3 ===================== */

struct arv_2_3 {
    info_com_tipo info[2];  // cada posição sabe seu tipo
    int n_infos;           // 1 ou 2

    struct arv_2_3 *esq;
    struct arv_2_3 *cen;
    struct arv_2_3 *dir;
};

#endif