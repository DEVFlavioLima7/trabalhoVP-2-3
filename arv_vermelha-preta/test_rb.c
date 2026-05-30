#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estruturas_vp.h"

// ============================================================================
// BUSCA COM RASTREAMENTO DE CAMINHO (Sem alterar o TAD original)
// ============================================================================
// Esta função faz a busca binária normal, mas possui os printfs exigidos 
// pela professora para rastrear o caminho e a cor dos nós visualmente.
rb_node* buscar_com_caminho_vp(rb_node* raiz, int chave) {
    if (raiz == NULL) {
        printf(" -> [NAO ENCONTRADO]\n");
        return NULL;
    }
    
    int chave_atual = raiz->info.curso.codigo_curso;
    char cor = (raiz->cor == PRETO) ? 'P' : 'V';

    // Imprime o nó atual visitado e a sua cor
    printf("[%d(%c)]", chave_atual, cor);

    // Verificação de sucesso
    if (chave == chave_atual) {
        printf(" -> [ALVO ENCONTRADO!]\n");
        return raiz;
    }

    // Decisão de descida
    if (chave < chave_atual) {
        printf(" -> ESQ -> ");
        return buscar_com_caminho_vp(raiz->esq, chave);
    } else {
        printf(" -> DIR -> ");
        return buscar_com_caminho_vp(raiz->dir, chave);
    }
}

// ============================================================================
// EXPERIMENTO PRINCIPAL
// ============================================================================
int main() {
    srand((unsigned int)time(NULL));
    rb_node* raiz_cursos = NULL;
    
    int qtd_total = 1000;
    int array_cursos[1000];
    int count = 0;
    
    printf("\n=======================================================\n");
    printf("   EXPERIMENTO DE BUSCA - ARVORE RUBRO-NEGRA (CURSOS) \n");
    printf("=======================================================\n\n");
    
    // 1. Populando a árvore com 1000 cursos para o teste ter relevância
    printf(">> Populando a arvore com %d cursos...\n", qtd_total);
    while (count < qtd_total) {
        int cod = (rand() % 90000) + 1000; 
        
        // Usa a sua função de inserção padrão do TAD
        if (inserirCurso(&raiz_cursos, cod, "Curso Exp", 8, 15) == 1) {
            array_cursos[count] = cod;
            count++;
        }
    }
    
    // 2. Sorteando 30 itens aleatórios que sabemos que existem na árvore
    int alvos_busca[30];
    for (int i = 0; i < 30; i++) {
        int indice_sorteado = rand() % count;
        alvos_busca[i] = array_cursos[indice_sorteado];
    }
    
    printf(">> Iniciando a busca pelos 30 itens selecionados...\n\n");
    
    // 3. Executando o experimento e cronometrando
    clock_t tempo_inicio = clock();
    
    for (int i = 0; i < 30; i++) {
        printf("Busca %02d (Alvo: %d): Caminho: ", i + 1, alvos_busca[i]);
        buscar_com_caminho_vp(raiz_cursos, alvos_busca[i]);
    }
    
    clock_t tempo_fim = clock();
    
    // 4. Exibindo os resultados de tempo
    double tempo_gasto_ms = ((double)(tempo_fim - tempo_inicio) / CLOCKS_PER_SEC) * 1000.0;
    
    printf("\n=======================================================\n");
    printf("                  RESULTADOS OBTIDOS                   \n");
    printf("=======================================================\n");
    printf("Total de buscas realizadas : 30 cursos\n");
    printf("Tamanho da Arvore VP       : %d cursos cadastrados\n", count);
    printf("Tempo total gasto          : %f milissegundos\n", tempo_gasto_ms);
    printf("Tempo medio por busca      : %f milissegundos\n", tempo_gasto_ms / 30.0);
    printf("=======================================================\n\n");
    
    return 0;
}