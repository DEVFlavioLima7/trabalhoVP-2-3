#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas2-3.h"

/* Função auxiliar para pegar a chave (ID/Matrícula) de forma genérica */
int obterChave23(info_com_tipo info) {
    int chave = -1;
    switch (info.tipo) {
        case TIPO_ALUNO:      
            chave = info.dado.aluno.matricula_aluno; 
            break;
        case TIPO_CURSO:      
            chave = info.dado.curso.codigo_curso; 
            break;
        case TIPO_DISCIPLINA: 
            chave = info.dado.disciplina.codigo_disciplina; 
            break;
    }
    return chave;
}

/* Cria um novo nó da árvore 2-3 */
arv_2_3* criarNo23(info_com_tipo info, arv_2_3* filhoEsq, arv_2_3* filhoCen) {
    arv_2_3* novo = (arv_2_3*)malloc(sizeof(arv_2_3));
    if (novo) {
        novo->info[0] = info;
        novo->n_infos = 1;
        novo->esq = filhoEsq;
        novo->cen = filhoCen;
        novo->dir = NULL;
    }
    return novo;
}

/* Função de Busca (Lógica 2-3: olha info[0], se não achar e tiver info[1], olha info[1]) */
arv_2_3* buscar23(arv_2_3* raiz, int chave) {
    arv_2_3* res = NULL;

    if (raiz != NULL) {
        int chave0 = obterChave23(raiz->info[0]);

        if (chave == chave0) {
            res = raiz;
        } else if (raiz->n_infos == 2 && chave == obterChave23(raiz->info[1])) {
            res = raiz;
        } else {
            // Se não achou no nó, decide para qual filho descer
            if (chave < chave0) {
                res = buscar23(raiz->esq, chave);
            } else if (raiz->n_infos == 1) {
                res = buscar23(raiz->cen, chave);
            } else if (chave < obterChave23(raiz->info[1])) {
                res = buscar23(raiz->cen, chave);
            } else {
                res = buscar23(raiz->dir, chave);
            }
        }
    }
    return res;
}

void adicionarInfoNo(arv_2_3 *no, info_com_tipo novaInfo, arv_2_3 *filhoRelativo) {
    int chaveNova = obterChave23(novaInfo);
    int chaveExistente = obterChave23(no->info[0]);

    if (chaveNova > chaveExistente) {
        // A nova info é maior, vai para a direita (posição 1)
        no->info[1] = novaInfo;
        no->dir = filhoRelativo;
    } else {
        // A nova info é menor, a antiga vai para a direita
        no->info[1] = no->info[0];
        no->info[0] = novaInfo;
        no->dir = no->cen;
        no->cen = filhoRelativo;
    }
    no->n_infos = 2;
}

arv_2_3* quebraNo(arv_2_3 *no, info_com_tipo novaInfo, info_com_tipo *promove, arv_2_3 *filhoRelativo) {
    arv_2_3 *maior;
    int cNova = obterChave23(novaInfo);
    int c0 = obterChave23(no->info[0]);
    int c1 = obterChave23(no->info[1]);

    // Caso 1: A nova info é a menor de todas
    if (cNova < c0) {
        *promove = no->info[0];
        maior = criarNo23(no->info[1], no->cen, no->dir);
        no->info[0] = novaInfo;
        no->cen = filhoRelativo;
    }
    // Caso 2: A nova info é a do meio (ela será promovida)
    else if (cNova < c1) {
        *promove = novaInfo;
        maior = criarNo23(no->info[1], filhoRelativo, no->dir);
    }
    // Caso 3: A nova info é a maior de todas
    else {
        *promove = no->info[1];
        maior = criarNo23(novaInfo, no->dir, filhoRelativo);
    }

    no->n_infos = 1;
    no->dir = NULL; 
    // O nó original volta a ser um nó de 1 info
    return maior;
}

/* Função auxiliar para saber se um nó é folha */
int ehFolha23(arv_2_3 *no) {
    return (no->esq == NULL && no->cen == NULL && no->dir == NULL);
}

/* Função interna recursiva de inserção */
arv_2_3* insere23Recursivo(arv_2_3 *no, info_com_tipo novaInfo, info_com_tipo *promove, int *houvePromocao) {
    arv_2_3 *maiorFilho = NULL;

    // Caso Base: Alcançou o local de inserção (sempre na folha)
    if (ehFolha23(no)) {
        *promove = novaInfo;
        *houvePromocao = 1;
        // Não retornamos aqui. A folha vai absorver a novaInfo ou quebrar logo abaixo.
    } else {
        // Caso contrário, navega pela árvore para achar a folha correta
        int chaveNova = obterChave23(novaInfo);
        int chave0 = obterChave23(no->info[0]);

        if (chaveNova < chave0) {
            maiorFilho = insere23Recursivo(no->esq, novaInfo, promove, houvePromocao);
        } else if (no->n_infos == 1 || chaveNova < obterChave23(no->info[1])) {
            maiorFilho = insere23Recursivo(no->cen, novaInfo, promove, houvePromocao);
        } else {
            maiorFilho = insere23Recursivo(no->dir, novaInfo, promove, houvePromocao);
        }
    }

    // --- VOLTA DA RECURSÃO (Tratamento dos nós superiores) ---
    if (*houvePromocao) {
        // Se o nó atual tem espaço, absorve o elemento promovido ("Onda")
        if (no->n_infos == 1) {
            adicionarInfoNo(no,*promove, maiorFilho);
            *houvePromocao = 0; // Promoção tratada com sucesso
            maiorFilho = NULL;
        } 
        // Se o nó atual estiver cheio, explode ele ("Quebra")
        else {
            info_com_tipo promoveLocal;
            arv_2_3 *novoNo = quebraNo(no, *promove, &promoveLocal, maiorFilho);
            *promove = promoveLocal; // Atualiza o que vai subir para o próximo pai
            maiorFilho = novoNo;    // Passa o novo nó criado no split para o pai segurar
        }
    }

    return maiorFilho;
}

/* Interface pública para inserção (Trata a explosão da raiz se necessário) */
int inserirNo23(arv_2_3 **raiz, info_com_tipo novaInfo) {
    // Evita duplicatas usando a busca que já criamos
    if (buscar23(*raiz, obterChave23(novaInfo)) != NULL) {
        return 0; // Código/Matrícula já existe
    }

    // Se a árvore estiver completamente vazia, cria a raiz inicial
    if (*raiz == NULL) {
        *raiz = criarNo23(novaInfo, NULL, NULL);
        return 1;
    }

    info_com_tipo promove;
    int houvePromocao = 0;
    arv_2_3 *maiorFilho = insere23Recursivo(*raiz, novaInfo, &promove, &houvePromocao);

    // Se a raiz estourou, a árvore cresce 1 nível para cima
    if (houvePromocao) {
        arv_2_3 *novaRaiz = criarNo23(promove, *raiz, maiorFilho);
        *raiz = novaRaiz;
    }

    return 1; // Inserção concluída com sucesso
}

/* ============================================================
   FUNÇÕES ESPECÍFICAS (CURSO, DISCIPLINA, ALUNO) - ÁRVORE 2-3
   ============================================================ */

/* 1. Inserção de um Novo Curso */
int inserirCurso23(arv_2_3** raiz, int codigo, char nome[], int blocos, int semanas) {
    info_com_tipo infoComTipo;
    infoComTipo.tipo = TIPO_CURSO;
    infoComTipo.dado.curso.codigo_curso = codigo;
    strcpy(infoComTipo.dado.curso.nome_curso, nome);
    infoComTipo.dado.curso.qtd_blocos_curso = blocos;
    infoComTipo.dado.curso.semanas_disciplina = semanas;
    infoComTipo.dado.curso.raiz_disciplinas = NULL; // Sub-árvore de disciplinas inicia vazia

    return inserirNo23(raiz, infoComTipo);
}

/* 2. Validação das Regras de Negócio (Item C do enunciado) */
int validarRegras23Detalhe(int bloco_disciplina, int qtd_blocos_curso, int carga, int semanas) {
    int erros = REGRA_OK;

    if (bloco_disciplina < 0 || bloco_disciplina >= qtd_blocos_curso) {
        erros |= REGRA_BLOCO_INVALIDO;
    }

    if (carga <= 0 || carga % semanas != 0) {
        erros |= REGRA_CARGA_INVALIDA;
    }

    return erros;
}

/* 3. Inserção de Disciplina vinculada a um Curso */
int inserirDisciplinaNoCurso23(arv_2_3* raiz_cursos, int cod_curso, int cod_disc, char nome[], int bloco, int carga) {
    int res = 0; // Ponto único de saída
    
    // Busca o curso na árvore 2-3 principal
    arv_2_3* no_curso = buscar23(raiz_cursos, cod_curso);
    
    if (no_curso == NULL) {
        res = -1; // Curso não encontrado
    } else {
        // Como o nó pode ter 1 ou 2 infos, precisamos achar qual delas é o curso alvo
        curso* curso_alvo = NULL;
        if (no_curso->info[0].dado.curso.codigo_curso == cod_curso) {
            curso_alvo = &(no_curso->info[0].dado.curso);
        } else {
            curso_alvo = &(no_curso->info[1].dado.curso);
        }

        // Valida as regras de negócio usando os dados do curso localizado
        if (validarRegras23Detalhe(bloco, curso_alvo->qtd_blocos_curso, carga, curso_alvo->semanas_disciplina) != REGRA_OK) {
            res = -2; // Regras violadas
        } 
        // Verifica se a disciplina já existe dentro da árvore de disciplinas desse curso
        else if (buscar23(curso_alvo->raiz_disciplinas, cod_disc) == NULL) {
            info_com_tipo info_disc;
            info_disc.tipo = TIPO_DISCIPLINA;
            info_disc.dado.disciplina.codigo_disciplina = cod_disc;
            strcpy(info_disc.dado.disciplina.nome_disciplina, nome);
            info_disc.dado.disciplina.bloco_disciplina = bloco;
            info_disc.dado.disciplina.carga_horaria = carga;

            // Insere na sub-árvore 2-3 de disciplinas do curso
            res = inserirNo23(&(curso_alvo->raiz_disciplinas), info_disc);
        }
    }
    
    return res;
}

/* 4. Inserção de um Novo Aluno */
int inserirAluno23(arv_2_3** raiz_alunos, int mat, char nome[], int cod_curso, int ano, int sem) {
    info_com_tipo infoComTipo;
    infoComTipo.tipo = TIPO_ALUNO;
    infoComTipo.dado.aluno.matricula_aluno = mat;
    strcpy(infoComTipo.dado.aluno.nome_aluno, nome);
    infoComTipo.dado.aluno.codigo_curso = cod_curso;
    infoComTipo.dado.aluno.ano_ingresso = ano;
    infoComTipo.dado.aluno.semestre_ingresso = sem;

    return inserirNo23(raiz_alunos, infoComTipo);
}

// 1. Listar matrícula e nome dos alunos de um determinado curso
int listarAlunosPorCurso23Rec(arv_2_3* raiz, int cod_curso) {
    int total = 0;
    if (raiz != NULL){
        total += listarAlunosPorCurso23Rec(raiz->esq, cod_curso);

        if (raiz->info[0].tipo == TIPO_ALUNO && raiz->info[0].dado.aluno.codigo_curso == cod_curso) {
            printf("Matricula: %d | Nome: %s\n", 
                raiz->info[0].dado.aluno.matricula_aluno, raiz->info[0].dado.aluno.nome_aluno);
            total += 1;
        }

        total += listarAlunosPorCurso23Rec(raiz->cen, cod_curso);

        if (raiz->n_infos == 2) {
            if (raiz->info[1].tipo == TIPO_ALUNO && raiz->info[1].dado.aluno.codigo_curso == cod_curso) {
                printf("Matricula: %d | Nome: %s\n", 
                    raiz->info[1].dado.aluno.matricula_aluno, raiz->info[1].dado.aluno.nome_aluno);
                total += 1;
            }
            total += listarAlunosPorCurso23Rec(raiz->dir, cod_curso);
        }
    }
    return total;
}

void listarAlunosPorCurso23(arv_2_3* raiz, int cod_curso) {
    int total = listarAlunosPorCurso23Rec(raiz, cod_curso);
    if (total == 0) {
        printf("(Nenhum aluno cadastrado para este curso)\n");
    }
}

// 2. Listar alunos de um curso que entraram em um determinado ano
int listarAlunosPorCursoEAno23Rec(arv_2_3* raiz, int cod_curso, int ano) {
    int total = 0;
    if (raiz != NULL){
        total += listarAlunosPorCursoEAno23Rec(raiz->esq, cod_curso, ano);

        if (raiz->info[0].tipo == TIPO_ALUNO && 
            raiz->info[0].dado.aluno.codigo_curso == cod_curso && 
            raiz->info[0].dado.aluno.ano_ingresso == ano) {
            printf("Matricula: %d | Nome: %s | Ano: %d\n", 
                raiz->info[0].dado.aluno.matricula_aluno, 
                raiz->info[0].dado.aluno.nome_aluno, 
                raiz->info[0].dado.aluno.ano_ingresso);
            total += 1;
        }

        total += listarAlunosPorCursoEAno23Rec(raiz->cen, cod_curso, ano);

        if (raiz->n_infos == 2) {
            if (raiz->info[1].tipo == TIPO_ALUNO && 
                raiz->info[1].dado.aluno.codigo_curso == cod_curso && 
                raiz->info[1].dado.aluno.ano_ingresso == ano) {
                printf("Matricula: %d | Nome: %s | Ano: %d\n", 
                    raiz->info[1].dado.aluno.matricula_aluno, 
                    raiz->info[1].dado.aluno.nome_aluno, 
                    raiz->info[1].dado.aluno.ano_ingresso);
                total += 1;
            }
            total += listarAlunosPorCursoEAno23Rec(raiz->dir, cod_curso, ano);
        }
    }
    return total;
}

void listarAlunosPorCursoEAno23(arv_2_3* raiz, int cod_curso, int ano) {
    int total = listarAlunosPorCursoEAno23Rec(raiz, cod_curso, ano);
    if (total == 0) {
        printf("(Nenhum aluno cadastrado para este curso e ano)\n");
    }
}

// 3. Mostrar quantos alunos tem um determinado curso (Contagem com Retorno Único)
int contarAlunosNoCurso23(arv_2_3* raiz, int cod_curso) {
    int total = 0;

    if (raiz != NULL) {
        int atual = 0;

        // Verifica a primeira posição do nó
        if (raiz->info[0].tipo == TIPO_ALUNO && raiz->info[0].dado.aluno.codigo_curso == cod_curso) {
            atual++;
        }

        // Se tiver a segunda posição, verifica também
        if (raiz->n_infos == 2 && raiz->info[1].tipo == TIPO_ALUNO && raiz->info[1].dado.aluno.codigo_curso == cod_curso) {
            atual++;
        }

        // Acumula o atual com o resultado de todos os filhos possíveis
        total = atual + contarAlunosNoCurso23(raiz->esq, cod_curso) + 
                       contarAlunosNoCurso23(raiz->cen, cod_curso);
        
        if (raiz->n_infos == 2) {
            total += contarAlunosNoCurso23(raiz->dir, cod_curso);
        }
    }

    return total;
}

/* ============================================================
   FUNÇÕES DE IMPRESSÃO ESPECÍFICAS - ÁRVORE 2-3
   ============================================================ */

// 1. Especializada em Disciplinas (Nível Folha da Sub-árvore)
void imprimirDisciplinas23(arv_2_3* raiz) {
    if (raiz != NULL) {
        // Visita o filho esquerdo
        imprimirDisciplinas23(raiz->esq);
        
        // Imprime a primeira disciplina (sempre existe se o nó não for nulo)
        printf("\n  -> [DISCIPLINA] ID: %d | Nome: %s | Bloco: %d | Carga: %dh", 
                raiz->info[0].dado.disciplina.codigo_disciplina, 
                raiz->info[0].dado.disciplina.nome_disciplina, 
                raiz->info[0].dado.disciplina.bloco_disciplina, 
                raiz->info[0].dado.disciplina.carga_horaria);
        
        // Visita o filho central
        imprimirDisciplinas23(raiz->cen);
        
        // Se o nó estiver cheio, imprime a segunda disciplina e visita o filho direito
        if (raiz->n_infos == 2) {
            printf("\n  -> [DISCIPLINA] ID: %d | Nome: %s | Bloco: %d | Carga: %dh", 
                    raiz->info[1].dado.disciplina.codigo_disciplina, 
                    raiz->info[1].dado.disciplina.nome_disciplina, 
                    raiz->info[1].dado.disciplina.bloco_disciplina, 
                    raiz->info[1].dado.disciplina.carga_horaria);
            imprimirDisciplinas23(raiz->dir);
        }
    }
}

// 2. Especializada em Cursos (Imprime o curso e varre sua árvore interna de disciplinas)
void imprimirCursos23Rec(arv_2_3* raiz) {
    if (raiz == NULL){
        // Visita o filho esquerdo
        imprimirCursos23Rec(raiz->esq);

        // --- PROCESSA INFO[0] ---
        curso* curso0 = &(raiz->info[0].dado.curso);
        printf("\n[CURSO] ID: %d | Nome: %s | Blocos: %d\n", 
                curso0->codigo_curso, curso0->nome_curso, curso0->qtd_blocos_curso);
        printf("  -------------------------------------------");
        if (curso0->raiz_disciplinas == NULL) {
            printf("\n  (Nenhuma disciplina cadastrada neste curso)\n");
        } else {
            imprimirDisciplinas23(curso0->raiz_disciplinas);
        }
        printf("\n  -------------------------------------------\n");

        // Visita o filho central
        imprimirCursos23Rec(raiz->cen);

        // --- PROCESSA INFO[1] (Se o nó possuir duas informações) ---
        if (raiz->n_infos == 2) {
            curso* curso1 = &(raiz->info[1].dado.curso);
            printf("\n[CURSO] ID: %d | Nome: %s | Blocos: %d\n", 
                    curso1->codigo_curso, curso1->nome_curso, curso1->qtd_blocos_curso);
            printf("  -------------------------------------------");
            if (curso1->raiz_disciplinas == NULL) {
                printf("\n  (Nenhuma disciplina cadastrada neste curso)\n");
            } else {
                imprimirDisciplinas23(curso1->raiz_disciplinas);
            }
            printf("\n  -------------------------------------------\n");

            // Visita o filho direito
            imprimirCursos23Rec(raiz->dir);
        }
    }
}

void imprimirCursos23(arv_2_3* raiz) {
    if (raiz == NULL) {
        printf("\nNenhum curso cadastrado.\n");
        return;
    }
    imprimirCursos23Rec(raiz);
}

// 3. Especializada em Alunos
void imprimirAlunos23Rec(arv_2_3* raiz) {
    if (raiz != NULL){
        // Visita o filho esquerdo
        imprimirAlunos23Rec(raiz->esq);

        // Imprime o primeiro aluno
        printf("\n[ALUNO] Mat: %d | Nome: %s | Curso: %d | Ingresso: %d/%d", 
                raiz->info[0].dado.aluno.matricula_aluno, 
                raiz->info[0].dado.aluno.nome_aluno, 
                raiz->info[0].dado.aluno.codigo_curso, 
                raiz->info[0].dado.aluno.ano_ingresso, 
                raiz->info[0].dado.aluno.semestre_ingresso);

        // Visita o filho central
        imprimirAlunos23Rec(raiz->cen);

        // Se o nó possuir dois alunos cadastrados nele
        if (raiz->n_infos == 2) {
            printf("\n[ALUNO] Mat: %d | Nome: %s | Curso: %d | Ingresso: %d/%d", 
                    raiz->info[1].dado.aluno.matricula_aluno, 
                    raiz->info[1].dado.aluno.nome_aluno, 
                    raiz->info[1].dado.aluno.codigo_curso, 
                    raiz->info[1].dado.aluno.ano_ingresso, 
                    raiz->info[1].dado.aluno.semestre_ingresso);

            // Visita o filho direito
            imprimirAlunos23Rec(raiz->dir);
       }
    }                            
}

void imprimirAlunos23(arv_2_3* raiz) {
    if (raiz == NULL) {
        printf("\nNenhum aluno cadastrado.\n");
        return;
    }
    imprimirAlunos23Rec(raiz);
}