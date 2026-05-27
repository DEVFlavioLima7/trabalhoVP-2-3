#ifndef ESTRUTURAS_VP_H
#define ESTRUTURAS_VP_H

#define TAM_NOME 100

struct rbnode;

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

typedef struct {
    int codigo_curso;
    char nome_curso[TAM_NOME];
    int qtd_blocos_curso;
    int semanas_disciplina;
    struct rbnode *raiz_disciplinas; // árvore própria
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

typedef struct rbnode {
    info info;
    tipo_info tipo;

    cor cor;
    struct rbnode *esq, *dir;
} rb_node;

/* ===================== PROTÓTIPOS ===================== */
int inserirAluno(rb_node** raiz_alunos, int mat, char nome[], int cod_curso, int ano, int sem);
int inserirCurso(rb_node** raiz_cursos, int codigo, char nome[], int blocos, int semanas);
int inserirDisciplinaNoCurso(rb_node* raiz_cursos, int cod_curso, int cod_disc, char nome[], int bloco, int carga);

int validarBloco(int bloco, int max_b);
int validarCarga(int carga, int semanas);

int inserir_no(rb_node** raiz, info info, tipo_info tipo);
rb_node* criar_no(info info, tipo_info tipo);
rb_node* buscar_no(rb_node* raiz, int chave, tipo_info tipo);
int obter_cor(rb_node* no);
void trocar_cores(rb_node* H);
rb_node* rotar_esquerda(rb_node* A);
rb_node* rotar_direita(rb_node* A);
rb_node* balancear(rb_node* H);
void imprimirArvoreCursos(rb_node* raiz);
void imprimirAlunos(rb_node* raiz);
void imprimirDisciplinas(rb_node* raiz);
void imprimirDadosCursos(rb_node* raiz, int codigo_curso);
void imprimirCursosComMesmaQtdBlocos(rb_node* raiz, int blocos_ref, int* encontrou);
void imprimirArvoreDisciplinas(rb_node* raiz_cursos, int codigo_curso);
void imprimirDadosDisciplina(rb_node* raiz_cursos, int cod_curso, int cod_disc);
void imprimirDisciplinasPorBlocoRec(rb_node* raiz_disc, int bloco_ref, int* encontrou);
void imprimirDisciplinasPorBloco(rb_node* raiz_cursos, int codigo_curso, int bloco);
void imprimirDisciplinasPorCHRec(rb_node* raiz_disc, int ch_ref, int* encontrou);
void imprimirDisciplinasPorCargaHoraria(rb_node* raiz_cursos, int codigo_curso, int ch_ref);
void listarAlunosPorCurso(rb_node* raiz, int cod_curso);
void listarAlunosPorCursoEAno(rb_node* raiz, int cod_curso, int ano);
int contarAlunosNoCurso(rb_node* raiz, int cod_curso);
rb_node* remover_no_disciplina(rb_node* raiz, int cod_disc);
rb_node *moverRedEsq(rb_node *raiz);
rb_node *moverRedDir(rb_node *raiz);
rb_node* procurarMenor(rb_node* raiz);
rb_node* removerMenor(rb_node* raiz);


#endif