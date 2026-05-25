#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas_vp.h"

void cabecalho() {
    printf("\n/***********************************************************/\n");
    printf(" SISTEMA ACADEMICO - UFPI 2026               \n");
    printf(" GESTAO DE CURSOS, DISCIPLINAS E ALUNOS            \n");
    printf("/***********************************************************/\n");
}

int main() {
    rb_node* raiz_cursos = NULL;
    rb_node* raiz_alunos = NULL;
    
    int opcao, res;
    int cod, blocos, semanas, cod_d, bloco_d, carga;
    int cod_curso, ano, sem; 
    char nome[TAM_NOME];

    do {
        cabecalho();
        printf("  [1] Cadastrar Novo Curso\n");
        printf("  [2] Cadastrar Disciplina em Curso\n");
        printf("  [3] Cadastrar Novo Aluno\n");
        printf("  [4] Listar Cursos e Disciplinas\n"); 
        printf("  [5] Listar Todos os Alunos\n");     
        printf("  [6] Listar Alunos de um Curso\n");         // Adicionado
        printf("  [7] Listar Alunos por Curso e Ano\n");     // Adicionado
        printf("  [8] Contar Alunos em um Curso\n");        // Adicionado
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

                int pode_cadastrar_curso = 1;
                if (buscar_no(raiz_cursos, cod, TIPO_CURSO) != NULL) {
                    printf("\n[ERRO] O curso %d ja esta cadastrado!\n", cod);
                    pode_cadastrar_curso = 0;
                }
                if (pode_cadastrar_curso) {
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
                }
                break;

            case 2:
                printf("\n/---------- VINCULAR DISCIPLINA ----------/\n");
                printf("Codigo do Curso: "); 
                scanf("%d", &cod_curso);

                rb_node* curso_ref = buscar_no(raiz_cursos, cod_curso, TIPO_CURSO);
                int pode_vincular_disc = 1;
                
                if (curso_ref == NULL) {
                    printf("\n[ERRO] Curso %d nao encontrado!\n", cod_curso);
                    pode_vincular_disc = 0;
                }
                
                if (pode_vincular_disc) {
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

                int pode_cadastrar_aluno = 1;
                if (buscar_no(raiz_alunos, cod, TIPO_ALUNO) != NULL) {
                    printf("\n[ERRO] Matricula %d ja existe!\n", cod);
                    pode_cadastrar_aluno = 0;
                }

                if (pode_cadastrar_aluno) {
                    printf("Nome do Aluno: "); 
                    scanf(" %[^\n]", nome);
                    printf("Codigo do Curso: "); 
                    scanf("%d", &cod_curso);

                    if (buscar_no(raiz_cursos, cod_curso, TIPO_CURSO) == NULL) {
                        printf("\n[ERRO] Curso %d nao existe! Cadastre o curso primeiro.\n", cod_curso);
                        pode_cadastrar_aluno = 0;
                    }
                }

                if (pode_cadastrar_aluno) {
                        printf("Ano de Ingresso: "); 
                        scanf("%d", &ano);
                        printf("Semestre (1 ou 2): "); 
                        scanf("%d", &sem);

                        if (inserirAluno(&raiz_alunos, cod, nome, cod_curso, ano, sem))
                            printf("\n[OK] Aluno cadastrado com sucesso!\n");
                        else
                            printf("\n[ERRO] Falha ao cadastrar aluno.\n");
                }
                break;

            case 4: // RELATÓRIO DE CURSOS
                printf("\n======= RELATORIO: CURSOS E DISCIPLINAS =======");
                if (raiz_cursos == NULL) {
                    printf("\nNenhum curso cadastrado.\n");
                } else {
                    imprimirCursos(raiz_cursos); 
                }
                break;

            case 5: // RELATÓRIO DE ALUNOS
                printf("\n=========== RELATORIO: ALUNOS ============");
                if (raiz_alunos == NULL) {
                    printf("\nNenhum aluno cadastrado.\n");
                } else {
                    imprimirAlunos(raiz_alunos); 
                }
                break;

            case 6: // LISTAR ALUNOS POR CURSO (Adicionado)
                printf("\n>> LISTAR ALUNOS POR CURSO (RUBRO-NEGRA) <<\n");
                printf("Codigo do Curso: "); 
                scanf("%d", &cod_curso);
                if (buscar_no(raiz_cursos, cod_curso, TIPO_CURSO) == NULL) {
                    printf("\n[ERRO] Curso %d nao encontrado!\n", cod_curso);
                } else {
                    printf("\nAlunos do Curso %d:\n", cod_curso);
                    listarAlunosPorCurso(raiz_alunos, cod_curso);
                }
                break;

            case 7: // LISTAR ALUNOS POR CURSO E ANO (Adicionado)
                printf("\n>> LISTAR ALUNOS POR CURSO E ANO (RUBRO-NEGRA) <<\n");
                printf("Codigo do Curso: "); 
                scanf("%d", &cod_curso);
                if (buscar_no(raiz_cursos, cod_curso, TIPO_CURSO) == NULL) {
                    printf("\n[ERRO] Curso %d nao encontrado!\n", cod_curso);
                } else {
                    printf("Ano de Ingresso: "); 
                    scanf("%d", &ano);
                    printf("\nAlunos do Curso %d ingressos em %d:\n", cod_curso, ano);
                    listarAlunosPorCursoEAno(raiz_alunos, cod_curso, ano);
                }
                break;

            case 8: // CONTAR ALUNOS NO CURSO (Adicionado)
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