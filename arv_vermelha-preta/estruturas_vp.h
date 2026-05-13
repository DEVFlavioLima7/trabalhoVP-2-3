#ifndef ESTRUTURAS_VP_H
#define ESTRUTURAS_VP_H

#define TAM_NOME 100

/* ===================== CORES ===================== */

typedef enum {
    PRETO = 0,
    VERMELHO = 1
<<<<<<< HEAD
} cor;
=======
} Cor;
>>>>>>> 0ea4239800a60456e882b89a9ff2e98f1ab40eb4

/* ===================== DADOS ===================== */

typedef struct {
    int codigo_disciplina;
    char nome_disciplina[TAM_NOME];
    int bloco_disciplina;
    int carga_horaria;
<<<<<<< HEAD
} disciplina;

struct rb_node; // forward declaration
=======
} Disciplina;

struct rbnode; // forward declaration
>>>>>>> 0ea4239800a60456e882b89a9ff2e98f1ab40eb4

typedef struct {
    int codigo_curso;
    char nome_curso[TAM_NOME];
    int qtd_blocos_curso;
    int semanas_disciplina;
<<<<<<< HEAD
    struct rb_node *raiz_disciplinas; // árvore própria
} curso;

typedef struct {
    int matricula_aluno;
=======
    struct rbnode *raiz_disciplinas; // árvore própria
} Curso;

typedef struct {
    int matricula;
>>>>>>> 0ea4239800a60456e882b89a9ff2e98f1ab40eb4
    char nome_aluno[TAM_NOME];
    int codigo_curso;
    int ano_ingresso;
    int semestre_ingresso;
<<<<<<< HEAD
} aluno;
=======
} Aluno;
>>>>>>> 0ea4239800a60456e882b89a9ff2e98f1ab40eb4

/* ===================== CONTROLE DE TIPO ===================== */

typedef enum {
    TIPO_ALUNO,
    TIPO_CURSO,
    TIPO_DISCIPLINA
<<<<<<< HEAD
} tipo_info;
=======
} TipoInfo;
>>>>>>> 0ea4239800a60456e882b89a9ff2e98f1ab40eb4

/* ===================== UNION ===================== */

typedef union {
<<<<<<< HEAD
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
=======
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

>>>>>>> 0ea4239800a60456e882b89a9ff2e98f1ab40eb4

#endif