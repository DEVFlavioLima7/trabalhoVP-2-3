#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "estruturas2-3.h"

// Função auxiliar para contar o tamanho real da árvore (quantidade de chaves)
int contar_chaves_23(arv_2_3* raiz) {
    if (raiz == NULL) return 0;
    int count = raiz->n_infos;
    count += contar_chaves_23(raiz->esq);
    count += contar_chaves_23(raiz->cen);
    if (raiz->n_infos == 2) {
        count += contar_chaves_23(raiz->dir);
    }
    return count;
}

// 1. Verifica propriedades estruturais e de busca (BST) de uma árvore 2-3
// Retorna a altura da subárvore, ou -1 se for inválida
int validar_arvore_23_recursivo(arv_2_3* node, int min, int max) {
    if (node == NULL) return 0; // Altura 0

    if (node->n_infos < 1 || node->n_infos > 2) {
        printf("  [ERRO] No com numero invalido de chaves (%d)!\n", node->n_infos);
        return -1;
    }

    int key1 = obterChave23(node->info[0]);
    if (key1 <= min || key1 >= max) {
        printf("  [ERRO] Chave 1 (%d) fora dos limites (%d, %d)!\n", key1, min, max);
        return -1;
    }

    int key2 = max;
    if (node->n_infos == 2) {
        key2 = obterChave23(node->info[1]);
        if (key2 <= key1 || key2 >= max) {
            printf("  [ERRO] Chave 2 (%d) fora dos limites (%d, %d) ou menor/igual a chave 1 (%d)!\n", key2, key1, max, key1);
            return -1;
        }
    }

    // Verifica filhos se é folha ou não
    if (ehFolha23(node)) {
        if (node->esq || node->cen || node->dir) {
            printf("  [ERRO] No eh considerado folha mas tem filhos nao nulos!\n");
            return -1;
        }
        return 1; // Altura 1 para folha
    } else {
        if (node->esq == NULL || node->cen == NULL) {
            printf("  [ERRO] No interno faltando filhos essenciais (esq ou cen)!\n");
            return -1;
        }
        if (node->n_infos == 1 && node->dir != NULL) {
             printf("  [ERRO] No com 1 chave tem filho direito!\n");
             return -1;
        }
        if (node->n_infos == 2 && node->dir == NULL) {
            printf("  [ERRO] No com 2 chaves faltando filho direito!\n");
            return -1;
        }
    }

    int h_esq = validar_arvore_23_recursivo(node->esq, min, key1);
    int h_cen = validar_arvore_23_recursivo(node->cen, key1, (node->n_infos == 2) ? key2 : max);
    int h_dir = 0;
    
    if (node->n_infos == 2) {
        h_dir = validar_arvore_23_recursivo(node->dir, key2, max);
    } else {
        h_dir = h_cen; // Igualamos para não falhar a checagem abaixo se nInfos == 1
    }

    if (h_esq == -1 || h_cen == -1 || h_dir == -1) return -1;

    if (h_esq != h_cen || h_esq != h_dir) {
        printf("  [ERRO] Folhas em diferentes alturas! (h_esq=%d, h_cen=%d, h_dir=%d) no no com chave %d\n", h_esq, h_cen, h_dir, key1);
        return -1;
    }

    return h_esq + 1;
}

int validar_arvore_23(arv_2_3* raiz) {
    if (raiz == NULL) return 1;
    int altura = validar_arvore_23_recursivo(raiz, -2147483647, 2147483647);
    return altura != -1;
}

// Imprimir Arvore Desenhada Visualmente com Conexoes
void imprimir_arvore_visual_23(arv_2_3* raiz, int espaco, int direcao) {
    if (raiz == NULL) return;
    
    int espaco_count = 8;
    espaco += espaco_count;
    
    if (raiz->n_infos == 2) {
        imprimir_arvore_visual_23(raiz->dir, espaco, 1);
    }
    
    imprimir_arvore_visual_23(raiz->cen, espaco, 0);
    
    printf("\n");
    for (int i = espaco_count; i < espaco; i++) {
        printf(" ");
    }
    
    if (direcao == 1) printf("/(D)"); 
    else if (direcao == -1) printf("\\(E)"); 
    else if (direcao == 0) printf("|(C)");
    else printf("-"); 
    
    printf("--");
    
    if (raiz->n_infos == 1) {
        printf("\033[1;34m[%d]\033[0m", obterChave23(raiz->info[0])); 
    } else {
        printf("\033[1;35m[%d|%d]\033[0m", obterChave23(raiz->info[0]), obterChave23(raiz->info[1])); 
    }
    
    imprimir_arvore_visual_23(raiz->esq, espaco, -1);
}

int main() {
    srand((unsigned int)time(NULL));
    arv_2_3* raiz_alunos = NULL;
    
    int qtd_insercoes = 1000;
    int chaves_inseridas[qtd_insercoes];
    int count = 0;
    
    printf("\n=======================================================\n");
    printf("   INICIANDO TESTES AUTOMATIZADOS - ARVORE 2-3 \n");
    printf("=======================================================\n\n");
    
    printf(">> 1. Inserindo %d alunos aleatorios...\n", qtd_insercoes);
    for (int i = 0; i < qtd_insercoes; i++) {
        int mat = (rand() % 90000) + 10000; // Matriculas entre 10000 e 99999
        int res = inserirAluno23(&raiz_alunos, mat, "Aluno Teste", 1, 2026, 1);
        
        if (res == 1) { // Só guarda se foi inserido com sucesso (não era duplicata)
            chaves_inseridas[count++] = mat;
        }
    }
    printf("   [OK] Foram inseridas %d chaves UNICAS na arvore.\n", count);
    
    int total_chaves = contar_chaves_23(raiz_alunos);
    if (total_chaves != count) {
        printf("   [ERRO FATAL] Quantidade de chaves reais (%d) diverge das inseridas (%d)!\n", total_chaves, count);
        return 1;
    } else {
        printf("   [OK] Contagem de chaves da arvore (%d) confere.\n", total_chaves);
    }

    printf("\n>> 2. Validando integridade estrutural da Arvore (Regras 2-3)...\n");
    if (validar_arvore_23(raiz_alunos)) {
        printf("   [OK] A arvore passou em TODAS as propriedades 2-3!\n");
        printf("        - Cada no tem 1 ou 2 chaves\n");
        printf("        - Ordem da Busca mantida (Esq < K1 < Cen < K2 < Dir)\n");
        printf("        - Todas as folhas estao na mesma profundidade\n");
    } else {
        printf("   [ERRO FATAL] A arvore quebrou as regras 2-3!\n");
        return 1;
    }
    
    printf("\n>> 3. Validando BLINDAGEM contra chaves duplicadas...\n");
    int sucesso_blindagem = 1;
    for (int i = 0; i < 50; i++) { // Tentando inserir as primeiras 50 que já sabemos que estão na árvore
        int res = inserirAluno23(&raiz_alunos, chaves_inseridas[i], "Aluno Clone", 1, 2026, 1);
        if (res != 0) { // Deveria retornar 0 (falha por já existir)
            sucesso_blindagem = 0;
            printf("   [ERRO FATAL] Permitiu inserir a matricula duplicada %d!\n", chaves_inseridas[i]);
            break;
        }
    }
    
    if (sucesso_blindagem) {
        printf("   [OK] Sistema rejeitou todas as insercoes duplicadas perfeitamente.\n");
    }
    
    if (contar_chaves_23(raiz_alunos) != count) {
        printf("   [ERRO FATAL] O tamanho da arvore foi alterado pelas duplicatas escondidas!\n");
        return 1;
    }
    
    printf("\n>> 4. Gerando teste visual para uma arvore pequena (10 elementos)...\n");
    arv_2_3* arvore_pequena = NULL;
    int vetor[10] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 90};
    for(int i=0; i<10; i++) {
        inserirAluno23(&arvore_pequena, vetor[i], "T", 1, 2026, 1);
    }
    printf("   --- Arvore 2-3 Gerada ---\n\n");
    imprimir_arvore_visual_23(arvore_pequena, 0, 0);
    printf("\n\n   ------------------------------------------------\n");
    
    printf("\n>> SUCESSO ABSOLUTO! O TAD da Arvore 2-3 esta impecavel.\n\n");
    
    return 0;
}
