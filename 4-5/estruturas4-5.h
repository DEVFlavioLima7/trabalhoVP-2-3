#ifndef ESTRUTURAS45_H
#define ESTRUTURAS45_H

#define BLOCO_TAM (1024 * 1024) // 1 Mbyte em bytes

typedef struct {
    char status;                // 'O' para Ocupado, 'L' para Livre
    int bloco_inicio;
    int bloco_fim;
    unsigned long endereco_inicio; 
    unsigned long endereco_fim;    
} BlocoMemoria;

typedef struct Arv45 {
    BlocoMemoria infos[4];      // Até 4 blocos por nó
    struct Arv45 *filhos[5];    // Até 5 filhos
    int nInfos;
} Arv45;

// ============================================================================
// Módulo Estrutural: Funções Básicas e Auxiliares
// ============================================================================
Arv45* criarNo45(BlocoMemoria info, Arv45* esq, Arv45* direita);
int ehFolha45(Arv45* no);
int encontrarPosicao(Arv45* no, int bloco_inicio);
void destruirArvore45(Arv45* raiz);

// ============================================================================
// Módulo Estrutural: Inserção
// ============================================================================
void adicionarInfoNo45(Arv45* no, BlocoMemoria info, Arv45* filhoDir);
Arv45* quebraNo45(Arv45* no, BlocoMemoria info, BlocoMemoria* promove, Arv45* filhoDir);
Arv45* inserir45(Arv45** raiz, BlocoMemoria info, BlocoMemoria* promove);
void inserir45_root(Arv45** raiz, BlocoMemoria info);

// ============================================================================
// Módulo Estrutural: Remoção
// ============================================================================
BlocoMemoria obterPredecessor(Arv45* no);
void tratarUnderflow(Arv45* pai, int indiceFilho);
int removerDaArvore(Arv45* no, int bloco_inicio, int ehRaiz);
void remover45_root(Arv45** raiz, int bloco_inicio);

// ============================================================================
// Módulo Estrutural: Buscas (Segurança de Memória)
// ============================================================================
int buscarBlocoPorNumero(Arv45* raiz, int bloco_alvo, BlocoMemoria* resultado);
int encontrarBlocoLivre(Arv45* raiz, int qtd_blocos, BlocoMemoria* resultado);

// ============================================================================
// Módulos das Regras de Negócio (Requisitos do Enunciado)
// ============================================================================
void cadastrarMemoriaInicial(Arv45** raiz, unsigned long total_memoria_bytes);
void alocarMemoria45(Arv45** raiz, int qtd_blocos);
void liberarMemoria45(Arv45** raiz, int bloco_ini);
void exibirMapaMemoria(Arv45* raiz);

#endif