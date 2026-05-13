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
    int codigo_disciplina;
    char nome_disciplina[TAM_NOME];
    int bloco_disciplina;
    int carga_horaria;
} Disciplina;

struct rbnode; // forward declaration

typedef struct {
    int codigo_curso;
    char nome_curso[TAM_NOME];
    int qtd_blocos_curso;
    int semanas_disciplina;
    struct rbnode *raiz_disciplinas; // árvore própria
} Curso;

typedef struct {
    int matricula;
    char nome_aluno[TAM_NOME];
    int codigo_curso;
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

/* ===================== PROTÓTIPOS ===================== */
int inserirAluno(RBNode** raiz_alunos, int mat, char nome[], int cod_curso, int ano, int sem);
int inserirCurso(RBNode** raiz_cursos, int codigo, char nome[], int blocos, int semanas);
int inserirDisciplina(RBNode** raiz_disciplinas, int cod_disc, char nome[], int bloco, int carga);
int inserirDisciplinaNoCurso(RBNode* raiz_cursos, int cod_curso, int cod_disc, char nome[], int bloco, int carga);

int inserir_no(RBNode** raiz, Info info, TipoInfo tipo);
RBNode* criar_no(Info info, TipoInfo tipo);
RBNode* buscar_no(RBNode* raiz, int chave, TipoInfo tipo);
int obter_cor(RBNode* no);
void trocar_cores(RBNode* H);
RBNode* rotar_esquerda(RBNode* A);
RBNode* rotar_direita(RBNode* A);
RBNode* balancear(RBNode* H);
void imprimirCursos(RBNode* raiz);
void imprimirAlunos(RBNode* raiz);
void imprimirDisciplinas(RBNode* raiz);

#endif