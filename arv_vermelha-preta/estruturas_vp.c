#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas_vp.h"

/* ============================================================
   FUNÇÕES DE APOIO E MANIPULAÇÃO DA ÁRVORE (GENÉRICAS)
   ============================================================ */

RBNode* criar_no(Info info, TipoInfo tipo) {
    RBNode* novo = (RBNode*)malloc(sizeof(RBNode));
    if (novo) {
        novo->info = info;
        novo->tipo = tipo;
        novo->cor = VERMELHO;
        novo->esq = novo->dir = NULL;
    }
    return novo;
}

int obter_chave(Info info, TipoInfo tipo) {
    int chave = -1;
    switch (tipo) {
        case TIPO_ALUNO:      
            chave = info.aluno.matricula; 
            break;
        case TIPO_CURSO:      
            chave = info.curso.codigo_curso; 
            break;
        case TIPO_DISCIPLINA: 
            chave = info.disciplina.codigo_disciplina; 
            break;
    }
    return chave;
}

int obter_cor(RBNode* no) {
    int cor = PRETO;
    if (no != NULL) cor = no->cor;
    return cor;
}

void trocar_cores(RBNode* H) {
    H->cor = !H->cor;
    if (H->esq != NULL)
        H->esq->cor = !H->esq->cor;
    if (H->dir != NULL) 
        H->dir->cor = !H->dir->cor;
}

RBNode* rotar_esquerda(RBNode* A) {
    RBNode* B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    B->cor = A->cor;
    A->cor = VERMELHO;
    return B;
}

RBNode* rotar_direita(RBNode* A) {
    RBNode* B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    B->cor = A->cor;
    A->cor = VERMELHO;
    return B;
}

RBNode* balancear(RBNode* H) {
    if (obter_cor(H->dir) == VERMELHO && obter_cor(H->esq) == PRETO)
        H = rotar_esquerda(H);

    if (obter_cor(H->esq) == VERMELHO && obter_cor(H->esq->esq) == VERMELHO)
        H = rotar_direita(H);

    if (obter_cor(H->esq) == VERMELHO && obter_cor(H->dir) == VERMELHO)
        trocar_cores(H);

    return H;
}

RBNode* insere_recursivo(RBNode* H, Info info, TipoInfo tipo, int *resp) {
    if (H == NULL) {
        H = criar_no(info, tipo);
        if (H != NULL) *resp = 1;
        else *resp = 0;
    } else {
        int valor_novo = obter_chave(info, tipo);
        int valor_atual = obter_chave(H->info, H->tipo);

        if (valor_novo < valor_atual)
            H->esq = insere_recursivo(H->esq, info, tipo, resp);
        else if (valor_novo > valor_atual)
            H->dir = insere_recursivo(H->dir, info, tipo, resp);
        else
            *resp = 0;

        H = balancear(H);
    }
    return H;
}

int inserir_no(RBNode** raiz, Info info, TipoInfo tipo) {
    int resp;
    *raiz = insere_recursivo(*raiz, info, tipo, &resp);
    if (*raiz != NULL)
        (*raiz)->cor = PRETO;
    return resp;
}

RBNode* buscar_no(RBNode* raiz, int chave, TipoInfo tipo) {
    RBNode* res = NULL;
    if (raiz != NULL) {
        int chave_atual = obter_chave(raiz->info, raiz->tipo);
        if (chave == chave_atual)
            res = raiz;
        else if (chave < chave_atual)
            res = buscar_no(raiz->esq, chave, tipo);
        else
            res = buscar_no(raiz->dir, chave, tipo);
    }
    return res;
}

/* ============================================================
   FUNÇÕES ESPECÍFICAS (ALUNO, CURSO, DISCIPLINA)
   ============================================================ */

int inserirAluno(RBNode** raiz_alunos, int mat, char nome[], int cod_curso, int ano, int sem) {
    int res = 0;
    if (buscar_no(*raiz_alunos, mat, TIPO_ALUNO) == NULL) {
        Info info;
        info.aluno.matricula = mat;
        strcpy(info.aluno.nome_aluno, nome);
        info.aluno.codigo_curso = cod_curso;
        info.aluno.ano_ingresso = ano;
        info.aluno.semestre_ingresso = sem;
        res = inserir_no(raiz_alunos, info, TIPO_ALUNO);
    }
    return res;
}

int inserirCurso(RBNode** raiz, int codigo, char nome[], int blocos, int semanas) {
    int res = 0;
    if (buscar_no(*raiz, codigo, TIPO_CURSO) == NULL) {
        Info info;
        info.curso.codigo_curso = codigo;
        strcpy(info.curso.nome_curso, nome);
        info.curso.qtd_blocos_curso = blocos;
        info.curso.semanas_disciplina = semanas;
        info.curso.raiz_disciplinas = NULL; 
        res = inserir_no(raiz, info, TIPO_CURSO);
    }
    return res;
}

int validarRegras(int bloco_disciplina, int qtd_blocos_curso, int carga, int semanas) {
    int status = 0;
    if (bloco_disciplina < qtd_blocos_curso && bloco_disciplina >= 0 && 
        carga % semanas == 0 && carga > 0) {
        status = 1;
    }
    return status;
}

int inserirDisciplinaNoCurso(RBNode* raiz_cursos, int cod_curso, int cod_disc, char nome[], int bloco, int carga) {
    int res = 0;
    RBNode* no_curso = buscar_no(raiz_cursos, cod_curso, TIPO_CURSO);
    
    if (no_curso == NULL) {
        res = -1; 
    } else if (!validarRegras(bloco, no_curso->info.curso.qtd_blocos_curso, carga, no_curso->info.curso.semanas_disciplina)) {
        res = -2;
    } else if (buscar_no(no_curso->info.curso.raiz_disciplinas, cod_disc, TIPO_DISCIPLINA) == NULL) {
        Info info_disc;
        info_disc.disciplina.codigo_disciplina = cod_disc;
        strcpy(info_disc.disciplina.nome_disciplina, nome);
        info_disc.disciplina.bloco_disciplina = bloco;
        info_disc.disciplina.carga_horaria = carga;
        res = inserir_no(&(no_curso->info.curso.raiz_disciplinas), info_disc, TIPO_DISCIPLINA);
    }
    
    return res;
}

/* --- FUNÇÕES ESPECÍFICAS DE IMPRESSÃO --- */

// 1. Especializada em Disciplinas (imprime sem mergulhar mais, pois é o nível folha)
void imprimirDisciplinas(RBNode* raiz) {
    if (raiz != NULL) {
        imprimirDisciplinas(raiz->esq);
        printf("\n  -> [DISCIPLINA] ID: %d | Nome: %s | Bloco: %d | Carga: %dh", 
                raiz->info.disciplina.codigo_disciplina, 
                raiz->info.disciplina.nome_disciplina, 
                raiz->info.disciplina.bloco_disciplina, 
                raiz->info.disciplina.carga_horaria);
        imprimirDisciplinas(raiz->dir);
    }
}

// 2. Especializada em Cursos (imprime o curso e chama a de disciplinas)
void imprimirCursos(RBNode* raiz) {
    if (raiz != NULL) {
        imprimirCursos(raiz->esq);
        printf("\n[CURSO] ID: %d | Nome: %s | Blocos: %d\n", 
                raiz->info.curso.codigo_curso, 
                raiz->info.curso.nome_curso, 
                raiz->info.curso.qtd_blocos_curso);
        printf("  -------------------------------------------");
        
        if (raiz->info.curso.raiz_disciplinas == NULL) {
            printf("\n  (Nenhuma disciplina cadastrada neste curso)\n");
        } else {
            imprimirDisciplinas(raiz->info.curso.raiz_disciplinas);
        }
        
        printf("\n  -------------------------------------------\n");
        imprimirCursos(raiz->dir);
    }
}

// 3. Especializada em Alunos
void imprimirAlunos(RBNode* raiz) {
    if (raiz != NULL) {
        imprimirAlunos(raiz->esq);
        printf("\n[ALUNO] Mat: %d | Nome: %s | Curso: %d | Ingresso: %d/%d", 
                raiz->info.aluno.matricula, 
                raiz->info.aluno.nome_aluno, 
                raiz->info.aluno.codigo_curso, 
                raiz->info.aluno.ano_ingresso, 
                raiz->info.aluno.semestre_ingresso);
        imprimirAlunos(raiz->dir);
    }
}


void listarAlunosPorCurso(RBNode* raiz, int cod_curso) {
    if (raiz != NULL) {
        // Visita a sub-árvore esquerda
        listarAlunosPorCurso(raiz->esq, cod_curso);
        
        // Processa o nó atual: verifica se é aluno e se o curso bate
        if (raiz->info.tipo == TIPO_ALUNO && raiz->info.dado.aluno.codigocurso == cod_curso) {
            printf("Matricula: %d | Nome: %s\n", 
                   raiz->info.dado.aluno.matriculaaluno, 
                   raiz->info.dado.aluno.nomealuno);
        }
        
        // Visita a sub-árvore direita
        listarAlunosPorCurso(raiz->dir, cod_curso);
    }
}

// 2. Listar alunos de um curso que entraram em um determinado ano
void listarAlunosPorCursoEAno(RBNode* raiz, int cod_curso, int ano) {
    if (raiz != NULL) {
        // Visita a sub-árvore esquerda
        listarAlunosPorCursoEAno(raiz->esq, cod_curso, ano);
        
        // Processa o nó atual: verifica as três condições
        if (raiz->info.tipo == TIPO_ALUNO && 
            raiz->info.dado.aluno.codigocurso == cod_curso && 
            raiz->info.dado.aluno.ano_ingresso == ano) {
            printf("Matricula: %d | Nome: %s | Ano: %d\n", 
                   raiz->info.dado.aluno.matriculaaluno, 
                   raiz->info.dado.aluno.nomealuno, 
                   raiz->info.dado.aluno.ano_ingresso);
        }
        
        // Visita a sub-árvore direita
        listarAlunosPorCursoEAno(raiz->dir, cod_curso, ano);
    }
}

// 3. Contar quantos alunos pertencem a um determinado curso (Ponto único de saída)
int contarAlunosNoCurso(RBNode* raiz, int cod_curso) {
    int total = 0;

    if (raiz != NULL) {
        int atual = 0;

        // Se o nó atual for do aluno e do curso procurado, conta 1
        if (raiz->info.tipo == TIPO_ALUNO && raiz->info.dado.aluno.codigocurso == cod_curso) {
            atual = 1;
        }

        // Soma o atual com o que encontrar na esquerda e na direita
        total = atual + contarAlunosNoCurso(raiz->esq, cod_curso) + 
                       contarAlunosNoCurso(raiz->dir, cod_curso);
    }

    return total;
}

