#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas_vp.h"

/* ============================================================
   FUNÇÕES DE APOIO E MANIPULAÇÃO DA ÁRVORE (GENÉRICAS)
   ============================================================ */

rb_node* criar_no(info info, tipo_info tipo) {
    rb_node* novo = (rb_node*)malloc(sizeof(rb_node));
    if (novo) {
        novo->info = info;
        novo->tipo = tipo;
        novo->cor = VERMELHO;
        novo->esq = novo->dir = NULL;
    }
    return novo;
}

int obter_chave(info info, tipo_info tipo) {
    int chave = -1;
    switch (tipo) {
        case TIPO_ALUNO:      
            chave = info.aluno.matricula_aluno; 
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

int obter_cor(rb_node* no) {
    int cor = PRETO;
    if (no != NULL) cor = no->cor;
    return cor;
}

void trocar_cores(rb_node* H) {
    H->cor = !H->cor;
    if (H->esq != NULL)
        H->esq->cor = !H->esq->cor;
    if (H->dir != NULL) 
        H->dir->cor = !H->dir->cor;
}

rb_node* rotar_esquerda(rb_node* A) {
    rb_node* B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    B->cor = A->cor;
    A->cor = VERMELHO;
    return B;
}

rb_node* rotar_direita(rb_node* A) {
    rb_node* B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    B->cor = A->cor;
    A->cor = VERMELHO;
    return B;
}

rb_node* balancear(rb_node* H) {
    if (obter_cor(H->dir) == VERMELHO && obter_cor(H->esq) == PRETO)
        H = rotar_esquerda(H);

    if (obter_cor(H->esq) == VERMELHO && obter_cor(H->esq->esq) == VERMELHO)
        H = rotar_direita(H);

    if (obter_cor(H->esq) == VERMELHO && obter_cor(H->dir) == VERMELHO)
        trocar_cores(H);

    return H;
}

rb_node* insere_recursivo(rb_node* H, info info, tipo_info tipo, int *resp) {
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

int inserir_no(rb_node** raiz, info info, tipo_info tipo) {
    int resp;
    *raiz = insere_recursivo(*raiz, info, tipo, &resp);
    if (*raiz != NULL)
        (*raiz)->cor = PRETO;
    return resp;
}

rb_node* buscar_no(rb_node* raiz, int chave, tipo_info tipo) {
    rb_node* res = NULL;
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

int inserirAluno(rb_node** raiz_alunos, int mat, char nome[], int cod_curso, int ano, int sem) {
    int res = 0;
    if (buscar_no(*raiz_alunos, mat, TIPO_ALUNO) == NULL) {
        info info;
        info.aluno.matricula_aluno = mat;
        strcpy(info.aluno.nome_aluno, nome);
        info.aluno.codigo_curso = cod_curso;
        info.aluno.ano_ingresso = ano;
        info.aluno.semestre_ingresso = sem;
        res = inserir_no(raiz_alunos, info, TIPO_ALUNO);
    }
    return res;
}

int inserirCurso(rb_node** raiz, int codigo, char nome[], int blocos, int semanas) {
    int res = 0;
    if (buscar_no(*raiz, codigo, TIPO_CURSO) == NULL) {
        info info;
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

int inserirDisciplinaNoCurso(rb_node* raiz_cursos, int cod_curso, int cod_disc, char nome[], int bloco, int carga) {
    int res = 0;
    rb_node* no_curso = buscar_no(raiz_cursos, cod_curso, TIPO_CURSO);
    
    if (no_curso == NULL) {
        res = -1; 
    } else if (!validarRegras(bloco, no_curso->info.curso.qtd_blocos_curso, carga, no_curso->info.curso.semanas_disciplina)) {
        res = -2;
    } else if (buscar_no(no_curso->info.curso.raiz_disciplinas, cod_disc, TIPO_DISCIPLINA) == NULL) {
        info info_disc;
        info_disc.disciplina.codigo_disciplina = cod_disc;
        strcpy(info_disc.disciplina.nome_disciplina, nome);
        info_disc.disciplina.bloco_disciplina = bloco;
        info_disc.disciplina.carga_horaria = carga;
        res = inserir_no(&(no_curso->info.curso.raiz_disciplinas), info_disc, TIPO_DISCIPLINA);
    }
    
    return res;
}