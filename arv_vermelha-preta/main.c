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

                // BLINDAGEM: Verifica se o curso já existe
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

                // BLINDAGEM: Verifica se o curso destino existe
                RBNode* curso_ref = buscar_no(raiz_cursos, cod_curso, TIPO_CURSO);
                if (curso_ref == NULL) {
                    printf("\n[ERRO] Curso %d nao encontrado!\n", cod_curso);
                    break;
                }

                printf("Codigo da Disciplina: "); 
                scanf("%d", &cod_d);

                // BLINDAGEM: Verifica se a disciplina ja existe nesse curso
                if (buscar_no(curso_ref->info.curso.raiz_disciplinas, cod_d, TIPO_DISCIPLINA) != NULL) {
                    printf("\n[ERRO] A disciplina %d ja existe no curso %s!\n", cod_d, curso_ref->info.curso.nome_curso);
                    break;
                }

                printf("Nome da Disciplina: "); 
                scanf(" %[^\n]", nome);

                // TRAVAS DE REGRAS (Item C): Impede continuar com dados invalidos
                int max_b = curso_ref->info.curso.qtd_blocos_curso;
                do {
                    printf("Bloco (0 a %d): ", max_b - 1);
                    scanf("%d", &bloco_d);
                    if(bloco_d >= max_b || bloco_d < 0) printf("[!] Bloco fora do limite do curso.\n");
                } while (bloco_d >= max_b || bloco_d < 0);
                
                int sem_ref = curso_ref->info.curso.semanas_disciplina;
                do {
                    printf("Carga Horaria (Multiplo de %d): ", sem_ref);
                    scanf("%d", &carga);
                    if(carga % sem_ref != 0) printf("[!] Carga horaria deve ser multipla de %d.\n", sem_ref);
                } while (carga % sem_ref != 0 || carga <= 0);

                res = inserirDisciplinaNoCurso(raiz_cursos, cod_curso, cod_d, nome, bloco_d, carga);
                if (res == 1) printf("\n[OK] Disciplina cadastrada com sucesso!\n");
                break;

            case 3:
                printf("\n/---------- CADASTRO DE ALUNO ----------/\n");
                printf("Matricula: "); 
                scanf("%d", &cod); 

                // BLINDAGEM: Verifica se a matricula ja existe no sistema
                if (buscar_no(raiz_alunos, cod, TIPO_ALUNO) != NULL) {
                    printf("\n[ERRO] Matricula %d ja existe!\n", cod);
                    break;
                }

                printf("Nome do Aluno: "); 
                scanf(" %[^\n]", nome);
                printf("Codigo do Curso: "); 
                scanf("%d", &cod_curso);

                // BLINDAGEM: Verifica se o curso para o aluno existe
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