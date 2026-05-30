#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas_vp.h"

/* ============================================================
   FUNÇÕES DE APOIO E MANIPULAÇÃO DA ÁRVORE (GENÉRICAS)
   ============================================================ */

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

int obter_cor(RBNode* no) {
    int cor = PRETO;
    if (no != NULL) cor = no->cor;
    return cor;
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
    if (obter_cor(H->dir) == VERMELHO && obter_cor(H->esq) == PRETO)
        H = rotar_esquerda(H);

    if (obter_cor(H->esq) == VERMELHO && obter_cor(H->esq->esq) == VERMELHO)
        H = rotar_direita(H);

    if (obter_cor(H->esq) == VERMELHO && obter_cor(H->dir) == VERMELHO)
        trocar_cores(H);

    return H;
}

RBNode* insere_recursivo(RBNode* H, Info info, TipoInfo tipo, int *resp) {
    if (H == NULL) {
        H = criar_no(info, tipo);
        if (H != NULL) *resp = 1;
        else *resp = 0;
    } else {
        int valor_novo = obter_chave(info, tipo);
        int valor_atual = obter_chave(H->info, H->tipo);

        if (valor_novo < valor_atual)
            H->esq = insere_recursivo(H->esq, info, tipo, resp);
        else if (valor_novo > valor_atual)
            H->dir = insere_recursivo(H->dir, info, tipo, resp);
        else
            *resp = 0;

        H = balancear(H);
    }
    return H;
}

int inserir_no(RBNode** raiz, Info info, TipoInfo tipo) {
    int resp;
    *raiz = insere_recursivo(*raiz, info, tipo, &resp);
    if (*raiz != NULL)
        (*raiz)->cor = PRETO;
    return resp;
}

RBNode* buscar_no(RBNode* raiz, int chave, TipoInfo tipo) {
    RBNode* res = NULL;
    if (raiz != NULL) {
        int chave_atual = obter_chave(raiz->info, raiz->tipo);
        if (chave == chave_atual)
            res = raiz;
        else if (chave < chave_atual)
            res = buscar_no(raiz->esq, chave, tipo);
        else
            res = buscar_no(raiz->dir, chave, tipo);
    }
    return res;
}

/* ============================================================
   FUNÇÕES ESPECÍFICAS (ALUNO, CURSO, DISCIPLINA)
   ============================================================ */

int inserirAluno(RBNode** raiz_alunos, int mat, char nome[], int cod_curso, int ano, int sem) {
    int res = 0;
    if (buscar_no(*raiz_alunos, mat, TIPO_ALUNO) == NULL) {
        Info info;
        info.aluno.matricula = mat;
        strcpy(info.aluno.nome_aluno, nome);
        info.aluno.codigo_curso = cod_curso;
        info.aluno.ano_ingresso = ano;
        info.aluno.semestre_ingresso = sem;
        res = inserir_no(raiz_alunos, info, TIPO_ALUNO);
    }
    return res;
}

int inserirCurso(RBNode** raiz, int codigo, char nome[], int blocos, int semanas) {
    int res = 0;
    if (buscar_no(*raiz, codigo, TIPO_CURSO) == NULL) {
        Info info;
        info.curso.codigo_curso = codigo;
        strcpy(info.curso.nome_curso, nome);
        info.curso.qtd_blocos_curso = blocos;
        info.curso.semanas_disciplina = semanas;
        info.curso.raiz_disciplinas = NULL; 
        res = inserir_no(raiz, info, TIPO_CURSO);
    }
    return res;
}

int validarRegras(int bloco_disciplina, int qtd_blocos_curso, int carga, int semanas) {
    int status = 0;
    if (bloco_disciplina < qtd_blocos_curso && bloco_disciplina >= 0 && 
        carga % semanas == 0 && carga > 0) {
        status = 1;
    }
    return status;
}

int inserirDisciplinaNoCurso(RBNode* raiz_cursos, int cod_curso, int cod_disc, char nome[], int bloco, int carga) {
    int res = 0;
    RBNode* no_curso = buscar_no(raiz_cursos, cod_curso, TIPO_CURSO);
    
    if (no_curso == NULL) {
        res = -1; 
    } else if (!validarRegras(bloco, no_curso->info.curso.qtd_blocos_curso, carga, no_curso->info.curso.semanas_disciplina)) {
        res = -2;
    } else if (buscar_no(no_curso->info.curso.raiz_disciplinas, cod_disc, TIPO_DISCIPLINA) == NULL) {
        Info info_disc;
        info_disc.disciplina.codigo_disciplina = cod_disc;
        strcpy(info_disc.disciplina.nome_disciplina, nome);
        info_disc.disciplina.bloco_disciplina = bloco;
        info_disc.disciplina.carga_horaria = carga;
        res = inserir_no(&(no_curso->info.curso.raiz_disciplinas), info_disc, TIPO_DISCIPLINA);
    }
    
    return res;
}

/* --- FUNÇÕES ESPECÍFICAS DE IMPRESSÃO --- */

// 1. Especializada em Disciplinas (imprime sem mergulhar mais, pois é o nível folha)
void imprimirDisciplinas(RBNode* raiz) {
    if (raiz != NULL) {
        imprimirDisciplinas(raiz->esq);
        printf("\n  -> [DISCIPLINA] ID: %d | Nome: %s | Bloco: %d | Carga: %dh", 
                raiz->info.disciplina.codigo_disciplina, 
                raiz->info.disciplina.nome_disciplina, 
                raiz->info.disciplina.bloco_disciplina, 
                raiz->info.disciplina.carga_horaria);
        imprimirDisciplinas(raiz->dir);
    }
}

// 2. Especializada em Cursos (imprime o curso e chama a de disciplinas)
void imprimirCursos(RBNode* raiz) {
    if (raiz != NULL) {
        imprimirCursos(raiz->esq);
        printf("\n[CURSO] ID: %d | Nome: %s | Blocos: %d\n", 
                raiz->info.curso.codigo_curso, 
                raiz->info.curso.nome_curso, 
                raiz->info.curso.qtd_blocos_curso);
        printf("  -------------------------------------------");
        
        if (raiz->info.curso.raiz_disciplinas == NULL) {
            printf("\n  (Nenhuma disciplina cadastrada neste curso)\n");
        } else {
            imprimirDisciplinas(raiz->info.curso.raiz_disciplinas);
        }
        
        printf("\n  -------------------------------------------\n");
        imprimirCursos(raiz->dir);
    }
}

// 3. Especializada em Alunos
void imprimirAlunos(RBNode* raiz) {
    if (raiz != NULL) {
        imprimirAlunos(raiz->esq);
        printf("\n[ALUNO] Mat: %d | Nome: %s | Curso: %d | Ingresso: %d/%d", 
                raiz->info.aluno.matricula, 
                raiz->info.aluno.nome_aluno, 
                raiz->info.aluno.codigo_curso, 
                raiz->info.aluno.ano_ingresso, 
                raiz->info.aluno.semestre_ingresso);
        imprimirAlunos(raiz->dir);
    }
}


void listarAlunosPorCurso(RBNode* raiz, int cod_curso) {
    if (raiz != NULL) {
        // Visita a sub-árvore esquerda
        listarAlunosPorCurso(raiz->esq, cod_curso);
        
        // Processa o nó atual: verifica se é aluno e se o curso bate
        if (raiz->info.tipo == TIPO_ALUNO && raiz->info.dado.aluno.codigocurso == cod_curso) {
            printf("Matricula: %d | Nome: %s\n", 
                   raiz->info.dado.aluno.matriculaaluno, 
                   raiz->info.dado.aluno.nomealuno);
        }
        
        // Visita a sub-árvore direita
        listarAlunosPorCurso(raiz->dir, cod_curso);
    }
}

// 2. Listar alunos de um curso que entraram em um determinado ano
void listarAlunosPorCursoEAno(RBNode* raiz, int cod_curso, int ano) {
    if (raiz != NULL) {
        // Visita a sub-árvore esquerda
        listarAlunosPorCursoEAno(raiz->esq, cod_curso, ano);
        
        // Processa o nó atual: verifica as três condições
        if (raiz->info.tipo == TIPO_ALUNO && 
            raiz->info.dado.aluno.codigocurso == cod_curso && 
            raiz->info.dado.aluno.ano_ingresso == ano) {
            printf("Matricula: %d | Nome: %s | Ano: %d\n", 
                   raiz->info.dado.aluno.matriculaaluno, 
                   raiz->info.dado.aluno.nomealuno, 
                   raiz->info.dado.aluno.ano_ingresso);
        }
        
        // Visita a sub-árvore direita
        listarAlunosPorCursoEAno(raiz->dir, cod_curso, ano);
    }
}

// 3. Contar quantos alunos pertencem a um determinado curso (Ponto único de saída)
int contarAlunosNoCurso(RBNode* raiz, int cod_curso) {
    int total = 0;

    if (raiz != NULL) {
        int atual = 0;

        // Se o nó atual for do aluno e do curso procurado, conta 1
        if (raiz->info.tipo == TIPO_ALUNO && raiz->info.dado.aluno.codigocurso == cod_curso) {
            atual = 1;
        }

        // Soma o atual com o que encontrar na esquerda e na direita
        total = atual + contarAlunosNoCurso(raiz->esq, cod_curso) + 
                       contarAlunosNoCurso(raiz->dir, cod_curso);
    }

    return total;
}

void imprimirDisciplinasDoCurso(rb_node* raiz_cursos, int codigo_curso) {
    rb_node* curso = buscar_no(raiz_cursos, codigo_curso, TIPO_CURSO);
    
    if (curso != NULL) {
        printf("\n--- Disciplinas do Curso: %s ---\n", curso->info.curso.nome_curso);
        
        if (curso->info.curso.raiz_disciplinas != NULL) {
            imprimirDisciplinas(curso->info.curso.raiz_disciplinas);
            printf("\n");
        } else {
            printf("  (Nenhuma disciplina cadastrada neste curso)\n");
        }
    } else {
        printf("\n[ERRO] Curso %d nao encontrado.\n", codigo_curso);
    }
}

// (8) Imprimir os dados de uma disciplina dado o código dela e do curso
void imprimirDadosDisciplina(rb_node* raiz_cursos, int codigo_curso, int codigo_disciplina) {
    rb_node* curso = buscar_no(raiz_cursos, codigo_curso, TIPO_CURSO);
    
    if (curso != NULL) {
        // Aproveitamos a função genérica de busca, passando a raiz de disciplinas do curso!
        rb_node* disc = buscar_no(curso->info.curso.raiz_disciplinas, codigo_disciplina, TIPO_DISCIPLINA);
        
        if (disc != NULL) {
            printf("\n[DISCIPLINA] ID: %d | Nome: %s | Bloco: %d | Carga: %dh\n",
                   disc->info.disciplina.codigo_disciplina,
                   disc->info.disciplina.nome_disciplina,
                   disc->info.disciplina.bloco_disciplina,
                   disc->info.disciplina.carga_horaria);
        } else {
            printf("\n[ERRO] Disciplina %d nao encontrada no curso %d.\n", codigo_disciplina, codigo_curso);
        }
    } else {
        printf("\n[ERRO] Curso %d nao encontrado.\n", codigo_curso);
    }
}

// (9) Função Auxiliar: Imprimir disciplinas de um determinado bloco
void imprimirDisciplinasPorBlocoRec(rb_node* raiz_disc, int bloco_ref, int* encontrou) {
    if (raiz_disc != NULL) {
        imprimirDisciplinasPorBlocoRec(raiz_disc->esq, bloco_ref, encontrou);
        
        if (raiz_disc->info.disciplina.bloco_disciplina == bloco_ref) {
            printf("\n  -> [DISCIPLINA] ID: %d | Nome: %s | Carga: %dh",
                   raiz_disc->info.disciplina.codigo_disciplina,
                   raiz_disc->info.disciplina.nome_disciplina,
                   raiz_disc->info.disciplina.carga_horaria);
            *encontrou = 1;
        }
        
        imprimirDisciplinasPorBlocoRec(raiz_disc->dir, bloco_ref, encontrou);
    }
}

// (9) Função Principal
void imprimirDisciplinasPorBloco(rb_node* raiz_cursos, int codigo_curso, int bloco) {
    rb_node* curso = buscar_no(raiz_cursos, codigo_curso, TIPO_CURSO);

    if (curso != NULL) {
        // Validação: Verifica se o bloco é maior que o limite do curso ou menor/igual a zero.
        // (Nota: Ajuste para "bloco >= curso->..." caso seus blocos comecem no número 0 em vez de 1)
        if (bloco >= curso->info.curso.qtd_blocos_curso || bloco < 0) {
            printf("\n[ERRO] Bloco %d invalido!\n", bloco);
            printf("O curso %s possui o limite de %d bloco(s).\n", 
                   curso->info.curso.nome_curso, 
                   curso->info.curso.qtd_blocos_curso);
        } 
        // Se o bloco for válido, faz a busca normalmente
        else {
            printf("\n--- Disciplinas do Bloco %d (Curso %d) ---", bloco, codigo_curso);

            int achou = 0;
            imprimirDisciplinasPorBlocoRec(curso->info.curso.raiz_disciplinas, bloco, &achou);

            if (achou == 0) {
                printf("\n  (Nenhuma disciplina encontrada no bloco %d)\n", bloco);
            } else {
                printf("\n");
            }
        }
    } else {
        printf("\n[ERRO] Curso %d nao encontrado.\n", codigo_curso);
    }
}

void imprimirDisciplinasDeUmBloco(rb_node* raiz_cursos, int codigo_curso, int bloco) {
    rb_node* curso = buscar_no(raiz_cursos, codigo_curso, TIPO_CURSO);
    
    if (curso != NULL) {
        printf("\n--- Disciplinas do Bloco %d (Curso %d) ---", bloco, codigo_curso);
        
        int achou = 0;
        imprimirDisciplinasPorBlocoRec(curso->info.curso.raiz_disciplinas, bloco, &achou);
        
        if (achou == 0) {
            printf("\n  (Nenhuma disciplina encontrada no bloco %d)\n", bloco);
        } else {
            printf("\n");
        }
    } else {
        printf("\n[ERRO] Curso %d nao encontrado.\n", codigo_curso);
    }
}

// (10) Função Auxiliar e Principal: Imprimir disciplinas com a mesma carga horária
void imprimirDisciplinasPorCHRec(rb_node* raiz_disc, int ch_ref, int* encontrou) {
    if (raiz_disc != NULL) {
        imprimirDisciplinasPorCHRec(raiz_disc->esq, ch_ref, encontrou);
        
        if (raiz_disc->info.disciplina.carga_horaria == ch_ref) {
            printf("\n  -> [DISCIPLINA] ID: %d | Nome: %s | Bloco: %d",
                   raiz_disc->info.disciplina.codigo_disciplina,
                   raiz_disc->info.disciplina.nome_disciplina,
                   raiz_disc->info.disciplina.bloco_disciplina);
            *encontrou = 1;
        }
        
        imprimirDisciplinasPorCHRec(raiz_disc->dir, ch_ref, encontrou);
    }
}

void imprimirDisciplinasPorCargaHoraria(rb_node* raiz_cursos, int codigo_curso, int ch) {
    rb_node* curso = buscar_no(raiz_cursos, codigo_curso, TIPO_CURSO);
    
    if (curso != NULL) {
        printf("\n--- Disciplinas com Carga Horaria de %dh (Curso %d) ---", ch, codigo_curso);
        
        int achou = 0;
        imprimirDisciplinasPorCHRec(curso->info.curso.raiz_disciplinas, ch, &achou);
        
        if (achou == 0) {
            printf("\n  (Nenhuma disciplina com %dh encontrada)\n", ch);
        } else {
            printf("\n");
        }
    } else {
        printf("\n[ERRO] Curso %d nao encontrado.\n", codigo_curso);
    }
}

/*Função de Exclusão: */

rb_node* remover_no_disciplina(rb_node* raiz, int cod_disc) {
    rb_node* resultado = NULL;

    if (cod_disc < raiz->info.disciplina.codigo_disciplina) {
        if (obter_cor(raiz->esq) == PRETO && raiz->esq != NULL && obter_cor(raiz->esq->esq) == PRETO) {
            raiz = moverRedEsq(raiz);
        }
        raiz->esq = remover_no_disciplina(raiz->esq, cod_disc);
        resultado = balancear(raiz);
    } else {
        if (obter_cor(raiz->esq) == VERMELHO) {
            raiz = rotar_direita(raiz);
        }
        
        // Nó encontrado e é uma folha
        if (cod_disc == raiz->info.disciplina.codigo_disciplina && raiz->dir == NULL) {
            free(raiz);
            resultado = NULL; // Única saída para este caso
        } else {
            if (obter_cor(raiz->dir) == PRETO && raiz->dir != NULL && obter_cor(raiz->dir->esq) == PRETO) {
                raiz = moverRedDir(raiz);
            }
            
            // Nó encontrado, mas tem filhos à direita
            if (cod_disc == raiz->info.disciplina.codigo_disciplina) {
                rb_node* menor = procurarMenor(raiz->dir);
                raiz->info.disciplina = menor->info.disciplina; 
                raiz->dir = removerMenor(raiz->dir);
            } else {
                raiz->dir = remover_no_disciplina(raiz->dir, cod_disc);
            }
            
            resultado = balancear(raiz);
        }
    }
    
    return resultado;
}

int excluirDisciplinaDoCurso(rb_node* raiz_cursos, int cod_curso, int cod_disc) {
    int res = 1; // 1 = Sucesso por padrão
    rb_node* no_curso = buscar_no(raiz_cursos, cod_curso, TIPO_CURSO);

    if (no_curso == NULL) {
        res = -1; // Erro: Curso não encontrado
    } else if (buscar_no(no_curso->info.curso.raiz_disciplinas, cod_disc, TIPO_DISCIPLINA) == NULL) {
        res = -2; // Erro: Disciplina não encontrada neste curso
    } else {
        // Tudo certo para excluir
        no_curso->info.curso.raiz_disciplinas = remover_no_disciplina(no_curso->info.curso.raiz_disciplinas, cod_disc);

        if (no_curso->info.curso.raiz_disciplinas != NULL) {
            no_curso->info.curso.raiz_disciplinas->cor = PRETO; 
        }
    }

    return res;
}

// Função recursiva para remover um nó (curso) da Árvore Rubro-Negra Principal
rb_node* remover_no_curso(rb_node* raiz, int cod_curso) {
    rb_node* resultado = NULL;

    if (raiz == NULL) {
        resultado = NULL;
    } 
    else if (cod_curso < raiz->info.curso.codigo_curso) {
        if (raiz->esq != NULL && obter_cor(raiz->esq) == PRETO && obter_cor(raiz->esq->esq) == PRETO) {
            raiz = moverRedEsq(raiz);
        }
        raiz->esq = remover_no_curso(raiz->esq, cod_curso);
        resultado = balancear(raiz);
    } 
    else {
        if (obter_cor(raiz->esq) == VERMELHO) {
            raiz = rotar_direita(raiz);
        }
        
        // Nó encontrado e é uma folha
        if (cod_curso == raiz->info.curso.codigo_curso && raiz->dir == NULL) {
            free(raiz);
            resultado = NULL; 
        } else {
            if (raiz->dir != NULL && obter_cor(raiz->dir) == PRETO && obter_cor(raiz->dir->esq) == PRETO) {
                raiz = moverRedDir(raiz);
            }
            
            // Nó encontrado, mas tem filhos à direita
            if (cod_curso == raiz->info.curso.codigo_curso) {
                rb_node* menor = procurarMenor(raiz->dir);
                
                // Copia os dados do curso sucessor
                raiz->info.curso = menor->info.curso; 
                
                // Usa a SUA removerMenor original!
                raiz->dir = removerMenor(raiz->dir);
            } else {
                raiz->dir = remover_no_curso(raiz->dir, cod_curso);
            }
            
            resultado = balancear(raiz);
        }
    }
    
    return resultado;
}


int excluirCurso(rb_node** raiz_cursos, int cod_curso) {
    int res = 1; 
    
   
    rb_node* no_curso = buscar_no(*raiz_cursos, cod_curso, TIPO_CURSO);

    if (no_curso == NULL) {
        res = -1; // Erro: Curso não encontrado
    } 
    // Regra de Negócio: O curso tem alguma disciplina pendurada nele?
    else if (no_curso->info.curso.raiz_disciplinas != NULL) {
        res = -2; // Erro: Violação de integridade (Existem disciplinas)
    } 
    else {
        // Tudo certo! O curso existe e está vazio. Podemos deletar.
        *raiz_cursos = remover_no_curso(*raiz_cursos, cod_curso);

        // Garante a raiz preta
        if (*raiz_cursos != NULL) {
            (*raiz_cursos)->cor = PRETO; 
        }
    }

    return res;
}
