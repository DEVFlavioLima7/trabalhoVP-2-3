#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estruturas_vp.h"

void cabecalho() {
    printf("\n/***********************************************************/\n");
    printf(" SISTEMA ACADEMICO - UFPI 2026               \n");
    printf(" GESTAO DE CURSOS, DISCIPLINAS E ALUNOS            \n");
    printf("/***********************************************************/\n");
}

/* =========================================================================
   FUNÇÕES AUXILIARES DO EXPERIMENTO (REQUISITO DE BUSCA E RASTREAMENTO)
   ========================================================================= */
static int obter_chave_no(rb_node* no) {
    if (no->tipo == TIPO_CURSO) {
        return no->info.curso.codigo_curso;
    } else if (no->tipo == TIPO_DISCIPLINA) {
        return no->info.disciplina.codigo_disciplina;
    } else {
        return no->info.aluno.matricula_aluno;
    }
}

rb_node* buscar_rastreando_vp(rb_node* raiz, int chave, tipo_info tipo) {
    if (raiz == NULL) {
        printf(" -> [NULO] (Nao encontrado)\n");
        return NULL;
    }

    int chave_atual = obter_chave_no(raiz);
    const char* cor_nome = (raiz->cor == VERMELHO) ? "VERMELHO" : "PRETO";

    printf(" -> [No ID: %d | Cor: %s]", chave_atual, cor_nome);

    if (chave == chave_atual && raiz->tipo == tipo) {
        printf(" -> [ACHOU!]\n");
        return raiz;
    }

    if (chave < chave_atual) {
        printf(" (Esq)");
        return buscar_rastreando_vp(raiz->esq, chave, tipo);
    } else {
        printf(" (Dir)");
        return buscar_rastreando_vp(raiz->dir, chave, tipo);
    }
}

// Função maravilhosa para injetar a massa de dados sem digitação manual
void povoar_arvore_experimento(rb_node** raiz_cursos) {
    printf("\n[SISTEMA] Injetando carga massiva de cursos na Arvore Rubro-Negra...\n");
    
    // Inserindo primeiro os elementos centrais para garantir boa distribuição física dos nós
    inserirCurso(raiz_cursos, 50, "Ciencia da Computacao", 8, 18);
    inserirCurso(raiz_cursos, 30, "Engenharia Eletrica", 10, 18);
    inserirCurso(raiz_cursos, 70, "Medicina", 12, 18);
    inserirCurso(raiz_cursos, 15, "Direito", 10, 18);
    inserirCurso(raiz_cursos, 40, "Administracao", 8, 18);
    inserirCurso(raiz_cursos, 60, "Arquitetura", 10, 18);
    inserirCurso(raiz_cursos, 90, "Psicologia", 10, 18);
    
    // Inserindo o restante dos IDs que batem com o vetor do experimento
    inserirCurso(raiz_cursos, 5,   "Filosofia", 8, 18);
    inserirCurso(raiz_cursos, 10,  "Historia", 8, 18);
    inserirCurso(raiz_cursos, 12,  "Geografia", 8, 18);
    inserirCurso(raiz_cursos, 18,  "Letras Portugues", 8, 18);
    inserirCurso(raiz_cursos, 20,  "Pedagogia", 8, 18);
    inserirCurso(raiz_cursos, 24,  "Artes Visuais", 8, 18);
    inserirCurso(raiz_cursos, 25,  "Matematica", 8, 18);
    inserirCurso(raiz_cursos, 32,  "Fisica", 8, 18);
    inserirCurso(raiz_cursos, 35,  "Quimica", 8, 18);
    inserirCurso(raiz_cursos, 45,  "Biologia", 8, 18);
    inserirCurso(raiz_cursos, 48,  "Educacao Fisica", 8, 18);
    inserirCurso(raiz_cursos, 52,  "Enfermagem", 10, 18);
    inserirCurso(raiz_cursos, 55,  "Nutricao", 8, 18);
    inserirCurso(raiz_cursos, 65,  "Odontologia", 10, 18);
    inserirCurso(raiz_cursos, 68,  "Farmacia", 10, 18);
    inserirCurso(raiz_cursos, 75,  "Fisioterapia", 10, 18);
    inserirCurso(raiz_cursos, 80,  "Medicina Veterinaria", 10, 18);
    inserirCurso(raiz_cursos, 85,  "Agronomia", 10, 18);
    inserirCurso(raiz_cursos, 88,  "Zootecnia", 10, 18);
    inserirCurso(raiz_cursos, 95,  "Engenharia Civil", 10, 18);
    inserirCurso(raiz_cursos, 100, "Engenharia Mecanica", 10, 18);
    inserirCurso(raiz_cursos, 105, "Engenharia de Producao", 10, 18);

    printf("[SISTEMA] 29 Cursos injetados com sucesso!\n");
}

void executar_experimento_30_cursos(rb_node* raiz_cursos) {
    int codigos_teste[30] = {
        10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
        15, 25, 35, 45, 55, 65, 75, 85, 95, 105,
        5,  12, 18, 24, 32, 48, 52, 68, 88, 999 
    };

    clock_t tempo_inicial, tempo_final;
    double tempo_gasto;

    printf("\n===========================================================\n");
    printf("    INICIANDO EXPERIMENTO DE BUSCA: 30 ITENS (RUBRO-NEGRA)   \n");
    printf("===========================================================\n\n");

    tempo_inicial = clock();

    for (int i = 0; i < 30; i++) {
        printf("Busca #%02d [Procurando ID: %d]:\nRAIZ", i + 1, codigos_teste[i]);
        buscar_rastreando_vp(raiz_cursos, codigos_teste[i], TIPO_CURSO);
        printf("-----------------------------------------------------------\n");
    }

    tempo_final = clock();
    tempo_gasto = ((double)(tempo_final - tempo_inicial)) / CLOCKS_PER_SEC;

    printf("\n>>> RESULTADO DO EXPERIMENTO <<<\n");
    printf("Tempo total computado para as 30 buscas: %f segundos.\n", tempo_gasto);
}

/* =========================================================================
   MAIN PRINCIPAL
   ========================================================================= */
int main() {
    RBNode* raiz_cursos = NULL;
    RBNode* raiz_alunos = NULL;
    
    int opcao, res;
    int cod, blocos, semanas, cod_d, bloco_d, carga;
    int cod_curso, ano, sem; 
    char nome[TAM_NOME];

    do {
        cabecalho();
        printf("  [1] Cadastrar Novo Curso\n");
        printf("  [2] Cadastrar Disciplina em Curso\n");
        printf("  [3] Cadastrar Novo Aluno\n");
        printf("  [4] Imprimir Árvore de Cursos\n");
        printf("  [5] Imprimir Dados de um Curso\n"); 
        printf("  [6] Imprimir Cursos com a mesma qtd de blocos\n");
        printf("  [7] Imprimir Árvore de Disciplinas\n");
        printf("  [8] Imprimir Dados de uma Disciplina\n");
        printf("  [9] Imprimir Disciplinas de um Bloco (Curso Específico)\n");
        printf("  [10] Imprimir Disciplinas com a mesma Carga Horária\n");
        printf("  [11] Listar Todos os Alunos\n");     
        printf("  [12] Listar Alunos de um Curso\n");         // Adicionado
        printf("  [13] Listar Alunos de um Curso por Ano de Ingresso\n"); // Adicionado
        printf("  [14] Contar Alunos em um Curso\n");        // Adicionado
        printf("  [15] Excluir Disciplina de um Curso\n");
        printf("  [16] Excluir Curso\n");
        printf("  [0] Sair do Sistema\n");
        printf("-----------------------------------------------------------\n");
        printf("  Selecione uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            while(getchar() != '\n'); 
            continue;
        }

        switch (opcao) {
            case 1:
                printf("\n>> FORMULARIO DE CURSO <<\n");
                printf("Codigo: "); 
                scanf("%d", &cod);

                if (buscar_no(raiz_cursos, cod, TIPO_CURSO) != NULL) {
                    printf("\n[ERRO] O curso %d ja esta cadastrado!\n", cod);
                    break;
                }

                printf("Nome do Curso: "); 
                scanf(" %[^\n]", nome);
                printf("Qtd. Blocos: "); 
                scanf("%d", &blocos);
                printf("Semanas/Semestre: "); 
                scanf("%d", &semanas);
                
                if (inserirCurso(&raiz_cursos, cod, nome, blocos, semanas))
                    printf("\n[OK] Curso registrado com sucesso!\n");
                else
                    printf("\n[ERRO] Falha na alocacao de memoria.\n");
                break;

            case 2:
                printf("\n/---------- VINCULAR DISCIPLINA ----------/\n");
                printf("Codigo do Curso: "); 
                scanf("%d", &cod_curso);

                RBNode* curso_ref = buscar_no(raiz_cursos, cod_curso, TIPO_CURSO);
                if (curso_ref == NULL) {
                    printf("\n[ERRO] Curso %d nao encontrado!\n", cod_curso);
                    break;
                }

                printf("Codigo da Disciplina: "); 
                scanf("%d", &cod_d);

                    // Verifica se a disciplina já existe antes de pedir o resto dos dados
                    if (buscar_no(curso_ref->info.curso.raiz_disciplinas, cod_d, TIPO_DISCIPLINA) != NULL) {
                        printf("\n[ERRO] A disciplina %d ja existe no curso %s!\n", cod_d, curso_ref->info.curso.nome_curso);
                    } else {
                        printf("Nome da Disciplina: "); 
                        scanf(" %[^\n]", nome);

                        int max_b = curso_ref->info.curso.qtd_blocos_curso;
                        int sem_ref = curso_ref->info.curso.semanas_disciplina;

                        // 1. Loop exclusivo para validar o Bloco
                        do {
                            printf("Bloco (0 a %d): ", max_b - 1);
                            scanf("%d", &bloco_d);
                            
                            if (!validarBloco(bloco_d, max_b)) {
                                printf("[!] Bloco fora do limite do curso. Tente novamente.\n");
                            }
                        } while (!validarBloco(bloco_d, max_b));

                        // 2. Loop exclusivo para validar a Carga Horária
                        do {
                            printf("Carga Horaria (Multiplo de %d): ", sem_ref);
                            scanf("%d", &carga);
                            
                            if (!validarCarga(carga, sem_ref)) {
                                printf("[!] Carga horaria deve ser multipla de %d. Tente novamente.\n", sem_ref);
                            }
                        } while (!validarCarga(carga, sem_ref));

                        // 3. Inserção Segura e Simplificada
                        res = inserirDisciplinaNoCurso(raiz_cursos, cod_curso, cod_d, nome, bloco_d, carga);
                        
                        if (res == 1) {
                            printf("\n[OK] Disciplina cadastrada com sucesso!\n");
                        } else if (res == -1) {
                            printf("\n[ERRO] Curso nao encontrado durante a insercao.\n");
                        } else {
                            printf("\n[ERRO] Falha ao cadastrar disciplina.\n");
                        }
                    }
                }
                break;

            case 3:
                printf("\n/---------- CADASTRO DE ALUNO ----------/\n");
                printf("Matricula: "); 
                scanf("%d", &cod); 

                if (buscar_no(raiz_alunos, cod, TIPO_ALUNO) != NULL) {
                    printf("\n[ERRO] Matricula %d ja existe!\n", cod);
                    break;
                }

                printf("Nome do Aluno: "); 
                scanf(" %[^\n]", nome);
                printf("Codigo do Curso: "); 
                scanf("%d", &cod_curso);

                if (buscar_no(raiz_cursos, cod_curso, TIPO_CURSO) == NULL) {
                    printf("\n[ERRO] Curso %d nao existe! Cadastre o curso primeiro.\n", cod_curso);
                    break;
                }

                printf("Ano de Ingresso: "); 
                scanf("%d", &ano);
                printf("Semestre (1 ou 2): "); 
                scanf("%d", &sem);

                if (inserirAluno(&raiz_alunos, cod, nome, cod_curso, ano, sem))
                    printf("\n[OK] Aluno cadastrado com sucesso!\n");
                else
                    printf("\n[ERRO] Falha ao cadastrar aluno.\n");
                break;

            case 4: // RELATÓRIO DE CURSOS
                printf("\n======= RELATORIO: CURSOS E DISCIPLINAS =======");
                if (raiz_cursos == NULL) {
                    printf("\nNenhum curso cadastrado.\n");
                } else {
                    imprimirArvoreCursos(raiz_cursos); 
                }
                break;

           case 5: 
                printf("\n=========== DADOS DE UM CURSO ============\n");
                if (raiz_cursos == NULL) {
                    // Cobre o cenário da árvore estar completamente vazia desde o início
                    printf("\nNenhum curso cadastrado.\n");
                } else {
                    printf("Codigo do Curso: ");
                    scanf("%d", &cod_curso);
                    // A própria função vai procurar. Se achar, imprime. Se não achar, dá o erro.
                    imprimirDadosCursos(raiz_cursos, cod_curso);
                }
                break;

            case 6: // Supondo que seja o case 6
                printf("\n=========== CURSOS COM MESMA QTD DE BLOCOS ============\n");
                
                int qtd_blocos_busca;
                printf("Digite a quantidade de blocos para comparar: ");
                scanf("%d", &qtd_blocos_busca);
                
                if (raiz_cursos == NULL) {
                    printf("\nNenhum curso cadastrado no sistema.\n");
                } else {
                    // Cria o sinalizador valendo 0
                    int achou_curso = 0; 
                    
                    // Passa o endereço (&) do sinalizador para a função
                    imprimirCursosComMesmaQtdBlocos(raiz_cursos, qtd_blocos_busca, &achou_curso);
                    
                    // Se depois de varrer a árvore inteira ele continuar 0, o curso não existe
                    if (achou_curso == 0) {
                        printf("\n[ERRO] Nenhum curso encontrado com %d blocos.\n", qtd_blocos_busca);
                    }
                }
                break;

            case 7:
                printf("\n=========== ARVORE DE DISCIPLINAS ============\n");
                if (raiz_cursos == NULL) {
                    printf("\nNenhum curso cadastrado, portanto nenhuma disciplina cadastrada.\n");
                } else {
                    printf("Codigo do Curso: ");
                    scanf("%d", &cod_curso);
                    imprimirArvoreDisciplinas(raiz_cursos, cod_curso); 
                }
                break;


            case 8:
                printf("\n=========== DADOS DE UMA DISCIPLINA ============\n");
                if (raiz_cursos == NULL) {
                    printf("\nNenhum curso cadastrado, portanto nenhuma disciplina cadastrada.\n");
                } else {
                    printf("Codigo do Curso: ");
                    scanf("%d", &cod_curso);
                    printf("Codigo da Disciplina: ");
                    scanf("%d", &cod_d);
                    imprimirDadosDisciplina(raiz_cursos, cod_curso, cod_d);
                }
                break;

                
            case 9:
                printf("\n=========== DISCIPLINAS DE UM BLOCO ============\n");
                if (raiz_cursos == NULL) {
                    printf("\nNenhum curso cadastrado, portanto nenhuma disciplina cadastrada.\n");
                } else {
                    printf("Codigo do Curso: ");
                    scanf("%d", &cod_curso);
                    printf("Numero do Bloco: ");
                    scanf("%d", &bloco_d);
                    imprimirDisciplinasPorBloco(raiz_cursos, cod_curso, bloco_d);
                }
                break;

            case 10:
                printf("\n=========== DISCIPLINAS COM MESMA CARGA HORARIA ============\n");
                if (raiz_cursos == NULL) {
                    printf("\nNenhum curso cadastrado, portanto nenhuma disciplina cadastrada.\n");
                } else {
                    printf("Codigo do Curso: ");
                    scanf("%d", &cod_curso);
                    printf("Carga Horaria: ");
                    scanf("%d", &carga);
                    imprimirDisciplinasPorCargaHoraria(raiz_cursos, cod_curso, carga);
                }
                break;
            
            case 11: // RELATÓRIO DE ALUNOS
                printf("\n=========== RELATORIO: ALUNOS ============ ");
                if (raiz_alunos == NULL) {
                    printf("\nNenhum aluno cadastrado.\n");
                } else {
                    imprimirAlunos(raiz_alunos); 
                }
                break;

            case 12: // LISTAR ALUNOS POR CURSO (Adicionado)
                printf("\n>> LISTAR ALUNOS POR CURSO (RUBRO-NEGRA) <<\n");
                printf("Codigo do Curso: "); 
                scanf("%d", &cod_curso);
                printf("\nAlunos do Curso %d:\n", cod_curso);
                listarAlunosPorCurso(raiz_alunos, cod_curso);
                break;

            case 13: // LISTAR ALUNOS POR CURSO E ANO (Adicionado)
                printf("\n>> LISTAR ALUNOS POR CURSO E ANO (RUBRO-NEGRA) <<\n");
                printf("Codigo do Curso: "); 
                scanf("%d", &cod_curso);
                printf("Ano de Ingresso: "); 
                scanf("%d", &ano);
                printf("\nAlunos do Curso %d ingressos em %d:\n", cod_curso, ano);
                listarAlunosPorCursoEAno(raiz_alunos, cod_curso, ano);
                break;

            case 14: // CONTAR ALUNOS NO CURSO (Adicionado)
                printf("\n>> QUANTIDADE DE ALUNOS NO CURSO (RUBRO-NEGRA) <<\n");
                printf("Codigo do Curso: "); 
                scanf("%d", &cod_curso);
                if (buscar_no(raiz_cursos, cod_curso, TIPO_CURSO) == NULL) {
                    printf("\n[ERRO] Curso %d nao encontrado!\n", cod_curso);
                } else {
                    res = contarAlunosNoCurso(raiz_alunos, cod_curso);
                    printf("\nTotal de alunos no curso %d: %d\n", cod_curso, res);
                }
                break;

            case 15:
              
                printf("\n/---------- EXCLUIR DISCIPLINA ----------/\n");
                printf("Codigo do Curso: ");
                scanf("%d", &cod_curso);
                
                printf("Codigo da Disciplina: ");
                scanf("%d", &cod_d);

                // Chama a função principal de exclusão
                res = excluirDisciplinaDoCurso(raiz_cursos, cod_curso, cod_d);
                
                // Trata as respostas mapeadas na sua função
                if (res == 1) {
                    printf("\n[OK] Disciplina removida com sucesso!\n");
                } else if (res == -1) {
                    printf("\n[ERRO] Curso %d nao encontrado!\n", cod_curso);
                } else if (res == -2) {
                    printf("\n[ERRO] A disciplina %d nao pertence ao curso %d ou nao existe.\n", cod_d, cod_curso);
                }
                break;
            
            case 16:
                printf("\n/---------- EXCLUIR CURSO ----------/\n");
                printf("Codigo do Curso: ");
                scanf("%d", &cod_curso);

                // Passamos &raiz_cursos (com o "e comercial") porque a raiz principal pode mudar!
                res = excluirCurso(&raiz_cursos, cod_curso);

                if (res == 1) {
                    printf("\n[OK] Curso removido com sucesso!\n");
                } else if (res == -1) {
                    printf("\n[ERRO] Curso %d nao encontrado!\n", cod_curso);
                } else if (res == -2) {
                    printf("\n[ERRO] O curso %d nao pode ser excluido pois possui disciplinas cadastradas.\n", cod_curso);
                    printf("       Remova todas as disciplinas deste curso primeiro.\n");
                }
                break;

            case 17: // AGORA AUTO-POVOADO E DIRETO!
                if (raiz_cursos == NULL) {
                    povoar_arvore_experimento(&raiz_cursos);
                }
                executar_experimento_30_cursos(raiz_cursos);
                break;

            case 0:
                printf("\nFinalizando sessao. Ate logo!\n");
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