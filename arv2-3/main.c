#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estruturas2-3.h"

void cabecalho() {
    printf("\n/***********************************************************/\n");
    printf(" SISTEMA ACADEMICO (ARVORE 2-3) - UFPI 2026   \n");
    printf(" GESTAO DE CURSOS, DISCIPLINAS E ALUNOS            \n");
    printf("/***********************************************************/\n");
}

/* =========================================================================
   FUNÇÕES AUXILIARES DO EXPERIMENTO (CAMPOS CORRIGIDOS CONFORME SEU .H)
   ========================================================================= */

// Função de rastreamento utilizando estritamente n_infos, esq, cen e dir
arv_2_3* buscar_rastreando_23(arv_2_3* raiz, int chave) {
    if (raiz == NULL) {
        printf(" -> [NULO] (Nao encontrado)\n");
        return NULL;
    }

    // Printa o nó atual e as chaves usando o campo correto: n_infos
    printf(" -> [No c/ %d info(s): ", raiz->n_infos);
    for (int j = 0; j < raiz->n_infos; j++) {
        printf("%d ", raiz->info[j].dado.curso.codigo_curso);
    }
    printf("]");

    // Verifica se a chave está no nó atual
    if (raiz->info[0].dado.curso.codigo_curso == chave) {
        printf(" -> [ACHOU na Info 0!]\n");
        return raiz;
    }
    if (raiz->n_infos == 2 && raiz->info[1].dado.curso.codigo_curso == chave) {
        printf(" -> [ACHOU na Info 1!]\n");
        return raiz;
    }

    // Decisão de caminhamento usando os ponteiros corretos: esq, cen e dir
    if (chave < raiz->info[0].dado.curso.codigo_curso) {
        printf(" (Esq)");
        return buscar_rastreando_23(raiz->esq, chave);
    } 
    else if (raiz->n_infos == 1 || chave < raiz->info[1].dado.curso.codigo_curso) {
        printf(" (Centro)");
        return buscar_rastreando_23(raiz->cen, chave);
    } 
    else {
        printf(" (Dir)");
        return buscar_rastreando_23(raiz->dir, chave);
    }
}

// Injeção automática da mesma massa de dados usada na Rubro-Negra
void povoar_arvore_experimento_23(arv_2_3** raiz_cursos) {
    printf("\n[SISTEMA] Injetando carga massiva de cursos na Arvore 2-3...\n");
    
    inserirCurso23(raiz_cursos, 50, "Ciencia da Computacao", 8, 18);
    inserirCurso23(raiz_cursos, 30, "Engenharia Eletrica", 10, 18);
    inserirCurso23(raiz_cursos, 70, "Medicina", 12, 18);
    inserirCurso23(raiz_cursos, 15, "Direito", 10, 18);
    inserirCurso23(raiz_cursos, 40, "Administracao", 8, 18);
    inserirCurso23(raiz_cursos, 60, "Arquitetura", 10, 18);
    inserirCurso23(raiz_cursos, 90, "Psicologia", 10, 18);
    
    inserirCurso23(raiz_cursos, 5,   "Filosofia", 8, 18);
    inserirCurso23(raiz_cursos, 10,  "Historia", 8, 18);
    inserirCurso23(raiz_cursos, 12,  "Geografia", 8, 18);
    inserirCurso23(raiz_cursos, 18,  "Letras Portugues", 8, 18);
    inserirCurso23(raiz_cursos, 20,  "Pedagogia", 8, 18);
    inserirCurso23(raiz_cursos, 24,  "Artes Visuais", 8, 18);
    inserirCurso23(raiz_cursos, 25,  "Matematica", 8, 18);
    inserirCurso23(raiz_cursos, 32,  "Fisica", 8, 18);
    inserirCurso23(raiz_cursos, 35,  "Quimica", 8, 18);
    inserirCurso23(raiz_cursos, 45,  "Biologia", 8, 18);
    inserirCurso23(raiz_cursos, 48,  "Educacao Fisica", 8, 18);
    inserirCurso23(raiz_cursos, 52,  "Enfermagem", 10, 18);
    inserirCurso23(raiz_cursos, 55,  "Nutricao", 8, 18);
    inserirCurso23(raiz_cursos, 65,  "Odontologia", 10, 18);
    inserirCurso23(raiz_cursos, 68,  "Farmacia", 10, 18);
    inserirCurso23(raiz_cursos, 75,  "Fisioterapia", 10, 18);
    inserirCurso23(raiz_cursos, 80,  "Medicina Veterinaria", 10, 18);
    inserirCurso23(raiz_cursos, 85,  "Agronomia", 10, 18);
    inserirCurso23(raiz_cursos, 88,  "Zootecnia", 10, 18);
    inserirCurso23(raiz_cursos, 95,  "Engenharia Civil", 10, 18);
    inserirCurso23(raiz_cursos, 100, "Engenharia Mecanica", 10, 18);
    inserirCurso23(raiz_cursos, 105, "Engenharia de Producao", 10, 18);

    printf("[SISTEMA] 29 Cursos injetados com sucesso na Arvore 2-3!\n");
}

void executar_experimento_30_cursos_23(arv_2_3* raiz_cursos) {
    int codigos_teste[30] = {
        10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
        15, 25, 35, 45, 55, 65, 75, 85, 95, 105,
        5,  12, 18, 24, 32, 48, 52, 68, 88, 999 
    };

    clock_t tempo_inicial, tempo_final;
    double tempo_gasto;

    printf("\n===========================================================\n");
    printf("      INICIANDO EXPERIMENTO DE BUSCA: 30 ITENS (ARVORE 2-3)  \n");
    printf("===========================================================\n\n");

    tempo_inicial = clock();

    for (int i = 0; i < 30; i++) {
        printf("Busca #%02d [Procurando ID: %d]:\nRAIZ", i + 1, codigos_teste[i]);
        buscar_rastreando_23(raiz_cursos, codigos_teste[i]);
        printf("-----------------------------------------------------------\n");
    }

    tempo_final = clock();
    tempo_gasto = ((double)(tempo_final - tempo_inicial)) / CLOCKS_PER_SEC;

    printf("\n>>> RESULTADO DO EXPERIMENTO <<<\n");
    printf("Tempo total computado para as 30 buscas na 2-3: %f segundos.\n", tempo_gasto);
}

/* =========================================================================
   MAIN PRINCIPAL
   ========================================================================= */
int main() {
    Arv23* raiz_cursos = NULL;
    Arv23* raiz_alunos = NULL;
    
    int opcao, res;
    int cod, blocos, semanas, cod_d, bloco_d, carga;
    int cod_curso, ano, sem; 
    char nome[TAM_NOME];

    do {
        cabecalho();
        printf("  [1] Cadastrar Novo Curso\n");
        printf("  [2] Cadastrar Disciplina em Curso\n");
        printf("  [3] Cadastrar Novo Aluno\n");
        printf("  [4] Imprimir Arvore de Cursos\n");
        printf("  [5] Imprimir Dados de um Curso\n"); 
        printf("  [6] Imprimir Cursos com a mesma qtd de blocos\n");
        printf("  [7] Imprimir Arvore de Disciplinas\n");
        printf("  [8] Imprimir Dados de uma Disciplina\n");
        printf("  [9] Imprimir Disciplinas de um Bloco (Curso Especifico)\n");
        printf("  [10] Imprimir Disciplinas com a mesma Carga Horaria\n");
        printf("  [11] Listar Todos os Alunos\n");     
        printf("  [12] Listar Alunos de um Curso\n");
        printf("  [13] Listar Alunos por Curso e Ano\n");
        printf("  [14] Contar Alunos em um Curso\n");
        printf("  [15] Excluir Disciplina de um Curso\n");
        printf("  [16] Excluir Curso\n");
        printf("  [17] Executar Experimento de Busca (Auto-Povoado)\n"); // Adicionado
        printf("  [0] Sair do Sistema\n");
        printf("-----------------------------------------------------------\n");
        printf("  Selecione uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            while(getchar() != '\n'); 
            continue;
        }

        switch (opcao) {
            case 1:
                printf("\n>> FORMULARIO DE CURSO (2-3) <<\n");
                printf("Codigo: "); 
                scanf("%d", &cod);

                if (buscar23(raiz_cursos, cod) != NULL) {
                    printf("\n[ERRO] O curso %d ja esta cadastrado!\n", cod);
                    break;
                }

                printf("Nome do Curso: "); 
                scanf(" %[^\n]", nome);
                printf("Qtd. Blocos: "); 
                scanf("%d", &blocos);
                printf("Semanas/Semestre: "); 
                scanf("%d", &semanas);
                
                if (inserirCurso23(&raiz_cursos, cod, nome, blocos, semanas))
                    printf("\n[OK] Curso registrado com sucesso na Arvore 2-3!\n");
                else
                    printf("\n[ERRO] Falha na alocacao de memoria.\n");
                break;

            case 2:
                printf("\n/---------- VINCULAR DISCIPLINA (2-3) ----------/\n");
                printf("Codigo do Curso: "); 
                scanf("%d", &cod_curso);

                Arv23* curso_ref = buscar23(raiz_cursos, cod_curso);
                if (curso_ref == NULL) {
                    printf("\n[ERRO] Curso %d nao encontrado!\n", cod_curso);
                    break;
                }

                if (pode_vincular) {
                    curso* curso_alvo = NULL;
                    if (curso_ref->info[0].dado.curso.codigo_curso == cod_curso) {
                        curso_alvo = &(curso_ref->info[0].dado.curso);
                    } else {
                        curso_alvo = &(curso_ref->info[1].dado.curso);
                    }

                    printf("Codigo da Disciplina: "); 
                    scanf("%d", &cod_d);

                    if (buscar23(curso_alvo->raiz_disciplinas, cod_d) != NULL) {
                        printf("\n[ERRO] A disciplina %d ja existe no curso %s!\n", cod_d, curso_alvo->nome_curso);
                    } else {
                        printf("Nome da Disciplina: "); 
                        scanf(" %[^\n]", nome);

                        int max_b = curso_alvo->qtd_blocos_curso;
                        int sem_ref = curso_alvo->semanas_disciplina;
                        
                        do {
                            printf("Bloco (0 a %d): ", max_b - 1);
                            scanf("%d", &bloco_d);
                            
                            if (!validarBloco(bloco_d, max_b)) {
                                printf("[!] Bloco fora do limite do curso. Tente novamente.\n");
                            }
                        } while (!validarBloco(bloco_d, max_b));

                        do {
                            printf("Carga Horaria (Multiplo de %d): ", sem_ref);
                            scanf("%d", &carga);
                            
                            if (!validarCarga(carga, sem_ref)) {
                                printf("[!] Carga horaria deve ser multipla de %d. Tente novamente.\n", sem_ref);
                            }
                        } while (!validarCarga(carga, sem_ref));

                        res = inserirDisciplinaNoCurso23(raiz_cursos, cod_curso, cod_d, nome, bloco_d, carga);
                        
                        if (res == 1) { 
                            printf("\n[OK] Disciplina cadastrada com sucesso na sub-arvore 2-3!\n");
                        } else if (res == -1) {
                            printf("\n[ERRO] Curso nao encontrado durante a insercao.\n");
                        } else {
                            printf("\n[ERRO] Falha ao cadastrar disciplina.\n");
                        }
                    }
                }

                printf("Codigo da Disciplina: "); 
                scanf("%d", &cod_d);

                if (buscar23(curso_alvo->raiz_disciplinas, cod_d) != NULL) {
                    printf("\n[ERRO] A disciplina %d ja existe no curso %s!\n", cod_d, curso_alvo->nomecurso);
                    break;
                }

                printf("Nome da Disciplina: "); 
                scanf(" %[^\n]", nome);

                int max_b = curso_alvo->qtd_blocoscurso;
                do {
                    printf("Bloco (0 a %d): ", max_b - 1);
                    scanf("%d", &bloco_d);
                    if(bloco_d >= max_b || bloco_d < 0) printf("[!] Bloco fora do limite do curso.\n");
                } while (bloco_d >= max_b || bloco_d < 0);
                
                int sem_ref = curso_alvo->semanas_disciplina;
                do {
                    printf("Carga Horaria (Multiplo de %d): ", sem_ref);
                    scanf("%d", &carga);
                    if(carga % sem_ref != 0) printf("[!] Carga horaria deve ser multipla de %d.\n", sem_ref);
                } while (carga % sem_ref != 0 || carga <= 0);

                res = inserirDisciplinaNoCurso23(raiz_cursos, cod_curso, cod_d, nome, bloco_d, carga);
                if (res == 1) printf("\n[OK] Disciplina cadastrada com sucesso na sub-arvore 2-3!\n");
                break;

            case 3:
                printf("\n/---------- CADASTRO DE ALUNO (2-3) ----------/\n");
                printf("Matricula: "); 
                scanf("%d", &cod); 

                if (buscar23(raiz_alunos, cod) != NULL) {
                    printf("\n[ERRO] Matricula %d ja existe!\n", cod);
                    break;
                }

                printf("Nome do Aluno: "); 
                scanf(" %[^\n]", nome);
                printf("Codigo do Curso: "); 
                scanf("%d", &cod_curso);

                if (buscar23(raiz_cursos, cod_curso) == NULL) {
                    printf("\n[ERRO] Curso %d nao existe! Cadastre o curso primeiro.\n", cod_curso);
                    break;
                }

                printf("Ano de Ingresso: "); 
                scanf("%d", &ano);
                printf("Semestre (1 ou 2): "); 
                scanf("%d", &sem);

                if (inserirAluno23(&raiz_alunos, cod, nome, cod_curso, ano, sem))
                    printf("\n[OK] Aluno cadastrado com sucesso na Arvore 2-3!\n");
                else
                    printf("\n[ERRO] Falha ao cadastrar aluno.\n");
                break;

            case 4: // RELATÓRIO DE CURSOS
                printf("\n======= RELATORIO 2-3: CURSOS E DISCIPLINAS =======");
                if (raiz_cursos == NULL) {
                    printf("\nNenhum curso cadastrado.\n");
                } else {
                    imprimirCursos23(raiz_cursos);
                }
                break;

            case 5: // RELATÓRIO DE ALUNOS
                printf("\n=========== RELATORIO 2-3: ALUNOS ============");
                if (raiz_alunos == NULL) {
                    printf("\nNenhum aluno cadastrado.\n");
                } else {
                    imprimirAlunos23(raiz_alunos);
                }
                break;

            case 6: // LISTAR ALUNOS POR CURSO
                printf("\n>> LISTAR ALUNOS POR CURSO (2-3) <<\n");
                printf("Codigo do Curso: "); scanf("%d", &cod_curso);
                printf("\nAlunos do Curso %d:\n", cod_curso);
                listarAlunosPorCurso23(raiz_alunos, cod_curso);
                break;

            case 7: // LISTAR ALUNOS POR CURSO E ANO
                printf("\n>> LISTAR ALUNOS POR CURSO E ANO (2-3) <<\n");
                printf("Codigo do Curso: "); scanf("%d", &cod_curso);
                printf("Ano de Ingresso: "); scanf("%d", &ano);
                printf("\nAlunos do Curso %d ingressos em %d:\n", cod_curso, ano);
                listarAlunosPorCursoEAno23(raiz_alunos, cod_curso, ano);
                break;

            case 8: // CONTAR ALUNOS NO CURSO
                printf("\n>> QUANTIDADE DE ALUNOS NO CURSO (2-3) <<\n");
                printf("Codigo do Curso: "); scanf("%d", &cod_curso);
                res = contarAlunosNoCurso23(raiz_alunos, cod_curso);
                printf("\nTotal de alunos no curso %d: %d\n", cod_curso, res);
                break;

            case 15: 
                printf("\n>> EXCLUIR DISCIPLINA DE UM CURSO (2-3) <<\n");
                if (raiz_cursos == NULL) {
                    printf("\n[ERRO] Nenhum curso cadastrado no sistema.\n");
                } else {
                    printf("Codigo do Curso: ");
                    scanf("%d", &cod_curso);
                    
                    arv_2_3* no_curso = buscar23(raiz_cursos, cod_curso);
                    
                    if (no_curso == NULL) {
                        printf("\n[ERRO] Curso %d nao encontrado!\n", cod_curso);
                    } else {
                        curso* curso_alvo = obterCursoNo23(no_curso, cod_curso);

                        if (curso_alvo->raiz_disciplinas == NULL) {
                            printf("\n[ERRO] O curso %d nao possui disciplinas cadastradas.\n", cod_curso);
                        } else {
                            printf("Codigo da Disciplina a ser excluida: ");
                            scanf("%d", &cod_d);

                            if (buscar23(curso_alvo->raiz_disciplinas, cod_d) == NULL) {
                                printf("\n[ERRO] Disciplina %d nao encontrada no curso %d.\n", cod_d, cod_curso);
                            } else {
                                res = removerNo23(&(curso_alvo->raiz_disciplinas), cod_d);
                                
                                if (res == 1) {
                                    printf("\n[OK] Disciplina %d excluida do curso %d com sucesso!\n", cod_d, cod_curso);
                                } else {
                                    printf("\n[ERRO] Falha ao excluir disciplina %d.\n", cod_d);
                                }
                            }
                        }
                    }
                }
                break;

            case 16: 
                printf("\n>> EXCLUIR CURSO (2-3) <<\n");
                printf("Codigo do Curso a ser excluido: ");
                scanf("%d", &cod_curso);

                if (raiz_cursos == NULL) {
                    printf("\n[ERRO] Nenhum curso cadastrado.\n");
                } else {
                    arv_2_3* no_curso = buscar23(raiz_cursos, cod_curso);
                    
                    if (no_curso == NULL) {
                        printf("\n[ERRO] Curso %d nao encontrado!\n", cod_curso);
                    } else {
                        curso* curso_alvo = obterCursoNo23(no_curso, cod_curso);

                        if (curso_alvo->raiz_disciplinas != NULL) {
                            printf("\n[ERRO] O curso %d possui disciplinas cadastradas. Remova-as primeiro.\n", cod_curso);
                        } else {
                            res = removerNo23(&raiz_cursos, cod_curso);
                            
                            if (res == 1) {
                                printf("\n[OK] Curso %d excluido com sucesso!\n", cod_curso);
                            } else {
                                printf("\n[ERRO] Falha ao excluir curso %d.\n", cod_curso);
                            }
                        }
                    }
                }
                break;

            case 17: 
                if (raiz_cursos == NULL) {
                    povoar_arvore_experimento_23(&raiz_cursos);
                }
                executar_experimento_30_cursos_23(raiz_cursos);
                break;

            case 0:
                printf("\nFinalizando sessao da Arvore 2-3. Ate logo!\n");
                break;

            default:
                printf("\n[!] Opcao invalida.\n");
        }

        printf("\nPressione ENTER para continuar...");
        getchar(); getchar(); 
        system("cls || clear"); 
    } while (opcao != 0);

    return 0;
}