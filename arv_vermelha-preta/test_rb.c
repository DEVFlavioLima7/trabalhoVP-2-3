#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "estruturas_vp.h"

// Função auxiliar para resgatar a chave de um nó
int get_key(RBNode* node, TipoInfo tipo) {
    if (node == NULL) return -1;
    if (tipo == TIPO_ALUNO) return node->info.aluno.matricula;
    if (tipo == TIPO_CURSO) return node->info.curso.codigo_curso;
    if (tipo == TIPO_DISCIPLINA) return node->info.disciplina.codigo_disciplina;
    return -1;
}

// 1. Verifica Propriedade BST (Binária de Busca)
int is_bst(RBNode* node, int min, int max, TipoInfo tipo) {
    if (node == NULL) return 1;
    int key = get_key(node, tipo);
    if (key <= min || key >= max) return 0; // Violou BST
    return is_bst(node->esq, min, key, tipo) && is_bst(node->dir, key, max, tipo);
}

// 2. Verifica cores consecutivas vermelhas
int has_no_consecutive_reds(RBNode* node) {
    if (node == NULL) return 1;
    if (node->cor == VERMELHO) {
        if ((node->esq && node->esq->cor == VERMELHO) || 
            (node->dir && node->dir->cor == VERMELHO)) {
            return 0; // Dois vermelhos seguidos
        }
    }
    return has_no_consecutive_reds(node->esq) && has_no_consecutive_reds(node->dir);
}

// 3. Verifica LLRB (Left-Leaning): Não pode haver nó vermelho na direita!
int is_left_leaning(RBNode* node) {
    if (node == NULL) return 1;
    if (node->dir && node->dir->cor == VERMELHO) {
        return 0; // Filho direito vermelho (Proibido pós-balanceamento na LLRB)
    }
    return is_left_leaning(node->esq) && is_left_leaning(node->dir);
}

// 4. Verifica altura negra uniforme
int get_black_height(RBNode* node) {
    if (node == NULL) return 1; // Folhas nulas são pretas
    
    int left_bh = get_black_height(node->esq);
    int right_bh = get_black_height(node->dir);
    
    if (left_bh == -1 || right_bh == -1 || left_bh != right_bh) {
        return -1; // Altura diferente!
    }
    
    return left_bh + (node->cor == PRETO ? 1 : 0);
}

// Validação Completa
int validar_arvore_rb(RBNode* raiz, TipoInfo tipo) {
    if (raiz == NULL) return 1;
    
    if (raiz->cor == VERMELHO) {
        printf("  [ERRO] Raiz nao e preta!\n");
        return 0;
    }
    
    if (!is_bst(raiz, -2147483647, 2147483647, tipo)) {
        printf("  [ERRO] A arvore perdeu a propriedade BST (desordenada)!\n");
        return 0;
    }
    
    if (!has_no_consecutive_reds(raiz)) {
        printf("  [ERRO] Nos vermelhos consecutivos encontrados!\n");
        return 0;
    }
    
    if (!is_left_leaning(raiz)) {
        printf("  [ERRO] No filho a direita vermelho detectado (LLRB violada)!\n");
        return 0;
    }
    
    if (get_black_height(raiz) == -1) {
        printf("  [ERRO] Caminhos com diferentes quantidades de nos pretos (Altura Negra invalida)!\n");
        return 0;
    }
    
    return 1;
}

// Imprimir Arvore Desenhada Visualmente com Conexoes
void imprimir_arvore_visual(RBNode* raiz, TipoInfo tipo, int espaco, int direcao) {
    if (raiz == NULL) return;
    
    int espaco_count = 6;
    espaco += espaco_count;
    
    // Imprime sub-arvore da direita
    imprimir_arvore_visual(raiz->dir, tipo, espaco, 1);
    
    printf("\n");
    for (int i = espaco_count; i < espaco; i++) {
        printf(" ");
    }
    
    int key = get_key(raiz, tipo);
    char cor = (raiz->cor == PRETO) ? 'P' : 'V';
    
    if (direcao == 1) printf("/"); // Filho da direita
    else if (direcao == -1) printf("\\"); // Filho da esquerda
    else printf("-"); // Raiz
    
    printf("--");
    
    if (cor == 'P') printf("\033[1;30m%d(%c)\033[0m", key, cor); 
    else printf("\033[1;31m%d(%c)\033[0m", key, cor); 
    
    // Imprime sub-arvore da esquerda
    imprimir_arvore_visual(raiz->esq, tipo, espaco, -1);
}



// Função auxiliar para contar o tamanho real da árvore
int contar_nos(RBNode* raiz) {
    if (raiz == NULL) return 0;
    return 1 + contar_nos(raiz->esq) + contar_nos(raiz->dir);
}


int main() {
    srand((unsigned int)time(NULL));
    RBNode* raiz_alunos = NULL;
    
    int qtd_insercoes = 1000;
    int chaves_inseridas[qtd_insercoes];
    int count = 0;
    
    printf("\n=======================================================\n");
    printf("   INICIANDO TESTES AUTOMATIZADOS - ARVORE RUBRO NEGRA \n");
    printf("=======================================================\n\n");
    
    printf(">> 1. Inserindo %d alunos aleatorios...\n", qtd_insercoes);
    for (int i = 0; i < qtd_insercoes; i++) {
        int mat = (rand() % 90000) + 10000; // Matriculas entre 10000 e 99999
        int res = inserirAluno(&raiz_alunos, mat, "Aluno Teste", 1, 2026, 1);
        
        if (res == 1) { // Só guarda se foi inserido com sucesso (não era duplicata)
            chaves_inseridas[count++] = mat;
        }
    }
    printf("   [OK] Foram inseridas %d chaves UNICAS na arvore.\n", count);
    
    int total_nos = contar_nos(raiz_alunos);
    if (total_nos != count) {
        printf("   [ERRO FATAL] Quantidade de nos reais (%d) diverge dos inseridos (%d)!\n", total_nos, count);
        return 1;
    } else {
        printf("   [OK] Contagem de nos da arvore (%d) confere.\n", total_nos);
    }

    printf("\n>> 2. Validando integridade estrutural da Arvore (Regras LLRB)...\n");
    if (validar_arvore_rb(raiz_alunos, TIPO_ALUNO)) {
        printf("   [OK] A arvore passou em TODAS as propriedades Rubro-Negras!\n");
        printf("        - Raiz e Preta\n");
        printf("        - Ordem da Busca Binaria\n");
        printf("        - Sem dois vermelhos seguidos\n");
        printf("        - Todos os caminhos tem a mesma altura preta\n");
        printf("        - Todos os nos vermelhos pendem para a esquerda (LLRB)\n");
    } else {
        printf("   [ERRO FATAL] A arvore quebrou as regras Rubro-Negras!\n");
        return 1;
    }
    
    printf("\n>> 3. Validando BLINDAGEM contra chaves duplicadas...\n");
    int sucesso_blindagem = 1;
    for (int i = 0; i < 50; i++) { // Tentando inserir as primeiras 50 que já sabemos que estão na árvore
        int res = inserirAluno(&raiz_alunos, chaves_inseridas[i], "Aluno Clone", 1, 2026, 1);
        if (res != 0) { // Deveria retornar 0 (falha por já existir)
            sucesso_blindagem = 0;
            printf("   [ERRO FATAL] Permitiu inserir a matricula duplicada %d!\n", chaves_inseridas[i]);
            break;
        }
    }
    
    if (sucesso_blindagem) {
        printf("   [OK] Sistema rejeitou todas as insercoes duplicadas perfeitamente.\n");
    }
    
    if (contar_nos(raiz_alunos) != count) {
        printf("   [ERRO FATAL] O tamanho da arvore foi alterado pelas duplicatas escondidas!\n");
        return 1;
    }
    
    printf("\n>> 4. Gerando teste visual para uma arvore pequena (10 elementos)...\n");
    RBNode* arvore_pequena = NULL;
    int vetor[10] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 90};
    for(int i=0; i<10; i++) {
        inserirAluno(&arvore_pequena, vetor[i], "T", 1, 2026, 1);
    }
    printf("   --- Arvore LLRB Gerada (P=Preto, V=Vermelho) ---\n\n");
    imprimir_arvore_visual(arvore_pequena, TIPO_ALUNO, 0, 0);
    printf("\n   ------------------------------------------------\n");
    
    printf("\n>> SUCESSO ABSOLUTO! O TAD da Arvore Vermelho-Preta esta impecavel.\n\n");
    
    return 0;
}
