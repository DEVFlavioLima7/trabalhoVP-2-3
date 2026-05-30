#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Lembre-se de verificar o nome do seu arquivo de cabeçalho
#include "estruturas4-5.h" 

// ============================================================================
// MÓDULO ESTRUTURAL: FUNÇÕES BÁSICAS E DE INSERÇÃO
// ============================================================================

Arv45* criarNo45(BlocoMemoria info, Arv45* esq, Arv45* direita)
{
    Arv45* no = (Arv45*)malloc(sizeof(Arv45));
    if (no == NULL)
    {
        printf("\n[ERRO] Falha na alocacao de memoria.\n");
        exit(EXIT_FAILURE);
    }

    no->nInfos = 1; 
    no->infos[0] = info;

    for (int i = 0; i < 5; i++)
    {
        no->filhos[i] = NULL;
    }

    no->filhos[0] = esq;
    no->filhos[1] = direita;

    return no;
}

int ehFolha45(Arv45* no) 
{
    int ehFolha = 0;
    if (no == NULL || no->filhos[0] == NULL)
    {
        ehFolha = 1;
    }
    return ehFolha;
}

int encontrarPosicao(Arv45 *no, int bloco_inicio)
{
    int pos = 0;
    while (pos < no->nInfos && bloco_inicio > no->infos[pos].bloco_inicio)
    {
        pos++;
    }
    return pos;
}

void adicionarInfoNo45(Arv45* no, BlocoMemoria info, Arv45* filhoDir)
{
    if (no->nInfos >= 4)
    {
        printf("\n[ERRO] Tentativa de inserir em no cheio.\n");
        return;
    }

    // DRY: Usa encontrarPosicao em vez de fazer o laço while manualmente
    int pos = encontrarPosicao(no, info.bloco_inicio);

    for (int i = no->nInfos - 1; i >= pos; i--)
    {
        no->infos[i + 1] = no->infos[i];
        no->filhos[i + 2] = no->filhos[i + 1];
    }

    no->infos[pos] = info;
    no->filhos[pos + 1] = filhoDir;
    no->nInfos++;
}

Arv45* quebraNo45(Arv45* no, BlocoMemoria info, BlocoMemoria* promove, Arv45* filhoDir)
{
    BlocoMemoria v[5];
    Arv45* f[6] = { NULL };      

    for (int i = 0; i < 4; i++) { v[i] = no->infos[i]; }
    for (int i = 0; i < 5; i++) { f[i] = no->filhos[i]; }

    int pos = 4;
    while (pos > 0 && info.bloco_inicio < v[pos - 1].bloco_inicio)
    {
        v[pos] = v[pos - 1];
        pos--;
    }

    v[pos] = info;

    for (int i = 5; i > pos + 1; i--)
    {
        f[i] = f[i - 1];
    }

    f[pos + 1] = filhoDir;
    *promove = v[2];

    Arv45* novoDir = (Arv45*)malloc(sizeof(Arv45));
    if (novoDir == NULL)
    {
        printf("\n[ERRO] Falha na alocacao de memoria.\n");
        exit(EXIT_FAILURE);
    }
    memset(novoDir, 0, sizeof(Arv45));

    novoDir->nInfos = 2;
    novoDir->infos[0] = v[3];
    novoDir->infos[1] = v[4];
    novoDir->filhos[0] = f[3];
    novoDir->filhos[1] = f[4];
    novoDir->filhos[2] = f[5];

    no->nInfos = 2;
    no->infos[0] = v[0];
    no->infos[1] = v[1];
    no->filhos[0] = f[0];
    no->filhos[1] = f[1];
    no->filhos[2] = f[2];
    no->filhos[3] = NULL;
    no->filhos[4] = NULL;

    return novoDir;
}

Arv45* inserir45(Arv45** raiz, BlocoMemoria info, BlocoMemoria* promove) {
    Arv45* resultado = NULL;

    if (*raiz == NULL) {
        *raiz = criarNo45(info, NULL, NULL);
        resultado = NULL;
    }
    else if (ehFolha45(*raiz)) {
        if ((*raiz)->nInfos < 4) {
            adicionarInfoNo45(*raiz, info, NULL);
            resultado = NULL; 
        }
        else {
            resultado = quebraNo45(*raiz, info, promove, NULL);
        }
    }
    else {
        // DRY: Usando encontrarPosicao
        int i = encontrarPosicao(*raiz, info.bloco_inicio);

        BlocoMemoria promovidoFilho;
        Arv45* noNovo = inserir45(&((*raiz)->filhos[i]), info, &promovidoFilho);

        if (noNovo != NULL) {
            if ((*raiz)->nInfos < 4) {
                adicionarInfoNo45(*raiz, promovidoFilho, noNovo);
                resultado = NULL; 
            }
            else {
                resultado = quebraNo45(*raiz, promovidoFilho, promove, noNovo);
            }
        }
        else {
            resultado = NULL;
        }
    }
    return resultado;
}

void inserir45_root(Arv45** raiz, BlocoMemoria info)
{
    int podeInserir = 1;
    BlocoMemoria temp;

    // DRY: Substituí o existeBlocoInicio pela nossa função de busca blindada
    if (buscarBlocoPorNumero(*raiz, info.bloco_inicio, &temp))
    {
        printf("\n[ERRO] Bloco ja existente.\n");
        podeInserir = 0;
    }

    if (podeInserir)
    {
        BlocoMemoria promove;
        Arv45* novo = inserir45(raiz, info, &promove);

        if (novo != NULL)
        {
            *raiz = criarNo45(promove, *raiz, novo);
        }
    }
}

// ============================================================================
// MÓDULO ESTRUTURAL: FUNÇÕES DE REMOÇÃO E UNDERFLOW
// ============================================================================
BlocoMemoria obterPredecessor(Arv45* no) {
    while (!ehFolha45(no)) {
        no = no->filhos[no->nInfos];
    }
    return no->infos[no->nInfos - 1];
}

void tratarUnderflow(Arv45* pai, int indiceFilho)
{
    Arv45* filho = pai->filhos[indiceFilho];
    Arv45* esq = (indiceFilho > 0) ? pai->filhos[indiceFilho - 1] : NULL;
    Arv45* dir = (indiceFilho < pai->nInfos) ? pai->filhos[indiceFilho + 1] : NULL;

    /* 1. EMPRÉSTIMO DA ESQUERDA */
    if (esq != NULL && esq->nInfos > 2)
    {
        for (int i = filho->nInfos; i > 0; i--) { filho->infos[i] = filho->infos[i - 1]; }
        for (int i = filho->nInfos + 1; i > 0; i--) { filho->filhos[i] = filho->filhos[i - 1]; }

        filho->infos[0] = pai->infos[indiceFilho - 1];
        filho->filhos[0] = esq->filhos[esq->nInfos];
        pai->infos[indiceFilho - 1] = esq->infos[esq->nInfos - 1];
        esq->filhos[esq->nInfos] = NULL;
        esq->nInfos--;
        filho->nInfos++;
        return;
    }

    /* 2. EMPRÉSTIMO DA DIREITA */
    if (dir != NULL && dir->nInfos > 2)
    {
        filho->infos[filho->nInfos] = pai->infos[indiceFilho];
        filho->filhos[filho->nInfos + 1] = dir->filhos[0];
        pai->infos[indiceFilho] = dir->infos[0];

        for (int i = 0; i < dir->nInfos - 1; i++) { dir->infos[i] = dir->infos[i + 1]; }
        for (int i = 0; i < dir->nInfos; i++) { dir->filhos[i] = dir->filhos[i + 1]; }

        dir->filhos[dir->nInfos] = NULL;
        dir->nInfos--;
        filho->nInfos++;
        return;
    }

    /* 3. MERGE COM ESQUERDA */
    if (esq != NULL)
    {
        if (esq->nInfos + filho->nInfos + 1 > 4) { return; }

        int pos = esq->nInfos;
        esq->infos[pos] = pai->infos[indiceFilho - 1];

        for (int i = 0; i < filho->nInfos; i++) { esq->infos[pos + 1 + i] = filho->infos[i]; }
        for (int i = 0; i <= filho->nInfos; i++) { esq->filhos[pos + 1 + i] = filho->filhos[i]; }

        esq->nInfos += filho->nInfos + 1;

        for (int i = indiceFilho - 1; i < pai->nInfos - 1; i++)
        {
            pai->infos[i] = pai->infos[i + 1];
            pai->filhos[i + 1] = pai->filhos[i + 2];
        }
        pai->filhos[pai->nInfos] = NULL;
        pai->nInfos--;
        free(filho);
        return;
    }

    /* 4. MERGE COM DIREITA */
    if (dir != NULL)
    {
        if (filho->nInfos + dir->nInfos + 1 > 4) { return; }

        int pos = filho->nInfos;
        filho->infos[pos] = pai->infos[indiceFilho];

        for (int i = 0; i < dir->nInfos; i++) { filho->infos[pos + 1 + i] = dir->infos[i]; }
        for (int i = 0; i <= dir->nInfos; i++) { filho->filhos[pos + 1 + i] = dir->filhos[i]; }

        filho->nInfos += dir->nInfos + 1;

        for (int i = indiceFilho; i < pai->nInfos - 1; i++)
        {
            pai->infos[i] = pai->infos[i + 1];
            pai->filhos[i + 1] = pai->filhos[i + 2];
        }
        pai->filhos[pai->nInfos] = NULL;
        pai->nInfos--;
        free(dir);
    }
}

int removerDaArvore(Arv45* no, int bloco_inicio, int ehRaiz)
{
    int underflowAtual = 0;

    if (no != NULL)
    {
        // DRY: Usando encontrarPosicao
        int i = encontrarPosicao(no, bloco_inicio);

        /* Encontrou a chave */
        if (i < no->nInfos && no->infos[i].bloco_inicio == bloco_inicio)
        {
            /* Caso 1: folha */
            if (ehFolha45(no))
            {
                for (int j = i; j < no->nInfos - 1; j++)
                {
                    no->infos[j] = no->infos[j + 1];
                }
                no->nInfos--;

                if (!ehRaiz) { underflowAtual = (no->nInfos < 2); }
            }
            else
            {
                /* Caso 2: nó interno */
                BlocoMemoria predecessor = obterPredecessor(no->filhos[i]);
                no->infos[i] = predecessor;

                int underflow = removerDaArvore(no->filhos[i], predecessor.bloco_inicio, 0);
                if (underflow) { tratarUnderflow(no, i); }
            }
        }
        else
        {
            /* Não encontrou */
            if (!ehFolha45(no))
            {
                int underflow = removerDaArvore(no->filhos[i], bloco_inicio, 0);
                if (underflow) { tratarUnderflow(no, i); }
            }
        }

        /* A raiz pode ficar com apenas 1 informação */
        if (!ehRaiz) { underflowAtual = (no->nInfos < 2); }
        else { underflowAtual = 0; }
    }

    return underflowAtual;
}

void remover45_root(Arv45** raiz, int bloco_inicio)
{
    if (*raiz != NULL)
    {
        removerDaArvore(*raiz, bloco_inicio, 1);
        
        /* Raiz vazia após merge */
        if (*raiz != NULL && (*raiz)->nInfos == 0)
        {
            Arv45* temp = *raiz;
            if (ehFolha45(*raiz)) { *raiz = NULL; }
            else { *raiz = (*raiz)->filhos[0]; }
            free(temp);
        }
    }
}

// ============================================================================
// MÓDULO ESTRUTURAL: BUSCAS BLINDADAS (SEGURANÇA DE MEMÓRIA)
// ============================================================================
int buscarBlocoPorNumero(Arv45* raiz, int bloco_alvo, BlocoMemoria* saida) {
    int achou = 0;

    if (raiz != NULL)
    {
        int i = 0;
        // Mantemos o while aqui pois ele compara com bloco_fim, o que e diferente do encontrarPosicao
        while (i < raiz->nInfos && bloco_alvo > raiz->infos[i].bloco_fim) i++;

        if (i < raiz->nInfos && bloco_alvo >= raiz->infos[i].bloco_inicio && bloco_alvo <= raiz->infos[i].bloco_fim) {
            *saida = raiz->infos[i];
            achou = 1; 
        }
        else if (!ehFolha45(raiz))
        {
            achou = buscarBlocoPorNumero(raiz->filhos[i], bloco_alvo, saida);
        }
    }
    return achou;
}

int encontrarBlocoLivre(Arv45* raiz, int qtd_blocos, BlocoMemoria* saida) {
    int achou = 0;

    if (raiz != NULL)
    {
        for (int i = 0; i < raiz->nInfos && !achou; i++) {
            if (encontrarBlocoLivre(raiz->filhos[i], qtd_blocos, saida))
            {
                achou = 1;
            }
            else
            {
                int tamanho_atual = (raiz->infos[i].bloco_fim - raiz->infos[i].bloco_inicio) + 1;
                if (raiz->infos[i].status == 'L' && tamanho_atual >= qtd_blocos) {
                    *saida = raiz->infos[i];
                    achou = 1; 
                }
            }
        }

        if (!achou)
        {
            achou = encontrarBlocoLivre(raiz->filhos[raiz->nInfos], qtd_blocos, saida);
        }
    }
    return achou;
}

// ============================================================================
// REGRAS DE NEGÓCIO E GERENCIAMENTO DE MEMÓRIA (REQUISITOS A, B, C)
// ============================================================================
void cadastrarMemoriaInicial(Arv45** raiz, unsigned long total_memoria_bytes)
{
    BlocoMemoria novo;
    char status;
    unsigned long end_ini, end_fim;
    int bloco_atual = 0;
    int encerrado = 0;

    printf("\n=== CONFIGURACAO INICIAL DA MEMORIA ===\n");
    printf("O primeiro no e Livre (L) ou Ocupado (O)? ");
    scanf(" %c", &status);

    while (status != 'L' && status != 'O')
    {
        printf("[ERRO] Digite apenas L ou O: ");
        scanf(" %c", &status);
    }

    printf("Endereco inicial (geralmente 0): ");
    scanf("%lu", &end_ini);
    printf("Endereco final do primeiro no: ");
    scanf("%lu", &end_fim);

    while (!encerrado)
    {
        if (end_ini > end_fim)
        {
            printf("\n[ERRO] Endereco inicial maior que o endereco final.\n");
            encerrado = 1;
        }

        if (!encerrado)
        {
            if (end_ini >= total_memoria_bytes)
            {
                printf("\n[ERRO] Endereco inicial fora da memoria.\n");
                encerrado = 1;
            }
        }

        if (!encerrado)
        {
            if (end_fim >= total_memoria_bytes)
            {
                printf("\n[AVISO] Endereco final ajustado para o limite da memoria.\n");
                end_fim = total_memoria_bytes - 1;
            }

            unsigned long tamanho = end_fim - end_ini + 1;
            int qtd_blocos = (int)((tamanho + BLOCO_TAM - 1) / BLOCO_TAM);

            if (qtd_blocos <= 0)
            {
                printf("\n[ERRO] Particao invalida.\n");
                encerrado = 1;
            }
            else
            {
                novo.status = status;
                novo.endereco_inicio = end_ini;
                novo.endereco_fim = end_fim;
                novo.bloco_inicio = bloco_atual;
                novo.bloco_fim = bloco_atual + qtd_blocos - 1;

                inserir45_root(raiz, novo);
                bloco_atual = novo.bloco_fim + 1;

                if (end_fim >= total_memoria_bytes - 1)
                {
                    encerrado = 1;
                }
                else
                {
                    status = (status == 'L') ? 'O' : 'L';
                    end_ini = end_fim + 1;

                    printf("\nProximo no interpretado como [%c].\n", status);
                    printf("Informe o endereco final para este no (Max: %lu): ", total_memoria_bytes - 1);
                    scanf("%lu", &end_fim);
                }
            }
        }
    }

    if (end_ini <= end_fim && end_ini < total_memoria_bytes)
    {
        printf("\n[OK] Memoria cadastrada com sucesso!\n");
    }
}

void alocarMemoria45(Arv45** raiz, int qtd_blocos) {
    int podeAlocar = 1;
    BlocoMemoria alvo;

    if (!encontrarBlocoLivre(*raiz, qtd_blocos, &alvo)) {
        printf("\n[ERRO] Nao ha memoria contigua suficiente para %d blocos!\n", qtd_blocos);
        podeAlocar = 0;
    }

    if (podeAlocar)
    {
        remover45_root(raiz, alvo.bloco_inicio);

        int blocos_totais = (alvo.bloco_fim - alvo.bloco_inicio) + 1;
        BlocoMemoria alocado = alvo;
        alocado.status = 'O';

        if (blocos_totais > qtd_blocos) {
            alocado.bloco_fim = alvo.bloco_inicio + qtd_blocos - 1;
            alocado.endereco_fim = alocado.endereco_inicio + (qtd_blocos * (unsigned long)BLOCO_TAM) - 1;

            BlocoMemoria restoLivre;
            restoLivre.status = 'L';
            restoLivre.bloco_inicio = alocado.bloco_fim + 1;
            restoLivre.bloco_fim = alvo.bloco_fim;
            restoLivre.endereco_inicio = alocado.endereco_fim + 1;
            restoLivre.endereco_fim = alvo.endereco_fim;

            inserir45_root(raiz, restoLivre); 
        }

        BlocoMemoria vizinho_esq;
        if (buscarBlocoPorNumero(*raiz, alocado.bloco_inicio - 1, &vizinho_esq) && vizinho_esq.status == 'O') {
            remover45_root(raiz, vizinho_esq.bloco_inicio);
            alocado.bloco_inicio = vizinho_esq.bloco_inicio;
            alocado.endereco_inicio = vizinho_esq.endereco_inicio;
        }

        BlocoMemoria vizinho_dir;
        if (buscarBlocoPorNumero(*raiz, alocado.bloco_fim + 1, &vizinho_dir) && vizinho_dir.status == 'O') {
            remover45_root(raiz, vizinho_dir.bloco_inicio);
            alocado.bloco_fim = vizinho_dir.bloco_fim;
            alocado.endereco_fim = vizinho_dir.endereco_fim;
        }

        inserir45_root(raiz, alocado);
        printf("\n[OK] %d blocos alocados com sucesso e arvore ajustada!\n", qtd_blocos);
    }
}

void liberarMemoria45(Arv45** raiz, int bloco_ini) {
    BlocoMemoria atual;
    int podeLiberar = 1;
    
    if (!buscarBlocoPorNumero(*raiz, bloco_ini, &atual) || atual.status == 'L') {
        printf("\n[ERRO] Bloco ocupado nao encontrado ou ja esta livre.\n");
        podeLiberar = 0;
    }

    if (podeLiberar)
    {
        remover45_root(raiz, atual.bloco_inicio);
        atual.status = 'L'; 

        BlocoMemoria vizinho_esq;
        if (buscarBlocoPorNumero(*raiz, atual.bloco_inicio - 1, &vizinho_esq) && vizinho_esq.status == 'L') {
            remover45_root(raiz, vizinho_esq.bloco_inicio); 
            atual.bloco_inicio = vizinho_esq.bloco_inicio;
            atual.endereco_inicio = vizinho_esq.endereco_inicio;
        }

        BlocoMemoria vizinho_dir;
        if (buscarBlocoPorNumero(*raiz, atual.bloco_fim + 1, &vizinho_dir) && vizinho_dir.status == 'L') {
            remover45_root(raiz, vizinho_dir.bloco_inicio); 
            atual.bloco_fim = vizinho_dir.bloco_fim;
            atual.endereco_fim = vizinho_dir.endereco_fim;
        }

        inserir45_root(raiz, atual);
        printf("\n[OK] Memoria liberada e particoes unidas com sucesso!\n");
    }
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

void destruirArvore45(Arv45* raiz)
{
    if (raiz != NULL)
    {
        for (int i = 0; i <= raiz->nInfos; i++)
        {
            destruirArvore45(raiz->filhos[i]);
        }
        free(raiz);
    }
}