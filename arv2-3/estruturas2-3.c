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
    int res = 1;

    // Evita duplicatas usando a busca que já criamos
    if (buscar23(*raiz, obterChave23(novaInfo)) != NULL) {
        res = 0; // Código/Matrícula já existe
    } else if (*raiz == NULL) {
        // Se a árvore estiver completamente vazia, cria a raiz inicial
        *raiz = criarNo23(novaInfo, NULL, NULL);
        res = (*raiz != NULL);
    } else {
        info_com_tipo promove;
        int houvePromocao = 0;
        arv_2_3 *maiorFilho = insere23Recursivo(*raiz, novaInfo, &promove, &houvePromocao);

        // Se a raiz estourou, a árvore cresce 1 nível para cima
        if (houvePromocao) {
            arv_2_3 *novaRaiz = criarNo23(promove, *raiz, maiorFilho);
            *raiz = novaRaiz;
        }
    }

    return res;
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

int validarBloco(int bloco, int max_b) {
    return (bloco >= 0 && bloco < max_b); // Retorna 1 se ok, 0 se erro
}

int validarCarga(int carga, int semanas) {
    return (carga > 0 && carga % semanas == 0); // Retorna 1 se ok, 0 se erro
}

int inserirDisciplinaNoCurso23(arv_2_3* raiz_cursos, int cod_curso, int cod_disc, char nome[], int bloco, int carga) {
    int res = 0; // Ponto único de saída
    
    // Busca o curso na árvore 2-3 principal
    arv_2_3* no_curso = buscar23(raiz_cursos, cod_curso);
    
    if (no_curso == NULL) {
        // Erro: Curso não encontrado
        res = -1; 
    } else {
        // Como o nó pode ter 1 ou 2 infos, precisamos achar qual delas é o curso alvo
        curso* curso_alvo = NULL;
        if (no_curso->info[0].dado.curso.codigo_curso == cod_curso) {
            curso_alvo = &(no_curso->info[0].dado.curso);
        } else {
            curso_alvo = &(no_curso->info[1].dado.curso);
        }

        // Sucesso: Verifica se a disciplina NÃO existe e insere diretamente!
        if (buscar23(curso_alvo->raiz_disciplinas, cod_disc) == NULL) {
            info_com_tipo info_disc;
            info_disc.tipo = TIPO_DISCIPLINA;
            info_disc.dado.disciplina.codigo_disciplina = cod_disc;
            strcpy(info_disc.dado.disciplina.nome_disciplina, nome);
            info_disc.dado.disciplina.bloco_disciplina = bloco;
            info_disc.dado.disciplina.carga_horaria = carga;

            // Insere na sub-árvore 2-3 de disciplinas do curso
            res = inserirNo23(&(curso_alvo->raiz_disciplinas), info_disc);
        }
        // Obs: Se a disciplina já existir, 'res' continua sendo 0.
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

curso* obterCursoNo23(arv_2_3* no, int codigo_curso) {
    curso* res = NULL;

    if (no != NULL) {
        if (no->info[0].tipo == TIPO_CURSO && no->info[0].dado.curso.codigo_curso == codigo_curso) {
            res = &(no->info[0].dado.curso);
        } else if (no->n_infos == 2 && no->info[1].tipo == TIPO_CURSO &&
                   no->info[1].dado.curso.codigo_curso == codigo_curso) {
            res = &(no->info[1].dado.curso);
        }
    }

    return res;
}

disciplina* obterDisciplinaNo23(arv_2_3* no, int codigo_disciplina) {
    disciplina* res = NULL;

    if (no != NULL) {
        if (no->info[0].tipo == TIPO_DISCIPLINA &&
            no->info[0].dado.disciplina.codigo_disciplina == codigo_disciplina) {
            res = &(no->info[0].dado.disciplina);
        } else if (no->n_infos == 2 && no->info[1].tipo == TIPO_DISCIPLINA &&
                   no->info[1].dado.disciplina.codigo_disciplina == codigo_disciplina) {
            res = &(no->info[1].dado.disciplina);
        }
    }

    return res;
}

/* ============================================================
   FUNÇÕES DE IMPRESSÃO ESPECÍFICAS - ÁRVORE 2-3
   ============================================================ */

// Imprime as disciplinas mostrando a estrutura da sub-árvore (Níveis)
void imprimirDisciplinas23Rec(arv_2_3* raiz, int nivel) {
    if (raiz != NULL) {
        // Visita filho esquerdo
        imprimirDisciplinas23Rec(raiz->esq, nivel + 1);
        
        char tab[50] = "";
        for (int i = 0; i < nivel; i++) {
            strcat(tab, "            "); // 12 espaços por nível para alinhar bem
        }
        
        disciplina* d0 = &(raiz->info[0].dado.disciplina);
        printf("\n%s-> [NIVEL %d] [DISCIPLINA] ID: %d | Nome: %s", 
                tab, nivel, d0->codigo_disciplina, d0->nome_disciplina);
                
        // Visita filho central
        imprimirDisciplinas23Rec(raiz->cen, nivel + 1);
        
        if (raiz->n_infos == 2) {
            disciplina* d1 = &(raiz->info[1].dado.disciplina);
            printf("\n%s-> [NIVEL %d] [DISCIPLINA] ID: %d | Nome: %s", 
                    tab, nivel, d1->codigo_disciplina, d1->nome_disciplina);
                    
            // Visita filho direito
            imprimirDisciplinas23Rec(raiz->dir, nivel + 1);
        }
    }
}

void imprimirDisciplinas23(arv_2_3* raiz) {
    if (raiz == NULL) {
        printf("\n   (Nenhuma disciplina cadastrada)\n");
    } else {
        imprimirDisciplinas23Rec(raiz, 0); // Raiz da sub-árvore de disciplinas é nível 0
    }
}

// 2. Especializada em Cursos (Adaptada para mostrar Nível e Estrutura)
void imprimirCursos23Rec(arv_2_3* raiz, int nivel) {
    if (raiz != NULL) {
        // Visita o filho esquerdo descendo um nível
        imprimirCursos23Rec(raiz->esq, nivel + 1);

        // --- Gera o recuo visual baseado na profundidade da árvore ---
        char tab[50] = "";
        for (int i = 0; i < nivel; i++) {
            strcat(tab, "        "); // 8 espaços por nível
        }

        // --- PROCESSA INFO[0] ---
        curso* curso0 = &(raiz->info[0].dado.curso);
        printf("\n%s-> [NIVEL %d] [CURSO] ID: %d | Nome: %s | Blocos: %d\n", 
                tab, nivel, curso0->codigo_curso, curso0->nome_curso, curso0->qtd_blocos_curso);
        printf("%s   -------------------------------------------", tab);
        
        if (curso0->raiz_disciplinas == NULL) {
            printf("\n%s   (Nenhuma disciplina cadastrada)\n", tab);
        } else {
            imprimirDisciplinas23(curso0->raiz_disciplinas); // Mantém sua chamada original
            printf("\n");
        }
        printf("%s   -------------------------------------------\n", tab);

        // Visita o filho central descendo um nível
        imprimirCursos23Rec(raiz->cen, nivel + 1);

        // --- PROCESSA INFO[1] (Se o nó possuir duas informações) ---
        if (raiz->n_infos == 2) {
            curso* curso1 = &(raiz->info[1].dado.curso);
            printf("\n%s-> [NIVEL %d] [CURSO] ID: %d | Nome: %s | Blocos: %d\n", 
                    tab, nivel, curso1->codigo_curso, curso1->nome_curso, curso1->qtd_blocos_curso);
            printf("%s   -------------------------------------------", tab);
            
            if (curso1->raiz_disciplinas == NULL) {
                printf("\n%s   (Nenhuma disciplina cadastrada)\n", tab);
            } else {
                imprimirDisciplinas23(curso1->raiz_disciplinas);
                printf("\n");
            }
            printf("%s   -------------------------------------------\n", tab);

            // Visita o filho direito descendo um nível
            imprimirCursos23Rec(raiz->dir, nivel + 1);
        }
    }
}

// Interface principal adaptada
void imprimirCursos23(arv_2_3* raiz) {
    if (raiz == NULL) {
        printf("\nNenhum curso cadastrado.\n");
    } else {
        printf("\n================ ESTRUTURA DA ARVORE ================\n");
        imprimirCursos23Rec(raiz, 0); // A raiz principal começa no nível 0
        printf("=====================================================\n");
    }
}

void imprimirDadosCursos23(arv_2_3* raiz, int codigo_curso) {
    arv_2_3* no = buscar23(raiz, codigo_curso);
    curso* curso_alvo = obterCursoNo23(no, codigo_curso);

    if (curso_alvo == NULL) {
        printf("\n[ERRO] Curso %d nao encontrado!\n", codigo_curso);
    } else {
        printf("\n[CURSO] ID: %d | Nome: %s | Blocos: %d\n",
               curso_alvo->codigo_curso,
               curso_alvo->nome_curso,
               curso_alvo->qtd_blocos_curso);
        printf("  -------------------------------------------");

        if (curso_alvo->raiz_disciplinas == NULL) {
            printf("\n  (Nenhuma disciplina cadastrada neste curso)\n");
        } else {
            imprimirDisciplinas23(curso_alvo->raiz_disciplinas);
        }

        printf("\n  -------------------------------------------\n");
    }
}

void imprimirCursosComMesmaQtdBlocos23(arv_2_3* raiz, int blocos_ref, int* encontrou) {
    if (raiz != NULL) {
        imprimirCursosComMesmaQtdBlocos23(raiz->esq, blocos_ref, encontrou);

        if (raiz->info[0].tipo == TIPO_CURSO && raiz->info[0].dado.curso.qtd_blocos_curso == blocos_ref) {
            printf("\n[CURSO] ID: %d | Nome: %s | Blocos: %d",
                   raiz->info[0].dado.curso.codigo_curso,
                   raiz->info[0].dado.curso.nome_curso,
                   raiz->info[0].dado.curso.qtd_blocos_curso);
            *encontrou = 1;
        }

        imprimirCursosComMesmaQtdBlocos23(raiz->cen, blocos_ref, encontrou);

        if (raiz->n_infos == 2) {
            if (raiz->info[1].tipo == TIPO_CURSO && raiz->info[1].dado.curso.qtd_blocos_curso == blocos_ref) {
                printf("\n[CURSO] ID: %d | Nome: %s | Blocos: %d",
                       raiz->info[1].dado.curso.codigo_curso,
                       raiz->info[1].dado.curso.nome_curso,
                       raiz->info[1].dado.curso.qtd_blocos_curso);
                *encontrou = 1;
            }

            imprimirCursosComMesmaQtdBlocos23(raiz->dir, blocos_ref, encontrou);
        }
    }
}

void imprimirArvoreDisciplinas23(arv_2_3* raiz_cursos, int codigo_curso) {
    arv_2_3* no = buscar23(raiz_cursos, codigo_curso);
    curso* curso_alvo = obterCursoNo23(no, codigo_curso);

    if (curso_alvo == NULL) {
        printf("\n[ERRO] Curso %d nao encontrado.\n", codigo_curso);
    } else {
        printf("\n--- Disciplinas do Curso: %s ---\n", curso_alvo->nome_curso);

        if (curso_alvo->raiz_disciplinas != NULL) {
            imprimirDisciplinas23(curso_alvo->raiz_disciplinas);
            printf("\n");
        } else {
            printf("  (Nenhuma disciplina cadastrada neste curso)\n");
        }
    }
}

void imprimirDadosDisciplina23(arv_2_3* raiz_cursos, int codigo_curso, int codigo_disciplina) {
    arv_2_3* no_curso = buscar23(raiz_cursos, codigo_curso);
    curso* curso_alvo = obterCursoNo23(no_curso, codigo_curso);

    if (curso_alvo == NULL) {
        printf("\n[ERRO] Curso %d nao encontrado.\n", codigo_curso);
    } else {
        arv_2_3* no_disc = buscar23(curso_alvo->raiz_disciplinas, codigo_disciplina);
        disciplina* disc = obterDisciplinaNo23(no_disc, codigo_disciplina);

        if (disc == NULL) {
            printf("\n[ERRO] Disciplina %d nao encontrada no curso %d.\n", codigo_disciplina, codigo_curso);
        } else {
            printf("\n[DISCIPLINA] ID: %d | Nome: %s | Bloco: %d | Carga: %dh\n",
                   disc->codigo_disciplina,
                   disc->nome_disciplina,
                   disc->bloco_disciplina,
                   disc->carga_horaria);
        }
    }
}

void imprimirDisciplinasPorBlocoRec23(arv_2_3* raiz_disc, int bloco_ref, int* encontrou) {
    if (raiz_disc != NULL) {
        imprimirDisciplinasPorBlocoRec23(raiz_disc->esq, bloco_ref, encontrou);

        if (raiz_disc->info[0].tipo == TIPO_DISCIPLINA &&
            raiz_disc->info[0].dado.disciplina.bloco_disciplina == bloco_ref) {
            printf("\n  -> [DISCIPLINA] ID: %d | Nome: %s | Carga: %dh",
                   raiz_disc->info[0].dado.disciplina.codigo_disciplina,
                   raiz_disc->info[0].dado.disciplina.nome_disciplina,
                   raiz_disc->info[0].dado.disciplina.carga_horaria);
            *encontrou = 1;
        }

        imprimirDisciplinasPorBlocoRec23(raiz_disc->cen, bloco_ref, encontrou);

        if (raiz_disc->n_infos == 2) {
            if (raiz_disc->info[1].tipo == TIPO_DISCIPLINA &&
                raiz_disc->info[1].dado.disciplina.bloco_disciplina == bloco_ref) {
                printf("\n  -> [DISCIPLINA] ID: %d | Nome: %s | Carga: %dh",
                       raiz_disc->info[1].dado.disciplina.codigo_disciplina,
                       raiz_disc->info[1].dado.disciplina.nome_disciplina,
                       raiz_disc->info[1].dado.disciplina.carga_horaria);
                *encontrou = 1;
            }

            imprimirDisciplinasPorBlocoRec23(raiz_disc->dir, bloco_ref, encontrou);
        }
    }
}

void imprimirDisciplinasPorBloco23(arv_2_3* raiz_cursos, int codigo_curso, int bloco) {
    arv_2_3* no_curso = buscar23(raiz_cursos, codigo_curso);
    curso* curso_alvo = obterCursoNo23(no_curso, codigo_curso);

    if (curso_alvo == NULL) {
        printf("\n[ERRO] Curso %d nao encontrado.\n", codigo_curso);
    } else if (bloco >= curso_alvo->qtd_blocos_curso || bloco < 0) {
        printf("\n[ERRO] Bloco %d invalido!\n", bloco);
        printf("O curso %s possui o limite de %d bloco(s).\n",
               curso_alvo->nome_curso,
               curso_alvo->qtd_blocos_curso);
    } else {
        printf("\n--- Disciplinas do Bloco %d (Curso %d) ---", bloco, codigo_curso);

        int achou = 0;
        imprimirDisciplinasPorBlocoRec23(curso_alvo->raiz_disciplinas, bloco, &achou);

        if (achou == 0) {
            printf("\n  (Nenhuma disciplina encontrada no bloco %d)\n", bloco);
        } else {
            printf("\n");
        }
    }
}

void imprimirDisciplinasPorCHRec23(arv_2_3* raiz_disc, int ch_ref, int* encontrou) {
    if (raiz_disc != NULL) {
        imprimirDisciplinasPorCHRec23(raiz_disc->esq, ch_ref, encontrou);

        if (raiz_disc->info[0].tipo == TIPO_DISCIPLINA &&
            raiz_disc->info[0].dado.disciplina.carga_horaria == ch_ref) {
            printf("\n  -> [DISCIPLINA] ID: %d | Nome: %s | Bloco: %d",
                   raiz_disc->info[0].dado.disciplina.codigo_disciplina,
                   raiz_disc->info[0].dado.disciplina.nome_disciplina,
                   raiz_disc->info[0].dado.disciplina.bloco_disciplina);
            *encontrou = 1;
        }

        imprimirDisciplinasPorCHRec23(raiz_disc->cen, ch_ref, encontrou);

        if (raiz_disc->n_infos == 2) {
            if (raiz_disc->info[1].tipo == TIPO_DISCIPLINA &&
                raiz_disc->info[1].dado.disciplina.carga_horaria == ch_ref) {
                printf("\n  -> [DISCIPLINA] ID: %d | Nome: %s | Bloco: %d",
                       raiz_disc->info[1].dado.disciplina.codigo_disciplina,
                       raiz_disc->info[1].dado.disciplina.nome_disciplina,
                       raiz_disc->info[1].dado.disciplina.bloco_disciplina);
                *encontrou = 1;
            }

            imprimirDisciplinasPorCHRec23(raiz_disc->dir, ch_ref, encontrou);
        }
    }
}

void imprimirDisciplinasPorCargaHoraria23(arv_2_3* raiz_cursos, int codigo_curso, int ch_ref) {
    arv_2_3* no_curso = buscar23(raiz_cursos, codigo_curso);
    curso* curso_alvo = obterCursoNo23(no_curso, codigo_curso);

    if (curso_alvo == NULL) {
        printf("\n[ERRO] Curso %d nao encontrado.\n", codigo_curso);
    } else {
        printf("\n--- Disciplinas com Carga Horaria de %dh (Curso %d) ---", ch_ref, codigo_curso);

        int achou = 0;
        imprimirDisciplinasPorCHRec23(curso_alvo->raiz_disciplinas, ch_ref, &achou);

        if (achou == 0) {
            printf("\n  (Nenhuma disciplina com %dh encontrada)\n", ch_ref);
        } else {
            printf("\n");
        }
    }
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
    } else {
        imprimirAlunos23Rec(raiz);
    }
}

/*Funções de remoção*/
/* ============================================================
   FUNÇÕES DE REMOÇÃO - ÁRVORE 2-3 (Refatoradas - Single Return)
   ============================================================ */

/* Função auxiliar para pegar a maior informação da subárvore esquerda/central (Predecessor) */
info_com_tipo obterPredecessor23(arv_2_3 *no) {
    info_com_tipo predecessor;

    while (!ehFolha23(no)) {
        if (no->n_infos == 2) {
            no = no->dir;
        } else {
            no = no->cen;
        }
    }

    if (no->n_infos == 2) {
        predecessor = no->info[1];
    } else {
        predecessor = no->info[0];
    }

    return predecessor;
}

// =======================================================
// AJUSTE DO PAI APÓS MERGE
// =======================================================

void ajustarPaiAposMerge(arv_2_3 *pai) {

    if (pai != NULL){

        if (pai->n_infos == 2) {

            pai->info[0] = pai->info[1];

            pai->cen = pai->dir;

            pai->dir = NULL;

            pai->n_infos = 1;
        }
        else {

            pai->cen = NULL;

            pai->n_infos = 0;
        }
    }
}


// =======================================================
// REBALANCEAMENTO ESQUERDO
// =======================================================

int rebalancearEsq(arv_2_3 *pai) {

    int houveUnderflowPai = 0;

    arv_2_3 *esq = pai->esq;
    arv_2_3 *cen = pai->cen;

    // ==========================================
    // REDISTRIBUIÇÃO
    // ==========================================

    if (cen != NULL && cen->n_infos == 2) {

        esq->info[0] = pai->info[0];

        esq->cen = cen->esq;

        esq->n_infos = 1;

        pai->info[0] = cen->info[0];

        cen->info[0] = cen->info[1];

        cen->esq = cen->cen;
        cen->cen = cen->dir;
        cen->dir = NULL;

        cen->n_infos = 1;
    }

    // ==========================================
    // MERGE
    // ==========================================

    else {

        esq->info[0] = pai->info[0];
        esq->info[1] = cen->info[0];

        // preserva esq->esq
        esq->cen = cen->esq;
        esq->dir = cen->cen;

        esq->n_infos = 2;

        free(cen);

        pai->cen = NULL;

        ajustarPaiAposMerge(pai);

        houveUnderflowPai = (pai->n_infos == 0);
    }

    return houveUnderflowPai;
}


// =======================================================
// REBALANCEAMENTO CENTRAL
// =======================================================

int rebalancearCen(arv_2_3 *pai) {

    int houveUnderflowPai = 0;

    arv_2_3 *esq = pai->esq;
    arv_2_3 *cen = pai->cen;
    arv_2_3 *dir = pai->dir;

    // ==========================================
    // REDISTRIBUIÇÃO PELO ESQUERDO
    // ==========================================

    if (esq != NULL && esq->n_infos == 2) {

        cen->info[0] = pai->info[0];

        cen->cen = cen->esq;

        cen->esq = esq->dir;

        cen->n_infos = 1;

        pai->info[0] = esq->info[1];

        esq->dir = NULL;

        esq->n_infos = 1;
    }

    // ==========================================
    // REDISTRIBUIÇÃO PELO DIREITO
    // ==========================================

    else if (dir != NULL && dir->n_infos == 2) {

        cen->info[0] = pai->info[1];

        cen->cen = dir->esq;

        cen->n_infos = 1;

        pai->info[1] = dir->info[0];

        dir->info[0] = dir->info[1];

        dir->esq = dir->cen;
        dir->cen = dir->dir;
        dir->dir = NULL;

        dir->n_infos = 1;
    }

    // ==========================================
    // MERGE
    // ==========================================

    else {

        esq->info[1] = pai->info[0];

        // preserva:
        // esq->esq
        // esq->cen

        esq->dir = cen->esq;

        esq->n_infos = 2;

        free(cen);

        pai->cen = NULL;

        ajustarPaiAposMerge(pai);

        houveUnderflowPai = (pai->n_infos == 0);
    }

    return houveUnderflowPai;
}


// =======================================================
// REBALANCEAMENTO DIREITO
// =======================================================

int rebalancearDir(arv_2_3 *pai) {

    arv_2_3 *cen = pai->cen;
    arv_2_3 *dir = pai->dir;

    // ==========================================
    // REDISTRIBUIÇÃO
    // ==========================================

    if (cen != NULL && cen->n_infos == 2) {

        dir->info[0] = pai->info[1];

        dir->cen = dir->esq;

        dir->esq = cen->dir;

        dir->n_infos = 1;

        pai->info[1] = cen->info[1];

        cen->dir = NULL;

        cen->n_infos = 1;
    }

    // ==========================================
    // MERGE
    // ==========================================

    else {

        cen->info[1] = pai->info[1];

        // preserva:
        // cen->esq
        // cen->cen

        cen->dir = dir->esq;

        cen->n_infos = 2;

        free(dir);

        pai->dir = NULL;

        // underflow à direita nunca propaga
        pai->n_infos = 1;
    }

    return 0;
}


// =======================================================
// REMOÇÃO RECURSIVA
// =======================================================

int remover23Recursivo(arv_2_3 **raiz, int chave) {

    int underflow = 0;

    if (raiz != NULL && *raiz != NULL) {

        arv_2_3 *no = *raiz;

        int chave0 = obterChave23(no->info[0]);

        int chave1 = -1;

        if (no->n_infos == 2)
            chave1 = obterChave23(no->info[1]);

        // ==========================================
        // REMOVE INFO[0]
        // ==========================================

        if (chave == chave0) {

            if (ehFolha23(no)) {

                if (no->n_infos == 2) {

                    no->info[0] = no->info[1];

                    no->n_infos = 1;
                }
                else {

                    no->n_infos = 0;

                    underflow = 1;
                }
            }
            else {

                info_com_tipo pred =
                    obterPredecessor23(no->esq);

                no->info[0] = pred;

                underflow =
                    remover23Recursivo(
                        &(no->esq),
                        obterChave23(pred)
                    );
            }
        }

        // ==========================================
        // REMOVE INFO[1]
        // ==========================================

        else if (
            no->n_infos == 2 &&
            chave == chave1
        ) {

            if (ehFolha23(no)) {

                no->n_infos = 1;
            }
            else {

                info_com_tipo pred =
                    obterPredecessor23(no->cen);

                no->info[1] = pred;

                underflow =
                    remover23Recursivo(
                        &(no->cen),
                        obterChave23(pred)
                    );
            }
        }

        // ==========================================
        // DESCIDA
        // ==========================================

        else {

            if (chave < chave0) {

                underflow =
                    remover23Recursivo(
                        &(no->esq),
                        chave
                    );
            }

            else if (
                no->n_infos == 1 ||
                chave < chave1
            ) {

                underflow =
                    remover23Recursivo(
                        &(no->cen),
                        chave
                    );
            }

            else {

                underflow =
                    remover23Recursivo(
                        &(no->dir),
                        chave
                    );
            }
        }

        // ==========================================
        // REBALANCEAMENTO
        // ==========================================

        if (underflow) {

            if (
                no->esq != NULL &&
                no->esq->n_infos == 0
            ) {

                underflow =
                    rebalancearEsq(no);
            }

            else if (
                no->cen != NULL &&
                no->cen->n_infos == 0
            ) {

                underflow =
                    rebalancearCen(no);
            }

            else if (
                no->dir != NULL &&
                no->dir->n_infos == 0
            ) {

                underflow =
                    rebalancearDir(no);
            }
        }
    }

    return underflow;
}


// =======================================================
// INTERFACE PÚBLICA
// =======================================================

int removerNo23(arv_2_3 **raiz, int chave) {

    int sucesso = 0;

    if (raiz != NULL && *raiz != NULL) {

        int underflow =
            remover23Recursivo(
                raiz,
                chave
            );

        // redução da altura da árvore
        if (underflow) {

            arv_2_3 *raizAntiga = *raiz;

            *raiz = raizAntiga->esq;

            free(raizAntiga);
        }

        sucesso = 1;
    }

    return sucesso;
}

void destruirArvore23(arv_2_3 *raiz) {
    if (raiz != NULL) {
        destruirArvore23(raiz->esq);
        destruirArvore23(raiz->cen);
        destruirArvore23(raiz->dir);
        free(raiz);
    }
}