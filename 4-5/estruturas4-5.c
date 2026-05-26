#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas4-5.h"

// Array global estático para ajudar nas fusões de memória antes de remontar a árvore
static BlocoMemoria listaAux[500];
static int qtdAux = 0;

Arv45* criarNo45(BlocoMemoria info, Arv45* esq, Arv45* centro) {
    Arv45* no = (Arv45*)malloc(sizeof(Arv45));
    no->infos[0] = info;
    no->nInfos = 1;
    no->filhos[0] = esq;
    no->filhos[1] = centro;
    for(int i = 2; i < 5; i++) no->filhos[i] = NULL;
    return no;
}

int ehFolha45(Arv45* no) {
    return (no == NULL || no->filhos[0] == NULL);
}

void adicionarInfoNo45(Arv45* no, BlocoMemoria info, Arv45* filhoDir) {
    int i = no->nInfos - 1;
    while (i >= 0 && info.bloco_inicio < no->infos[i].bloco_inicio) {
        no->infos[i + 1] = no->infos[i];
        no->filhos[i + 2] = no->filhos[i + 1];
        i--;
    }
    no->infos[i + 1] = info;
    no->filhos[i + 2] = filhoDir;
    no->nInfos++;
}

Arv45* quebraNo45(Arv45* no, BlocoMemoria info, BlocoMemoria* promove, Arv45* filhoDir) {
    BlocoMemoria v[5];
    Arv45* f[6];
    int i = no->nInfos - 1;

    for(int j = 0; j < 4; j++) { 
        v[j] = no->infos[j]; 
        f[j] = no->filhos[j]; 
    }
    f[4] = no->filhos[4];

    while(i >= 0 && info.bloco_inicio < v[i].bloco_inicio) {
        v[i + 1] = v[i]; 
        f[i + 2] = f[i + 1];
        i--;
    }
    v[i + 1] = info; 
    f[i + 2] = filhoDir;

    *promove = v[2];

    Arv45* novoDir = (Arv45*)malloc(sizeof(Arv45));
    novoDir->infos[0] = v[3];
    novoDir->infos[1] = v[4];
    novoDir->nInfos = 2;
    novoDir->filhos[0] = f[3];
    novoDir->filhos[1] = f[4];
    novoDir->filhos[2] = f[5];
    novoDir->filhos[3] = novoDir->filhos[4] = NULL;

    no->infos[0] = v[0];
    no->infos[1] = v[1];
    no->nInfos = 2;
    no->filhos[2] = f[2];
    no->filhos[3] = no->filhos[4] = NULL;

    return novoDir;
}

Arv45* inserir45(Arv45** raiz, BlocoMemoria info, BlocoMemoria* promove) {
    if (*raiz == NULL) {
        *raiz = criarNo45(info, NULL, NULL);
        return NULL;
    }

    if (ehFolha45(*raiz)) {
        if ((*raiz)->nInfos < 4) {
            adicionarInfoNo45(*raiz, info, NULL);
            return NULL;
        }
        return quebraNo45(*raiz, info, promove, NULL);
    }

    int i = 0;
    while (i < (*raiz)->nInfos && info.bloco_inicio > (*raiz)->infos[i].bloco_inicio) i++;

    BlocoMemoria promovidoFilho;
    Arv45* noNovo = inserir45(&((*raiz)->filhos[i]), info, &promovidoFilho);

    if (noNovo != NULL) {
        if ((*raiz)->nInfos < 4) {
            adicionarInfoNo45(*raiz, promovidoFilho, noNovo);
            return NULL;
        }
        return quebraNo45(*raiz, promovidoFilho, promove, noNovo);
    }
    return NULL;
}

void inserir45_root(Arv45** raiz, BlocoMemoria info) {
    BlocoMemoria promove;
    Arv45* novo = inserir45(raiz, info, &promove);
    if (novo != NULL) {
        *raiz = criarNo45(promove, *raiz, novo);
    }
}

// Auxiliares para exportar e otimizar a memória de forma sequencial (Garante intercalação)
static void descarregarEmArray(Arv45* raiz) {
    if (raiz != NULL) {
        int i;
        for (i = 0; i < raiz->nInfos; i++) {
            descarregarEmArray(raiz->filhos[i]);
            listaAux[qtdAux++] = raiz->infos[i];
        }
        descarregarEmArray(raiz->filhos[i]);
    }
}

static void liberarNosArvore(Arv45* raiz) {
    if (raiz != NULL) {
        for (int i = 0; i <= raiz->nInfos; i++) {
            liberarNosArvore(raiz->filhos[i]);
        }
        free(raiz);
    }
}

static void remontarArvore(Arv45** raiz) {
    liberarNosArvore(*raiz);
    *raiz = NULL;

    if (qtdAux == 0) return;

    BlocoMemoria listaOtimizada[500];
    int qtdOtimizada = 0;
    
    listaOtimizada[0] = listaAux[0];
    qtdOtimizada = 1;

    for (int i = 1; i < qtdAux; i++) {
        BlocoMemoria* anterior = &listaOtimizada[qtdOtimizada - 1];
        if (listaAux[i].status == anterior->status && listaAux[i].bloco_inicio == anterior->bloco_fim + 1) {
            // Junta apenas blocos adjacentes de mesmo status
            anterior->bloco_fim = listaAux[i].bloco_fim;
            anterior->endereco_fim = listaAux[i].endereco_fim;
        } else {
            listaOtimizada[qtdOtimizada] = listaAux[i];
            qtdOtimizada++;
        }
    }

    // Carrega de volta na 4-5 perfeitamente limpa e organizada
    for (int i = 0; i < qtdOtimizada; i++) {
        BlocoMemoria aux;
        inserir45(raiz, listaOtimizada[i], &aux);
    }
}

/* =========================================================================
   REQUISITO (A): CADASTRO INICIAL
   ========================================================================= */
void cadastrarMemoriaInicial(Arv45** raiz, unsigned long total_memoria_bytes) {
    BlocoMemoria novo;
    char status;
    unsigned long end_ini, end_fim;
    int bloco_atual = 0;

    printf("\n=== CONFIGURACAO INICIAL DA MEMORIA ===\n");
    printf("O primeiro no e Livre (L) ou Ocupado (O)? ");
    scanf(" %c", &status);
    printf("Endereco inicial (geralmente 0): ");
    scanf("%lu", &end_ini);
    printf("Endereco final do primeiro no: ");
    scanf("%lu", &end_fim);

    novo.status = status;
    novo.endereco_inicio = end_ini;
    novo.endereco_fim = end_fim;
    novo.bloco_inicio = bloco_atual;
    novo.bloco_fim = bloco_atual + ((end_fim - end_ini + 1) / BLOCO_TAM) - 1;
    
    bloco_atual = novo.bloco_fim + 1;
    inserir45(raiz, novo, &novo);

    while (end_fim < total_memoria_bytes - 1) {
        status = (status == 'L') ? 'O' : 'L';
        end_ini = end_fim + 1;
        
        printf("\nProximo no interpretado como [%c].\n", status);
        printf("Informe o Endereco final para este no (Memoria Max: %lu): ", total_memoria_bytes);
        scanf("%lu", &end_fim);

        if (end_fim >= total_memoria_bytes) end_fim = total_memoria_bytes - 1;

        novo.status = status;
        novo.endereco_inicio = end_ini;
        novo.endereco_fim = end_fim;
        novo.bloco_inicio = bloco_atual;
        novo.bloco_fim = bloco_atual + ((end_fim - end_ini + 1) / BLOCO_TAM) - 1;

        bloco_atual = novo.bloco_fim + 1;
        
        BlocoMemoria aux;
        inserir45(raiz, novo, &aux);
    }
}

/* =========================================================================
   REQUISITO (B): ALOCAÇÃO DINÂMICA (Muda L para O e trata quebras)
   ========================================================================= */
void alocarMemoria45(Arv45** raiz, int qtd_blocos) {
    qtdAux = 0;
    descarregarEmArray(*raiz);

    int achou = -1;
    for (int i = 0; i < qtdAux; i++) {
        int blocos_disponiveis = (listaAux[i].bloco_fim - listaAux[i].bloco_inicio) + 1;
        if (listaAux[i].status == 'L' && blocos_disponiveis >= qtd_blocos) {
            achou = i;
            break;
        }
    }

    if (achou == -1) {
        printf("\n[ERRO] Nao ha nenhum bloco livre contiguo que suporte %d blocos!\n", qtd_blocos);
        return;
    }

    BlocoMemoria alvo = listaAux[achou];
    int blocos_totais = (alvo.bloco_fim - alvo.bloco_inicio) + 1;

    if (blocos_totais == qtd_blocos) {
        listaAux[achou].status = 'O';
    } else {
        BlocoMemoria alocado;
        alocado.status = 'O';
        alocado.bloco_inicio = alvo.bloco_inicio;
        alocado.bloco_fim = alvo.bloco_inicio + qtd_blocos - 1;
        alocado.endereco_inicio = alvo.endereco_inicio;
        alocado.endereco_fim = alocado.endereco_inicio + (qtd_blocos * (unsigned long)BLOCO_TAM) - 1;

        BlocoMemoria restoLivre;
        restoLivre.status = 'L';
        restoLivre.bloco_inicio = alocado.bloco_fim + 1;
        restoLivre.bloco_fim = alvo.bloco_fim;
        restoLivre.endereco_inicio = alocado.endereco_fim + 1;
        restoLivre.endereco_fim = alvo.endereco_fim;

        for (int j = qtdAux; j > achou; j--) {
            listaAux[j] = listaAux[j - 1];
        }
        listaAux[achou] = alocado;
        listaAux[achou + 1] = restoLivre;
        qtdAux++;
    }

    remontarArvore(raiz);
    printf("\n[OK] %d blocos alocados com sucesso!\n", qtd_blocos);
}

/* =========================================================================
   REQUISITO (C): LIBERAÇÃO DE MEMÓRIA (Muda de O para L e concatena vizinhos)
   ========================================================================= */
void liberarMemoria45(Arv45** raiz, int bloco_ini, int bloco_fim) {
    qtdAux = 0;
    descarregarEmArray(*raiz);

    for (int i = 0; i < qtdAux; i++) {
        if (listaAux[i].bloco_inicio >= bloco_ini && listaAux[i].bloco_fim <= bloco_fim) {
            listaAux[i].status = 'L';
        }
    }

    remontarArvore(raiz);
    printf("\n[OK] Intervalo de blocos de %d ate %d liberados (e vizinhos concatenados)!\n", bloco_ini, bloco_fim);
}

void exibirMapaMemoria(Arv45* raiz) {
    if (raiz != NULL) {
        int i;
        for (i = 0; i < raiz->nInfos; i++) {
            exibirMapaMemoria(raiz->filhos[i]);
            printf("[%c] Blocos: %d-%d | Enderecos: %lu a %lu\n",
                   raiz->infos[i].status, raiz->infos[i].bloco_inicio, raiz->infos[i].bloco_fim,
                   raiz->infos[i].endereco_inicio, raiz->infos[i].endereco_fim);
        }
        exibirMapaMemoria(raiz->filhos[i]);
    }
}