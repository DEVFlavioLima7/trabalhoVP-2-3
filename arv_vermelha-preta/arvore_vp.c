#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_vp.h"

/* ===================== APOIO ===================== */

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

int obter_cor(RBNode* no) {
    return (no == NULL) ? PRETO : no->cor;
}

void trocar_cores(RBNode* H) {
    H->cor = !H->cor;
    if (H->esq != NULL)
        H->esq->cor = !H->esq->cor;
    if (H->dir != NULL)
        H->dir->cor = !H->dir->cor;
}

static int obter_chave(Info info, TipoInfo tipo) {
    switch (tipo) {
        case TIPO_ALUNO:      return info.aluno.matricula;
        case TIPO_CURSO:      return info.curso.codigocurso;
        case TIPO_DISCIPLINA: return info.disciplina.codigodisciplina;
        default:              return -1;
    }
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
    // 1. Nó vermelho é sempre filho à esquerda
    if (obter_cor(H->dir) == VERMELHO && obter_cor(H->esq) == PRETO)
        H = rotar_esquerda(H);

    // 2. Filho da esquerda e neto da esquerda são vermelhos
    if (obter_cor(H->esq) == VERMELHO && obter_cor(H->esq->esq) == VERMELHO)
        H = rotar_direita(H);

    // 3. Dois filhos vermelhos: troca cor!
    if (obter_cor(H->esq) == VERMELHO && obter_cor(H->dir) == VERMELHO)
        trocar_cores(H);

    return H;
}



static RBNode* insere_recursivo(RBNode* H, Info info, TipoInfo tipo, int *resp) {
    if (H == NULL) {
        RBNode* novo = criar_no(info, tipo);
        if (novo == NULL) {
            *resp = 0;
            return NULL;
        }
        *resp = 1;
        return novo;
    }

    int valor_novo = obter_chave(info, tipo);
    int valor_atual = obter_chave(H->info, H->tipo);

    if (valor_novo == valor_atual) {
        *resp = 0; 
    } else {
        if (valor_novo < valor_atual)
            H->esq = insere_recursivo(H->esq, info, tipo, resp);
        else
            H->dir = insere_recursivo(H->dir, info, tipo, resp);
    }

    // Chama a função de balanceamento na volta da recursão
    return balancear(H);
}

int inserir_no(RBNode** raiz, Info info, TipoInfo tipo) {
    int resp;
    *raiz = insere_recursivo(*raiz, info, tipo, &resp);
    if (*raiz != NULL)
        (*raiz)->cor = PRETO;
    return resp; 
}

/* ===================== BUSCAS ===================== */

RBNode* buscar_no(RBNode* raiz, int chave, TipoInfo tipo) {
    if (raiz == NULL) return NULL;
    int chave_atual = obter_chave(raiz->info, raiz->tipo);
    if (chave == chave_atual) return raiz;
    if (chave < chave_atual)
        return buscar_no(raiz->esq, chave, tipo);
    return buscar_no(raiz->dir, chave, tipo);
}