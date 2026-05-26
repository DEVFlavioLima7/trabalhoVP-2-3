#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "estruturas4-5.h"

static BlocoMemoria criarBloco(char status, int inicio, int fim) {
    BlocoMemoria bloco;
    bloco.status = status;
    bloco.bloco_inicio = inicio;
    bloco.bloco_fim = fim;
    bloco.endereco_inicio = (unsigned long long)inicio * BLOCO_TAM;
    bloco.endereco_fim = bloco.endereco_inicio + (unsigned long long)(fim - inicio + 1) * BLOCO_TAM - 1;
    return bloco;
}

static bool validar_estrutura_45(Arv45* no) {
    if (no == NULL) return true;
    if (no->nInfos < 1 || no->nInfos > 4) return false;
    if (ehFolha45(no)) {
        for (int i = 0; i < 5; i++) {
            if (no->filhos[i] != NULL) return false;
        }
    } else {
        for (int i = 0; i <= no->nInfos; i++) {
            if (no->filhos[i] == NULL) return false;
        }
        for (int i = no->nInfos + 1; i < 5; i++) {
            if (no->filhos[i] != NULL) return false;
        }
    }
    for (int i = 1; i < no->nInfos; i++) {
        if (no->infos[i].bloco_inicio <= no->infos[i - 1].bloco_inicio) return false;
    }
    for (int i = 0; i <= no->nInfos; i++) {
        if (!validar_estrutura_45(no->filhos[i])) return false;
    }
    return true;
}

static bool validar_ordem_45(Arv45* no, int* ultimo) {
    if (no == NULL) return true;
    if (!validar_ordem_45(no->filhos[0], ultimo)) return false;
    for (int i = 0; i < no->nInfos; i++) {
        if (*ultimo != -1 && no->infos[i].bloco_inicio <= *ultimo) return false;
        *ultimo = no->infos[i].bloco_inicio;
        if (!validar_ordem_45(no->filhos[i + 1], ultimo)) return false;
    }
    return true;
}

static int altura_folhas_45(Arv45* no) {
    if (no == NULL) return 0;
    if (ehFolha45(no)) return 1;
    int altura = altura_folhas_45(no->filhos[0]);
    if (altura <= 0) return -1;
    for (int i = 1; i <= no->nInfos; i++) {
        int alturaFilho = altura_folhas_45(no->filhos[i]);
        if (alturaFilho != altura) return -1;
    }
    return altura + 1;
}

static int contar_blocos_45(Arv45* no) {
    if (no == NULL) return 0;
    int total = 0;
    for (int i = 0; i < no->nInfos; i++) {
        total += (no->infos[i].bloco_fim - no->infos[i].bloco_inicio + 1);
    }
    for (int i = 0; i <= no->nInfos; i++) {
        total += contar_blocos_45(no->filhos[i]);
    }
    return total;
}

static int contar_blocos_por_status(Arv45* no, char status) {
    if (no == NULL) return 0;
    int count = 0;
    for (int i = 0; i < no->nInfos; i++) {
        if (no->infos[i].status == status) count++;
    }
    for (int i = 0; i <= no->nInfos; i++) {
        count += contar_blocos_por_status(no->filhos[i], status);
    }
    return count;
}

static bool existe_bloco_status_tamanho(Arv45* no, char status, int inicio, int fim) {
    if (no == NULL) return false;
    for (int i = 0; i < no->nInfos; i++) {
        if (no->infos[i].status == status && no->infos[i].bloco_inicio == inicio && no->infos[i].bloco_fim == fim) {
            return true;
        }
    }
    for (int i = 0; i <= no->nInfos; i++) {
        if (existe_bloco_status_tamanho(no->filhos[i], status, inicio, fim)) return true;
    }
    return false;
}

static bool existe_bloco_status_contem(Arv45* no, char status, int inicio, int fim) {
    if (no == NULL) return false;
    for (int i = 0; i < no->nInfos; i++) {
        if (no->infos[i].status == status && no->infos[i].bloco_inicio <= inicio && no->infos[i].bloco_fim >= fim) {
            return true;
        }
    }
    for (int i = 0; i <= no->nInfos; i++) {
        if (existe_bloco_status_contem(no->filhos[i], status, inicio, fim)) return true;
    }
    return false;
}

int main() {
    Arv45* raiz = NULL;
    int intervalos[6][2] = {
        {0, 7}, {8, 15}, {16, 23}, {24, 31}, {32, 39}, {40, 47}
    };

    for (int i = 0; i < 6; i++) {
        BlocoMemoria bloco = criarBloco('L', intervalos[i][0], intervalos[i][1]);
        inserir45_root(&raiz, bloco);
    }

    printf("\n=== TESTE AUTOMATIZADO 4-5: CONSTRUCAO INICIAL ===\n");
    if (!validar_estrutura_45(raiz)) {
        printf("[ERRO] Estrutura 4-5 invalida apos insercao inicial.\n");
        return 1;
    }

    int ultimo = -1;
    if (!validar_ordem_45(raiz, &ultimo)) {
        printf("[ERRO] Ordem de blocos invalida na arvore 4-5.\n");
        return 1;
    }

    int blocos_totais = contar_blocos_45(raiz);
    if (blocos_totais != 48) {
        printf("[ERRO] Total de blocos esperado 48, obteve %d.\n", blocos_totais);
        return 1;
    }

    int altura = altura_folhas_45(raiz);
    if (altura <= 0) {
        printf("[ERRO] Altura das folhas invalida na arvore 4-5.\n");
        return 1;
    }

    printf("[OK] Arvore inicial 4-5 valida. Blocos totais = %d, altura = %d.\n", blocos_totais, altura);

    printf("\n=== TESTE AUTOMATIZADO 4-5: ALOCACAO DE 3 BLOCOS ===\n");
    alocarMemoria45(&raiz, 3);
    if (!validar_estrutura_45(raiz) || altura_folhas_45(raiz) <= 0) {
        printf("[ERRO] Estrutura invalida apos alocacao.\n");
        return 1;
    }
    if (contar_blocos_45(raiz) != 48) {
        printf("[ERRO] Total de blocos alterado apos alocacao.\n");
        return 1;
    }
    if (contar_blocos_por_status(raiz, 'O') != 1) {
        printf("[ERRO] Esperado exatamente 1 bloco ocupado apos alocacao, obteve %d.\n", contar_blocos_por_status(raiz, 'O'));
        return 1;
    }
    if (!existe_bloco_status_tamanho(raiz, 'O', 0, 2)) {
        printf("[ERRO] Bloco ocupado 0-2 nao encontrado apos alocacao.\n");
        return 1;
    }
    printf("[OK] Alocacao de 3 blocos valida.\n");

    printf("\n=== TESTE AUTOMATIZADO 4-5: LIBERACAO DE 0-2 ===\n");
    liberarMemoria45(&raiz, 0, 2);
    if (!validar_estrutura_45(raiz) || altura_folhas_45(raiz) <= 0) {
        printf("[ERRO] Estrutura invalida apos liberacao.\n");
        return 1;
    }
    if (contar_blocos_45(raiz) != 48) {
        printf("[ERRO] Total de blocos alterado apos liberacao.\n");
        return 1;
    }
    if (contar_blocos_por_status(raiz, 'O') != 0) {
        printf("[ERRO] Esperado nenhum bloco ocupado apos liberacao, obteve %d.\n", contar_blocos_por_status(raiz, 'O'));
        return 1;
    }
    if (!existe_bloco_status_contem(raiz, 'L', 0, 7)) {
        printf("[ERRO] Nao foi encontrado bloco livre contendo o intervalo 0-7 apos liberacao.\n");
        return 1;
    }
    printf("[OK] Liberacao e concatencao de blocos valida.\n");

    printf("\n=== TESTE AUTOMATIZADO 4-5: ALOCACAO DE 5 BLOCOS ===\n");
    alocarMemoria45(&raiz, 5);
    if (!validar_estrutura_45(raiz) || altura_folhas_45(raiz) <= 0) {
        printf("[ERRO] Estrutura invalida apos nova alocacao.\n");
        return 1;
    }
    if (!existe_bloco_status_tamanho(raiz, 'O', 0, 4)) {
        printf("[ERRO] Bloco ocupado 0-4 nao encontrado apos alocacao de 5.\n");
        return 1;
    }
    printf("[OK] Alocacao adicional de 5 blocos valida.\n");

    printf("\n=== TESTE AUTOMATIZADO 4-5: RESULTADO ===\n");
    exibirMapaMemoria(raiz);
    printf("\n[SUCESSO] Testes automatizados 4-5 concluídos sem falhas.\n");
    return 0;
}
