#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas2-3.h"

/* Função auxiliar para pegar a chave (ID/Matrícula) de forma genérica */
int obterChave23(InfoComTipo info) {
    int chave = -1;
    switch (info.tipo) {
        case TIPO_ALUNO:      
            chave = info.dado.aluno.matriculaaluno; 
            break;
        case TIPO_CURSO:      
            chave = info.dado.curso.codigocurso; 
            break;
        case TIPO_DISCIPLINA: 
            chave = info.dado.disciplina.codigodisciplina; 
            break;
    }
    return chave;
}

/* Cria um novo nó da árvore 2-3 */
Arv23* criarNo23(InfoComTipo info, Arv23* filhoEsq, Arv23* filhoCen) {
    Arv23* novo = (Arv23*)malloc(sizeof(Arv23));
    if (novo) {
        novo->info[0] = info;
        novo->nInfos = 1;
        novo->esq = filhoEsq;
        novo->cen = filhoCen;
        novo->dir = NULL;
    }
    return novo;
}

/* Função de Busca (Lógica 2-3: olha info[0], se não achar e tiver info[1], olha info[1]) */
Arv23* buscar23(Arv23* raiz, int chave) {
    Arv23* res = NULL;

    if (raiz != NULL) {
        int chave0 = obterChave23(raiz->info[0]);

        if (chave == chave0) {
            res = raiz;
        } else if (raiz->nInfos == 2 && chave == obterChave23(raiz->info[1])) {
            res = raiz;
        } else {
            // Se não achou no nó, decide para qual filho descer
            if (chave < chave0) {
                res = buscar23(raiz->esq, chave);
            } else if (raiz->nInfos == 1) {
                res = buscar23(raiz->cen, chave);
            } else if (chave < obterChave23(raiz->info[1])) {
                res = buscar23(raiz->cen, chave);
            } else {
                res = buscar23(raiz->dir, chave);
            }
        }
    }
    return res;\
}

void adicionarInfoNo(Arv23 *no, InfoComTipo novaInfo, Arv23 *filhoRelativo) {
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
    no->nInfos = 2;
}

Arv23* quebraNo(Arv23 *no, InfoComTipo novaInfo, InfoComTipo *promove, Arv23 *filhoRelativo) {
    Arv23 *maior;
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

    no->nInfos = 1;
    no->dir = NULL; 
    // O nó original volta a ser um nó de 1 info
    return maior;
}

/* Função auxiliar para saber se um nó é folha */
int ehFolha23(Arv23 *no) {
    return (no->esq == NULL && no->cen == NULL && no->dir == NULL);
}

/* Função interna recursiva de inserção */
Arv23* insere23Recursivo(Arv23 *no, InfoComTipo novaInfo, InfoComTipo *promove, int *houvePromocao) {
    Arv23 *maiorFilho = NULL;

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
        } else if (no->nInfos == 1 || chaveNova < obterChave23(no->info[1])) {
            maiorFilho = insere23Recursivo(no->cen, novaInfo, promove, houvePromocao);
        } else {
            maiorFilho = insere23Recursivo(no->dir, novaInfo, promove, houvePromocao);
        }
    }

    // --- VOLTA DA RECURSÃO (Tratamento dos nós superiores) ---
    if (*houvePromocao) {
        // Se o nó atual tem espaço, absorve o elemento promovido ("Onda")
        if (no->nInfos == 1) {
            adicionarInfoNo(no,*promove, maiorFilho);
            *houvePromocao = 0; // Promoção tratada com sucesso
            maiorFilho = NULL;
        } 
        // Se o nó atual estiver cheio, explode ele ("Quebra")
        else {
            InfoComTipo promoveLocal;
            Arv23 *novoNo = quebraNo(no, *promove, &promoveLocal, maiorFilho);
            *promove = promoveLocal; // Atualiza o que vai subir para o próximo pai
            maiorFilho = novoNo;    // Passa o novo nó criado no split para o pai segurar
        }
    }

    return maiorFilho;
}

/* Interface pública para inserção (Trata a explosão da raiz se necessário) */
int inserirNo23(Arv23 **raiz, InfoComTipo novaInfo) {
    // Evita duplicatas usando a busca que já criamos
    if (buscar23(*raiz, obterChave23(novaInfo)) != NULL) {
        return 0; // Código/Matrícula já existe
    }

    // Se a árvore estiver completamente vazia, cria a raiz inicial
    if (*raiz == NULL) {
        *raiz = criarNo23(novaInfo, NULL, NULL);
        return 1;
    }

    InfoComTipo promove;
    int houvePromocao = 0;
    Arv23 *maiorFilho = insere23Recursivo(*raiz, novaInfo, &promove, &houvePromocao);

    // Se a raiz estourou, a árvore cresce 1 nível para cima
    if (houvePromocao) {
        Arv23 *novaRaiz = criarNo23(promove, *raiz, maiorFilho);
        *raiz = novaRaiz;
    }

    return 1; // Inserção concluída com sucesso
}

/* ============================================================
   FUNÇÕES ESPECÍFICAS (CURSO, DISCIPLINA, ALUNO) - ÁRVORE 2-3
   ============================================================ */

/* 1. Inserção de um Novo Curso */
int inserirCurso23(Arv23** raiz, int codigo, char nome[], int blocos, int semanas) {
    InfoComTipo infoComTipo;
    infoComTipo.tipo = TIPO_CURSO;
    infoComTipo.dado.curso.codigocurso = codigo;
    strcpy(infoComTipo.dado.curso.nomecurso, nome);
    infoComTipo.dado.curso.qtd_blocoscurso = blocos;
    infoComTipo.dado.curso.semanas_disciplina = semanas;
    infoComTipo.dado.curso.raiz_disciplinas = NULL; // Sub-árvore de disciplinas inicia vazia

    return inserirNo23(raiz, infoComTipo);
}

/* 2. Validação das Regras de Negócio (Item C do enunciado) */
int validarRegras23(int bloco_disciplina, int qtd_blocos_curso, int carga, int semanas) {
    int status = 0; // Ponto único de saída: assume erro por padrão

    if (bloco_disciplina < qtd_blocos_curso && bloco_disciplina >= 0 && 
        carga % semanas == 0 && carga > 0) {
        status = 1;
    }

    return status;
}

/* 3. Inserção de Disciplina vinculada a um Curso */
int inserirDisciplinaNoCurso23(Arv23* raiz_cursos, int cod_curso, int cod_disc, char nome[], int bloco, int carga) {
    int res = 0; // Ponto único de saída
    
    // Busca o curso na árvore 2-3 principal
    Arv23* no_curso = buscar23(raiz_cursos, cod_curso);
    
    if (no_curso == NULL) {
        res = -1; // Curso não encontrado
    } else {
        // Como o nó pode ter 1 ou 2 infos, precisamos achar qual delas é o curso alvo
        Curso* curso_alvo = NULL;
        if (no_curso->info[0].dado.curso.codigocurso == cod_curso) {
            curso_alvo = &(no_curso->info[0].dado.curso);
        } else {
            curso_alvo = &(no_curso->info[1].dado.curso);
        }

        // Valida as regras de negócio usando os dados do curso localizado
        if (!validarRegras23(bloco, curso_alvo->qtd_blocoscurso, carga, curso_alvo->semanas_disciplina)) {
            res = -2; // Regras violadas
        } 
        // Verifica se a disciplina já existe dentro da árvore de disciplinas desse curso
        else if (buscar23(curso_alvo->raiz_disciplinas, cod_disc) == NULL) {
            InfoComTipo info_disc;
            info_disc.tipo = TIPO_DISCIPLINA;
            info_disc.dado.disciplina.codigodisciplina = cod_disc;
            strcpy(info_disc.dado.disciplina.nomedisciplina, nome);
            info_disc.dado.disciplina.blocodisciplina = bloco;
            info_disc.dado.disciplina.carga_horaria = carga;

            // Insere na sub-árvore 2-3 de disciplinas do curso
            res = inserirNo23(&(curso_alvo->raiz_disciplinas), info_disc);
        }
    }
    
    return res;
}

/* 4. Inserção de um Novo Aluno */
int inserirAluno23(Arv23** raiz_alunos, int mat, char nome[], int cod_curso, int ano, int sem) {
    InfoComTipo infoComTipo;
    infoComTipo.tipo = TIPO_ALUNO;
    infoComTipo.dado.aluno.matriculaaluno = mat;
    strcpy(infoComTipo.dado.aluno.nomealuno, nome);
    infoComTipo.dado.aluno.codigocurso = cod_curso;
    infoComTipo.dado.aluno.ano_ingresso = ano;
    infoComTipo.dado.aluno.semestre_ingresso = sem;

    return inserirNo23(raiz_alunos, infoComTipo);
}

// 1. Listar matrícula e nome dos alunos de um determinado curso
void listarAlunosPorCurso23(Arv23* raiz, int cod_curso) {
    if (raiz != NULL) {
        // Visita o filho esquerdo
        listarAlunosPorCurso23(raiz->esq, cod_curso);
        
        // Verifica a primeira informação do nó
        if (raiz->info[0].tipo == TIPO_ALUNO && raiz->info[0].dado.aluno.codigocurso == cod_curso) {
            printf("Matricula: %d | Nome: %s\n", 
                   raiz->info[0].dado.aluno.matriculaaluno, raiz->info[0].dado.aluno.nomealuno);
        }
        
        // Visita o filho central
        listarAlunosPorCurso23(raiz->cen, cod_curso);
        
        // Se o nó tiver duas informações, verifica a segunda e visita o filho direito
        if (raiz->nInfos == 2) {
            if (raiz->info[1].tipo == TIPO_ALUNO && raiz->info[1].dado.aluno.codigocurso == cod_curso) {
                printf("Matricula: %d | Nome: %s\n", 
                       raiz->info[1].dado.aluno.matriculaaluno, raiz->info[1].dado.aluno.nomealuno);
            }
            listarAlunosPorCurso23(raiz->dir, cod_curso);
        }
    }
}

// 2. Listar alunos de um curso que entraram em um determinado ano
void listarAlunosPorCursoEAno23(Arv23* raiz, int cod_curso, int ano) {
    if (raiz != NULL) {
        // Visita o filho esquerdo
        listarAlunosPorCursoEAno23(raiz->esq, cod_curso, ano);
        
        // Verifica a primeira informação do nó
        if (raiz->info[0].tipo == TIPO_ALUNO && 
            raiz->info[0].dado.aluno.codigocurso == cod_curso && 
            raiz->info[0].dado.aluno.ano_ingresso == ano) {
            printf("Matricula: %d | Nome: %s | Ano: %d\n", 
                   raiz->info[0].dado.aluno.matriculaaluno, 
                   raiz->info[0].dado.aluno.nomealuno, 
                   raiz->info[0].dado.aluno.ano_ingresso);
        }
        
        // Visita o filho central
        listarAlunosPorCursoEAno23(raiz->cen, cod_curso, ano);
        
        // Se o nó tiver duas informações, verifica a segunda e visita o filho direito
        if (raiz->nInfos == 2) {
            if (raiz->info[1].tipo == TIPO_ALUNO && 
                raiz->info[1].dado.aluno.codigocurso == cod_curso && 
                raiz->info[1].dado.aluno.ano_ingresso == ano) {
                printf("Matricula: %d | Nome: %s | Ano: %d\n", 
                       raiz->info[1].dado.aluno.matriculaaluno, 
                       raiz->info[1].dado.aluno.nomealuno, 
                       raiz->info[1].dado.aluno.ano_ingresso);
            }
            listarAlunosPorCursoEAno23(raiz->dir, cod_curso, ano);
        }
    }
}

// 3. Mostrar quantos alunos tem um determinado curso (Contagem com Retorno Único)
int contarAlunosNoCurso23(Arv23* raiz, int cod_curso) {
    int total = 0;

    if (raiz != NULL) {
        int atual = 0;

        // Verifica a primeira posição do nó
        if (raiz->info[0].tipo == TIPO_ALUNO && raiz->info[0].dado.aluno.codigocurso == cod_curso) {
            atual++;
        }

        // Se tiver a segunda posição, verifica também
        if (raiz->nInfos == 2 && raiz->info[1].tipo == TIPO_ALUNO && raiz->info[1].dado.aluno.codigocurso == cod_curso) {
            atual++;
        }

        // Acumula o atual com o resultado de todos os filhos possíveis
        total = atual + contarAlunosNoCurso23(raiz->esq, cod_curso) + 
                       contarAlunosNoCurso23(raiz->cen, cod_curso);
        
        if (raiz->nInfos == 2) {
            total += contarAlunosNoCurso23(raiz->dir, cod_curso);
        }
    }

    return total;
}

/* ============================================================
   FUNÇÕES DE IMPRESSÃO ESPECÍFICAS - ÁRVORE 2-3
   ============================================================ */

// 1. Especializada em Disciplinas (Nível Folha da Sub-árvore)
void imprimirDisciplinas23(Arv23* raiz) {
    if (raiz != NULL) {
        // Visita o filho esquerdo
        imprimirDisciplinas23(raiz->esq);
        
        // Imprime a primeira disciplina (sempre existe se o nó não for nulo)
        printf("\n  -> [DISCIPLINA] ID: %d | Nome: %s | Bloco: %d | Carga: %dh", 
                raiz->info[0].dado.disciplina.codigodisciplina, 
                raiz->info[0].dado.disciplina.nomedisciplina, 
                raiz->info[0].dado.disciplina.blocodisciplina, 
                raiz->info[0].dado.disciplina.carga_horaria);
        
        // Visita o filho central
        imprimirDisciplinas23(raiz->cen);
        
        // Se o nó estiver cheio, imprime a segunda disciplina e visita o filho direito
        if (raiz->nInfos == 2) {
            printf("\n  -> [DISCIPLINA] ID: %d | Nome: %s | Bloco: %d | Carga: %dh", 
                    raiz->info[1].dado.disciplina.codigodisciplina, 
                    raiz->info[1].dado.disciplina.nomedisciplina, 
                    raiz->info[1].dado.disciplina.blocodisciplina, 
                    raiz->info[1].dado.disciplina.carga_horaria);
            imprimirDisciplinas23(raiz->dir);
        }
    }
}

// 2. Especializada em Cursos (Imprime o curso e varre sua árvore interna de disciplinas)
void imprimirCursos23(Arv23* raiz) {
    if (raiz != NULL) {
        // Visita o filho esquerdo
        imprimirCursos23(raiz->esq);
        
        // --- PROCESSA INFO[0] ---
        Curso* curso0 = &(raiz->info[0].dado.curso);
        printf("\n[CURSO] ID: %d | Nome: %s | Blocos: %d\n", 
                curso0->codigocurso, curso0->nomecurso, curso0->qtd_blocoscurso);
        printf("  -------------------------------------------");
        if (curso0->raiz_disciplinas == NULL) {
            printf("\n  (Nenhuma disciplina cadastrada neste curso)\n");
        } else {
            imprimirDisciplinas23(curso0->raiz_disciplinas);
        }
        printf("\n  -------------------------------------------\n");
        
        // Visita o filho central
        imprimirCursos23(raiz->cen);
        
        // --- PROCESSA INFO[1] (Se o nó possuir duas informações) ---
        if (raiz->nInfos == 2) {
            Curso* curso1 = &(raiz->info[1].dado.curso);
            printf("\n[CURSO] ID: %d | Nome: %s | Blocos: %d\n", 
                    curso1->codigocurso, curso1->nomecurso, curso1->qtd_blocoscurso);
            printf("  -------------------------------------------");
            if (curso1->raiz_disciplinas == NULL) {
                printf("\n  (Nenhuma disciplina cadastrada neste curso)\n");
            } else {
                imprimirDisciplinas23(curso1->raiz_disciplinas);
            }
            printf("\n  -------------------------------------------\n");
            
            // Visita o filho direito
            imprimirCursos23(raiz->dir);
        }
    }
}

// 3. Especializada em Alunos
void imprimirAlunos23(Arv23* raiz) {
    if (raiz != NULL) {
        // Visita o filho esquerdo
        imprimirAlunos23(raiz->esq);
        
        // Imprime o primeiro aluno
        printf("\n[ALUNO] Mat: %d | Nome: %s | Curso: %d | Ingresso: %d/%d", 
                raiz->info[0].dado.aluno.matriculaaluno, 
                raiz->info[0].dado.aluno.nomealuno, 
                raiz->info[0].dado.aluno.codigocurso, 
                raiz->info[0].dado.aluno.ano_ingresso, 
                raiz->info[0].dado.aluno.semestre_ingresso);
        
        // Visita o filho central
        imprimirAlunos23(raiz->cen);
        
        // Se o nó possuir dois alunos cadastrados nele
        if (raiz->nInfos == 2) {
            printf("\n[ALUNO] Mat: %d | Nome: %s | Curso: %d | Ingresso: %d/%d", 
                    raiz->info[1].dado.aluno.matriculaaluno, 
                    raiz->info[1].dado.aluno.nomealuno, 
                    raiz->info[1].dado.aluno.codigocurso, 
                    raiz->info[1].dado.aluno.ano_ingresso, 
                    raiz->info[1].dado.aluno.semestre_ingresso);
            
            // Visita o filho direito
            imprimirAlunos23(raiz->dir);
        }
    }
}