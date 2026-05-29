#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas2-3.h" // Substitua pelo nome do seu header

// Função auxiliar para injetar dados rapidamente no teste
info_com_tipo criarCursoMock(int codigo, const char* nome) {
    info_com_tipo nova_info;
    nova_info.tipo = TIPO_CURSO; 
    nova_info.dado.curso.codigo_curso = codigo;
    strcpy(nova_info.dado.curso.nome_curso, nome);
    nova_info.dado.curso.qtd_blocos_curso = 8;
    nova_info.dado.curso.raiz_disciplinas = NULL; 
    return nova_info;
}

// Limpa a árvore entre um teste e outro
void destruirArvore(arv_2_3 **raiz) {
    if (*raiz != NULL) {
        destruirArvore(&((*raiz)->esq));
        destruirArvore(&((*raiz)->cen));
        if ((*raiz)->n_infos == 2) destruirArvore(&((*raiz)->dir));
        free(*raiz);
        *raiz = NULL;
    }
}

int main() {
    arv_2_3 *raiz = NULL;

    printf("\n\n=================================================\n");
    printf(" CENARIO 1: UNDERFLOW COM MERGE (ALTURA DIMINUI)\n");
    printf("=================================================\n");
    // Inserimos 3 elementos: a árvore dividirá e terá Nível 0 e Nível 1
    inserirNo23(&raiz, criarCursoMock(20, "Fisica"));
    inserirNo23(&raiz, criarCursoMock(10, "Matematica"));
    inserirNo23(&raiz, criarCursoMock(30, "Quimica"));
    
    printf("\n[ANTES DA REMOCAO]\n");
    imprimirCursos23(raiz);
    
    printf("\n>>> Removendo o curso 10...\n");
    removerNo23(&raiz, 10);
    
    printf("\n[DEPOIS DA REMOCAO - Veja que todos subiram para o Nivel 0]\n");
    imprimirCursos23(raiz);
    
    destruirArvore(&raiz);


    printf("\n\n=================================================\n");
    printf(" CENARIO 2: REDISTRIBUICAO (PEGA DO IRMAO)\n");
    printf("=================================================\n");
    inserirNo23(&raiz, criarCursoMock(20, "Fisica"));
    inserirNo23(&raiz, criarCursoMock(10, "Matematica"));
    inserirNo23(&raiz, criarCursoMock(40, "Biologia"));
    inserirNo23(&raiz, criarCursoMock(30, "Quimica"));
    
    printf("\n[ANTES DA REMOCAO]\n");
    imprimirCursos23(raiz);
    
    printf("\n>>> Removendo o curso 10...\n");
    removerNo23(&raiz, 10);
    
    printf("\n[DEPOIS DA REMOCAO - A arvore manteve a altura, mas rebalanceou]\n");
    imprimirCursos23(raiz);
    
    destruirArvore(&raiz);


    printf("\n\n=================================================\n");
    printf(" CENARIO 3: REMOCAO NA RAIZ PRINCIPAL (NO INTERNO)\n");
    printf("=================================================\n");
    inserirNo23(&raiz, criarCursoMock(40, "Engenharia")); 
    inserirNo23(&raiz, criarCursoMock(20, "Fisica")); 
    inserirNo23(&raiz, criarCursoMock(60, "Medicina"));
    inserirNo23(&raiz, criarCursoMock(10, "Matematica")); 
    inserirNo23(&raiz, criarCursoMock(30, "Quimica")); 
    inserirNo23(&raiz, criarCursoMock(50, "Direito")); 
    inserirNo23(&raiz, criarCursoMock(70, "Filosofia"));
    
    printf("\n[ANTES DA REMOCAO]\n");
    imprimirCursos23(raiz);
    
    printf("\n>>> Removendo o curso 40 (O predecessor '30' deve assumir o Nivel 0)...\n");
    removerNo23(&raiz, 40);
    
    printf("\n[DEPOIS DA REMOCAO]\n");
    imprimirCursos23(raiz);
    
    destruirArvore(&raiz);

    return 0;
}