#ifndef ESTRUTURAS_VP_H
#define ESTRUTURAS_VP_H
#define VERMELHO 1
#define PRETO    0

// Estrutura para as Disciplinas (organizada por código) 
typedef struct no_disciplina {
    int codigo;
    char nome[100];
    int bloco;
    int carga_horaria;
    int cor; // 1 para Vermelho, 0 para Preto
    struct no_disciplina *esq, *dir;
} NoDisciplina;


typedef struct no_curso {
    int codigo;
    char nome[100];
    int qtd_blocos;
    int semanas_disciplina;
    NoDisciplina *raiz_disciplinas; 
    int cor; // 1 para Vermelho, 0 para Preto
    struct no_curso *esq, *dir;
} NoCurso;


// Estrutura para os Alunos (organizada por matrícula) 
typedef struct no_aluno {
    int matricula;
    char nome[100];
    int codigo_curso;
    int ano_ingresso;
    int semestre_ingresso; 
    int cor; // 1 para Vermelho, 0 para Preto
    struct no_aluno *esq, *dir;
} NoAluno;

#endif