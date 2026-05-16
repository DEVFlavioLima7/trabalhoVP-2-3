#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas2-3.h"

void cabecalho() {
    printf("\n/***********************************************************/\n");
    printf(" SISTEMA ACADEMICO (ARVORE 2-3) - UFPI 2026   \n");
    printf(" GESTAO DE CURSOS, DISCIPLINAS E ALUNOS            \n");
    printf("/***********************************************************/\n");
}

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
        printf("  [4] Listar Cursos e Disciplinas\n"); 
        printf("  [5] Listar Todos os Alunos\n");     
        printf("  [6] Listar Alunos de um Curso\n");
        printf("  [7] Listar Alunos por Curso e Ano\n");
        printf("  [8] Contar Alunos em um Curso\n");
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

                // Como o nó pode ter 1 ou 2 cursos, localiza o ponteiro correto para a struct Curso
                Curso* curso_alvo = NULL;
                if (curso_ref->info[0].dado.curso.codigocurso == cod_curso) {
                    curso_alvo = &(curso_ref->info[0].dado.curso);
                } else {
                    curso_alvo = &(curso_ref->info[1].dado.curso);
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