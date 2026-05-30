#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estruturas2-3.h"

// ============================================================================
// FUNÇÃO AUXILIAR PARA INSERÇÃO (Reaproveitada)
// ============================================================================
int inserirCursoExperimento(arv_2_3** raiz, int codigo) {
    info_com_tipo nova;
    nova.tipo = TIPO_CURSO;
    nova.dado.curso.codigo_curso = codigo;
    strcpy(nova.dado.curso.nome_curso, "Curso Experimento");
    nova.dado.curso.qtd_blocos_curso = 8;
    nova.dado.curso.semanas_disciplina = 15;
    nova.dado.curso.raiz_disciplinas = NULL;
    
    return inserirNo23(raiz, nova);
}

// ============================================================================
// BUSCA COM RASTREAMENTO DE CAMINHO (Sem alterar o TAD original)
// ============================================================================
// Esta função faz exatamente o que a buscar23 faz, mas possui os printfs 
// exigidos pela professora para rastrear o caminho visualmente.
arv_2_3* buscar_com_caminho_23(arv_2_3* raiz, int chave) {
    if (raiz == NULL) {
        printf(" -> [NAO ENCONTRADO]\n");
        return NULL;
    }
    
    int chave0 = obterChave23(raiz->info[0]);
    int chave1 = (raiz->n_infos == 2) ? obterChave23(raiz->info[1]) : -1;

    // Imprime o nó atual visitado
    if (raiz->n_infos == 1) {
        printf("[%d]", chave0);
    } else {
        printf("[%d | %d]", chave0, chave1);
    }

    // Verificações de sucesso
    if (chave == chave0) {
        printf(" -> [ALVO ENCONTRADO!]\n");
        return raiz;
    }
    if (raiz->n_infos == 2 && chave == chave1) {
        printf(" -> [ALVO ENCONTRADO!]\n");
        return raiz;
    }

    // Decisão de descida
    if (chave < chave0) {
        printf(" -> ESQ -> ");
        return buscar_com_caminho_23(raiz->esq, chave);
    } else if (raiz->n_infos == 1 || chave < chave1) {
        printf(" -> CEN -> ");
        return buscar_com_caminho_23(raiz->cen, chave);
    } else {
        printf(" -> DIR -> ");
        return buscar_com_caminho_23(raiz->dir, chave);
    }
}

// ============================================================================
// EXPERIMENTO PRINCIPAL
// ============================================================================
int main() {
    srand((unsigned int)time(NULL));
    arv_2_3* raiz_cursos = NULL;
    
    int qtd_total = 1000;
    int array_cursos[1000];
    int count = 0;
    
    printf("\n=======================================================\n");
    printf("   EXPERIMENTO DE BUSCA - ARVORE 2-3 (CURSOS) \n");
    printf("=======================================================\n\n");
    
    // 1. Populando a árvore com 1000 cursos para o teste ser válido
    printf(">> Populando a arvore com %d cursos...\n", qtd_total);
    while (count < qtd_total) {
        int cod = (rand() % 90000) + 1000; 
        if (inserirCursoExperimento(&raiz_cursos, cod)) {
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
        buscar_com_caminho_23(raiz_cursos, alvos_busca[i]);
    }
    
    clock_t tempo_fim = clock();
    
    // 4. Exibindo os resultados de tempo
    double tempo_gasto_ms = ((double)(tempo_fim - tempo_inicio) / CLOCKS_PER_SEC) * 1000.0;
    
    printf("\n=======================================================\n");
    printf("                  RESULTADOS OBTIDOS                   \n");
    printf("=======================================================\n");
    printf("Total de buscas realizadas : 30 cursos\n");
    printf("Tamanho da Arvore 2-3      : %d cursos cadastrados\n", count);
    printf("Tempo total gasto          : %f milissegundos\n", tempo_gasto_ms);
    printf("Tempo medio por busca      : %f milissegundos\n", tempo_gasto_ms / 30.0);
    printf("=======================================================\n\n");
    
    return 0;
}