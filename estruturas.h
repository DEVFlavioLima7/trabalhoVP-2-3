#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define PRETO 0
#define VERMELHO 1

typedef struct Disciplina {
    int codigo;
    char nome[50];
    int bloco;
    int cargaHoraria;
} Disciplina;

typedef struct NodoDisciplina {
    Disciplina dado;
    int cor; 
    struct NodoDisciplina *esq, *dir, *pai;
} NodoDisciplina;

// Estrutura para Cursos
typedef struct Curso {
    int codigo;
    char nome[50];
    int qtdBlocos;
    int semanasPorDisc;
    NodoDisciplina *raizDisciplinas; // Árvore específica de disciplinas do curso
} Curso;

typedef struct NodoCurso {
    Curso dado;
    int cor;
    struct NodoCurso *esq, *dir, *pai;
} NodoCurso;

// Estrutura para Alunos
typedef struct Aluno {
    int matricula;
    char nome[50];
    int codCurso;
    int anoIngresso;
    int semestreIngresso;
} Aluno;

typedef struct NodoAluno {
    Aluno dado;
    int cor;
    struct NodoAluno *esq, *dir, *pai;
} NodoAluno;

// =================================================================
//                    MÓDULO 2: ÁRVORE 2-3
// =================================================================

// O nó 2-3 pode armazenar até 2 informações e ter até 3 filhos
typedef struct Nodo23 {
    void *dado1; // Pode apontar para Aluno, Curso ou Disciplina
    void *dado2;
    int nInfos;  // Indica se o nó tem 1 ou 2 informações
    struct Nodo23 *esq, *cen, *dir;
} Nodo23;

// =================================================================
//                 MÓDULO 3: GERENTE DE MEMÓRIA (4-5)
// =================================================================

typedef struct Bloco {
    int id;
    int tam;
    int status; // 0 para Livre, 1 para Ocupado
} Bloco;

// Árvore B de ordem 5 (Nó 4-5)
typedef struct Nodo45 {
    Bloco dados[4];          // No máximo m-1 chaves (4)
    struct Nodo45 *filhos[5]; // No máximo m filhos (5)
    int nInfos;               // Quantidade atual de chaves no nó
} Nodo45;

#endif