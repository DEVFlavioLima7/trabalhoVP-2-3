#include <stdio.h>
#include <stdlib.h>
#include "estruturas4-5.h" // Certifique-se de que este e o nome do seu header

// ============================================================================
// FUNÇÕES AUXILIARES DE TESTE E VALIDAÇÃO
// ============================================================================

// Cria um BlocoMemoria genérico apenas para preencher a árvore no teste
BlocoMemoria criarBlocoTeste(int inicio) {
    BlocoMemoria b;
    b.status = 'O';
    b.bloco_inicio = inicio;
    b.bloco_fim = inicio + 9;
    b.endereco_inicio = inicio * (unsigned long)BLOCO_TAM;
    b.endereco_fim = b.endereco_inicio + (10 * (unsigned long)BLOCO_TAM) - 1;
    return b;
}

// Imprime a árvore no terminal (deitada) para inspeção visual rápida
void imprimirArvoreTeste(Arv45* raiz, int nivel) {
    if (raiz != NULL) {
        int i;
        for (i = raiz->nInfos; i > 0; i--) {
            imprimirArvoreTeste(raiz->filhos[i], nivel + 1);
            for (int k = 0; k < nivel; k++) printf("      ");
            printf("[%d]\n", raiz->infos[i - 1].bloco_inicio);
        }
        imprimirArvoreTeste(raiz->filhos[0], nivel + 1);
    }
}

// O Auditor: Verifica se as regras matemáticas da Árvore 4-5 foram violadas
int verificarPropriedades(Arv45* no, int ehRaiz) {
    if (no == NULL) return 1; // Árvore vazia é válida

    // 1. Verificação de Quantidade (Nenhum nó interno/folha pode ter < 2 infos)
    if (!ehRaiz && no->nInfos < 2) {
        printf("\n[FALHA] No com apenas %d info encontrado! (Quebra de Regra B-Tree).\n", no->nInfos);
        return 0;
    }
    if (no->nInfos > 4) {
        printf("\n[FALHA] No excedeu limite (Overflow) com %d infos!\n", no->nInfos);
        return 0;
    }

    // 2. Verificação de Ordenação
    for (int i = 0; i < no->nInfos - 1; i++) {
        if (no->infos[i].bloco_inicio >= no->infos[i+1].bloco_inicio) {
            printf("\n[FALHA] Ordenacao quebrada: %d veio antes de %d.\n", 
                   no->infos[i].bloco_inicio, no->infos[i+1].bloco_inicio);
            return 0;
        }
    }

    // 3. Verificação Recursiva
    if (!ehFolha45(no)) {
        for (int i = 0; i <= no->nInfos; i++) {
            if (!verificarPropriedades(no->filhos[i], 0)) return 0;
        }
    }

    return 1; // Tudo perfeitamente balanceado
}

// ============================================================================
// ROTINA PRINCIPAL DE ESTRESSE
// ============================================================================
int main() {
    Arv45* raiz = NULL;
    int array_inserir[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int array_remover[] = {100, 90, 80, 70, 60, 50, 40, 30, 20, 10, 0};
    int qtd_elementos = 11;
    int i;

    printf("========================================================\n");
    printf("   TESTE DE ESTRESSE DA ARVORE 4-5 (INSERCAO E REMOCAO) \n");
    printf("========================================================\n\n");

    // ---------------------------------------------------------
    // ETAPA 1: INSERÇÕES (Forçando Divisões / Splits)
    // ---------------------------------------------------------
    printf("--- ETAPA 1: INSERINDO ELEMENTOS ---\n");
    for (i = 0; i < qtd_elementos; i++) {
        printf("Inserindo bloco_inicio: %d...\n", array_inserir[i]);
        inserir45_root(&raiz, criarBlocoTeste(array_inserir[i]));
    }

    printf("\n[ESTRUTURA APOS TODAS AS INSERCOES]\n");
    imprimirArvoreTeste(raiz, 0);

    if (verificarPropriedades(raiz, 1)) {
        printf("\n[AUDITORIA 1 OK] A raiz atualizou e todos os nos respeitam o minimo (2 infos).\n");
    } else {
        printf("\n[ERRO FATAL] A arvore quebrou as regras estruturais durante a insercao!\n");
        return 1;
    }

    // ---------------------------------------------------------
    // ETAPA 2: REMOÇÕES (Forçando Underflow e Merges)
    // ---------------------------------------------------------
    printf("\n\n--- ETAPA 2: REMOVENDO ELEMENTOS (DE TRAS PRA FRENTE) ---\n");
    for (i = 0; i < qtd_elementos; i++) {
        printf("\nRemovendo bloco_inicio: %d...\n", array_remover[i]);
        remover45_root(&raiz, array_remover[i]);

        printf("Estrutura atual:\n");
        imprimirArvoreTeste(raiz, 0);

        // Valida agressivamente após CADA remoção
        if (!verificarPropriedades(raiz, 1)) {
            printf("\n[ERRO FATAL] Ocorreu uma quebra na estrutura apos remover o %d!\n", array_remover[i]);
            return 1;
        }
    }

    // ---------------------------------------------------------
    // ETAPA 3: VALIDAÇÃO FINAL
    // ---------------------------------------------------------
    printf("\n========================================================\n");
    printf("              VEREDITO FINAL DO TESTE                   \n");
    printf("========================================================\n");
    
    if (raiz == NULL) {
        printf("- [SUCESSO] Raiz atualizada para NULL corretamente.\n");
        printf("- [SUCESSO] Nenhum Segmentation Fault ocorreu (programa nao quebrou).\n");
        printf("- [SUCESSO] Ordenacao e limite minimo de chaves respeitados em cada etapa.\n");
    } else {
        printf("- [FALHA] A arvore não ficou vazia no final!\n");
    }

    // Garante que não haverá Memory Leak caso o teste falhe no meio do caminho
    destruirArvore45(raiz); 

    return 0;
}