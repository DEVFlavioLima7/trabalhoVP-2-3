#ifndef ESTRUTURAS_23_H
#define ESTRUTURAS_23_H

#define TAM_NOME 100

/* ===================== VALIDACAO ===================== */

#define REGRA_OK 0
#define REGRA_BLOCO_INVALIDO 1
#define REGRA_CARGA_INVALIDA 2

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

/* ===================== PROTÓTIPOS DAS FUNÇÕES ===================== */

// Funções de suporte e manipulação da Árvore 2-3
int obterChave23(info_com_tipo info);
arv_2_3* criarNo23(info_com_tipo info, arv_2_3* filhoEsq, arv_2_3* filhoCen);
arv_2_3* buscar23(arv_2_3* raiz, int chave);
int ehFolha23(arv_2_3 *no);

// Funções do miolo da inserção (Módulos auxiliares)
void adicionarInfoNo(arv_2_3 *no, info_com_tipo novaInfo, arv_2_3 *filhoRelativo);
arv_2_3* quebraNo(arv_2_3 *no, info_com_tipo novaInfo, info_com_tipo *promove, arv_2_3 *filhoRelativo);

// Interfaces de Inserção principal
arv_2_3* insere23Recursivo(arv_2_3 *no, info_com_tipo novaInfo, info_com_tipo *promove, int *houvePromocao);
int inserirNo23(arv_2_3 **raiz, info_com_tipo novaInfo);

// Protótipos das operações acadêmicas da 2-3
int inserirCurso23(arv_2_3** raiz, int codigo, char nome[], int blocos, int semanas);
int validarBloco(int bloco, int max_b);
int validarCarga(int carga, int semanas);
int inserirDisciplinaNoCurso23(arv_2_3* raiz_cursos, int cod_curso, int cod_disc, char nome[], int bloco, int carga);
int inserirAluno23(arv_2_3** raiz_alunos, int mat, char nome[], int cod_curso, int ano, int sem);
// Protótipos das funções específicas de exibição da 2-3
void imprimirDisciplinas23(arv_2_3* raiz);
void imprimirCursos23(arv_2_3* raiz);
void imprimirAlunos23(arv_2_3* raiz);

void listarAlunosPorCurso23(arv_2_3* raiz, int cod_curso);
void listarAlunosPorCursoEAno23(arv_2_3* raiz, int cod_curso, int ano);
int contarAlunosNoCurso23(arv_2_3* raiz, int cod_curso);



#endif

