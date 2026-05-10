#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas_vp.h"

/* ===================== APOIO ===================== */

RBNode* criar_no(Info info, TipoInfo tipo) {
    RBNode* novo = (RBNode*)malloc(sizeof(RBNode));
    if (novo) {
        novo->info = info;
        novo->tipo = tipo;
        novo->cor = VERMELHO;
        novo->esq = novo->dir = NULL;
    }
    return novo;
}

int obter_chave(Info info, TipoInfo tipo) {
    int chave = -1; 
    
    switch (tipo) {
        case TIPO_ALUNO:      
            chave = info.aluno.matricula;
            break;
            
        case TIPO_CURSO:      
            chave = info.curso.codigo_curso;
            break;
            
        case TIPO_DISCIPLINA: 
            chave = info.disciplina.codigo_disciplina;
            break;
            
    }
    
    return chave; 
}

RBNode* insere_recursivo(RBNode* H, Info info, TipoInfo tipo, int *resp) {
    if (H == NULL) {
        RBNode* novo = criar_no(info, tipo);
        if (novo != NULL) {
            *resp = 1;
            H = novo;
        }
    }

    int valor_novo = obter_chave(info, tipo);
    int valor_atual = obter_chave(H->info, H->tipo);

    if (valor_novo == valor_atual) {
        *resp = 0; 
    } else {
        if (valor_novo < valor_atual)
            H->esq = insere_recursivo(H->esq, info, tipo, resp);
        else
            H->dir = insere_recursivo(H->dir, info, tipo, resp);
    }

    return balancear(H);
}

int inserir_no(RBNode** raiz, Info info, TipoInfo tipo) {
    int resp;
    *raiz = insere_recursivo(*raiz, info, tipo, &resp);
    if (*raiz != NULL)
        (*raiz)->cor = PRETO;
    return resp; 
}

int obter_cor(RBNode* no) {
    return (no == NULL) ? PRETO : no->cor;
}

void trocar_cores(RBNode* H) {
    H->cor = !H->cor;
    if (H->esq != NULL)
        H->esq->cor = !H->esq->cor;
    if (H->dir != NULL)
        H->dir->cor = !H->dir->cor;
}



RBNode* rotar_esquerda(RBNode* A) {
    RBNode* B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    B->cor = A->cor;
    A->cor = VERMELHO;
    return B;
}

RBNode* rotar_direita(RBNode* A) {
    RBNode* B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    B->cor = A->cor;
    A->cor = VERMELHO;
    return B;
}

RBNode* balancear(RBNode* H) {
    // 1. Nó vermelho é sempre filho à esquerda
    if (obter_cor(H->dir) == VERMELHO && obter_cor(H->esq) == PRETO)
        H = rotar_esquerda(H);

    // 2. Filho da esquerda e neto da esquerda são vermelhos
    if (obter_cor(H->esq) == VERMELHO && obter_cor(H->esq->esq) == VERMELHO)
        H = rotar_direita(H);

    // 3. Dois filhos vermelhos: troca cor!
    if (obter_cor(H->esq) == VERMELHO && obter_cor(H->dir) == VERMELHO)
        trocar_cores(H);

    return H;
}


/* ===================== BUSCAS ===================== */

RBNode* buscar_no(RBNode* raiz, int chave, TipoInfo tipo) {
    RBNode* resultado = NULL; // Começa assumindo que não encontrou (NULL)

    if (raiz != NULL) {
        int chave_atual = obter_chave(raiz->info, raiz->tipo);

        if (chave == chave_atual) {
            resultado = raiz; // Achou o nó!
        } 
        else if (chave < chave_atual) {
            resultado = buscar_no(raiz->esq, chave, tipo); // Busca na esquerda
        } 
        else {
            resultado = buscar_no(raiz->dir, chave, tipo); // Busca na direita
        }
    }

    return resultado; // Um único ponto de saída
}
/*============= ALUNO =============*/

int inserirAluno(RBNode** raiz_alunos, int mat, char nome[], int cod_curso, int ano, int sem) {
    int resultado = 0; // Começa como 0 (falha/duplicado) por padrão
   
    // Se a busca retornar NULL, o aluno não existe, então podemos inserir
    if (buscar_no(*raiz_alunos, mat, TIPO_ALUNO) == NULL) {
        Info info;
        info.aluno.matricula = mat;
        strcpy(info.aluno.nome_aluno, nome);
        info.aluno.codigo_curso = cod_curso;
        info.aluno.ano_ingresso = ano;
        info.aluno.semestre_ingresso = sem;

        resultado = inserir_no(raiz_alunos, info, TIPO_ALUNO);
    }

    return resultado; // Único ponto de saída
}

/*============= CURSO =============*/

/* Insere um novo curso na árvore principal */
int inserirCurso(RBNode** raiz, int codigo, char nome[], int blocos, int semanas) {
    int resultado = 0; // Começa como 0 (falha/duplicado) por padrão

    // Verifica se o curso NÃO existe para poder prosseguir
    if (buscar_no(*raiz, codigo, TIPO_CURSO) == NULL) { 
        Info info;
        info.curso.codigo_curso = codigo;
        strcpy(info.curso.nome_curso, nome);
        info.curso.qtd_blocos_curso = blocos;
        info.curso.semanas_disciplina = semanas;
        
        // Todo curso nasce com sua árvore de disciplinas vazia
        info.curso.raiz_disciplinas = NULL; 

        resultado = inserir_no(raiz, info, TIPO_CURSO);
    }

    return resultado; // Único ponto de saída
}

/*============= DISCIPLINA =============*/

// Função auxiliar para validar as regras de inserção de disciplina
int validarRegras(int bloco_disciplina, int qtd_blocos_curso, int carga, int semanas) {
    int status = 0; // Assume que é INVÁLIDO até provar o contrário

    // Lógica:
    // Se o bloco for MENOR que a qtd do curso
    // E o bloco for válido (>= 0)
    // E a carga for múltipla das semanas (resto da divisão == 0)
    // E a carga for válida (> 0)
    if (bloco_disciplina < qtd_blocos_curso && bloco_disciplina >= 0 && carga % semanas == 0 && carga > 0) {
        status = 1; // Tudo certo, passou em todos os testes!
    }

    return status;
}

int inserirDisciplina(RBNode** raiz_disciplinas, int cod_disc, char nome[], int bloco, int carga){
    int resultado = 0; // Começa como 0 (falha/duplicado) por padrão

    // Verifica se a disciplina NÃO existe para poder prosseguir
    if (buscar_no(*raiz_disciplinas, cod_disc, TIPO_DISCIPLINA) == NULL) { 
        Info info;
        info.disciplina.codigo_disciplina = cod_disc;
        strcpy(info.disciplina.nome_disciplina, nome);
        info.disciplina.bloco_disciplina = bloco;
        info.disciplina.carga_horaria = carga;

        resultado = inserir_no(raiz_disciplinas, info, TIPO_DISCIPLINA);
    }

    return resultado; // Único ponto de saída
}

int inserirDisciplinaNoCurso(RBNode* raiz_cursos, int cod_curso, int cod_disc, char nome[], int bloco, int carga) {
    int res = 0;
    
    // 1. Busca o curso
    RBNode* no_curso = buscar_no(raiz_cursos, cod_curso, TIPO_CURSO);
    
    if (no_curso == NULL) {
        res = -1; // Curso não encontrado
    } else {
        // 2. Valida as regras usando a função static acima
        if (!validarRegras(bloco, no_curso->info.curso.qtd_blocos_curso, carga, no_curso->info.curso.semanas_disciplina)) {
            res = -2; // Violação de regras
        } else {
            // 3. Verifica duplicata na árvore interna de disciplinas
            if (buscar_no(no_curso->info.curso.raiz_disciplinas, cod_disc, TIPO_DISCIPLINA) != NULL) {
                res = 0; // Já existe
            } else {
                // 4. Prepara a inserção
                Info info_disc;
                info_disc.disciplina.codigo_disciplina = cod_disc;
                strcpy(info_disc.disciplina.nome_disciplina, nome);
                info_disc.disciplina.bloco_disciplina = bloco;
                info_disc.disciplina.carga_horaria = carga;

                res = inserir_no(&(no_curso->info.curso.raiz_disciplinas), info_disc, TIPO_DISCIPLINA);
            }
        }
    }

    return res;
}