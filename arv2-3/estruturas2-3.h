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

/* ===================== PROTÓTIPOS DAS FUNÇÕES ===================== */

// Funções de suporte e manipulação da Árvore 2-3
int obterChave23(InfoComTipo info);
Arv23* criarNo23(InfoComTipo info, Arv23* filhoEsq, Arv23* filhoCen);
Arv23* buscar23(Arv23* raiz, int chave);
int ehFolha23(Arv23 *no);

// Funções do miolo da inserção (Módulos auxiliares)
void adicionarInfoNo(Arv23 *no, InfoComTipo novaInfo, Arv23 *filhoRelativo);
Arv23* quebraNo(Arv23 *no, InfoComTipo novaInfo, InfoComTipo *promove, Arv23 *filhoRelativo);

// Interfaces de Inserção principal
Arv23* insere23Recursivo(Arv23 *no, InfoComTipo novaInfo, InfoComTipo *promove, int *houvePromocao);
int inserirNo23(Arv23 **raiz, InfoComTipo novaInfo);

// Protótipos das operações acadêmicas da 2-3
int inserirCurso23(Arv23** raiz, int codigo, char nome[], int blocos, int semanas);
int validarRegras23(int bloco_disciplina, int qtd_blocos_curso, int carga, int semanas);
int inserirDisciplinaNoCurso23(Arv23* raiz_cursos, int cod_curso, int cod_disc, char nome[], int bloco, int carga);
int inserirAluno23(Arv23** raiz_alunos, int mat, char nome[], int cod_curso, int ano, int sem);
// Protótipos das funções específicas de exibição da 2-3
void imprimirDisciplinas23(Arv23* raiz);
void imprimirCursos23(Arv23* raiz);
void imprimirAlunos23(Arv23* raiz);

void listarAlunosPorCurso23(Arv23* raiz, int cod_curso);
void listarAlunosPorCursoEAno23(Arv23* raiz, int cod_curso, int ano);
int contarAlunosNoCurso23(Arv23* raiz, int cod_curso);



#endif

