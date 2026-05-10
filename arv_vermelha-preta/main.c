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
                printf("Codigo: "); scanf("%d", &cod);
                printf("Nome do Curso: "); scanf(" %[^\n]", nome);
                printf("Qtd. Blocos: "); scanf("%d", &blocos);
                printf("Semanas/Semestre: "); scanf("%d", &semanas);
                
                if (inserirCurso(&raiz_cursos, cod, nome, blocos, semanas))
                    printf("\n[OK] Curso '%s' registrado com sucesso!\n", nome);
                else
                    printf("\n[ERRO] Codigo %d ja existe no sistema.\n", cod);
                break;

            case 2:
                printf("\n/---------- VINCULAR DISCIPLINA ----------/\n");
                printf("Codigo do Curso: "); 
                scanf("%d", &cod); 

                RBNode* curso_encontrado = buscar_no(raiz_cursos, cod, TIPO_CURSO);
                if (curso_encontrado == NULL) {
                    printf("\n O curso %d nao existe! Cadastre o curso primeiro.\n", cod);
                    break; 
                }

                printf("Curso: %s | Max Blocos: %d | Semanas: %d\n", 
                        curso_encontrado->info.curso.nome_curso,
                        curso_encontrado->info.curso.qtd_blocos_curso,
                        curso_encontrado->info.curso.semanas_disciplina);
                printf("-------------------------------------------\n");
                
                printf("Codigo da Disciplina: "); 
                scanf("%d", &cod_d);
                printf("Nome da Disciplina: "); 
                scanf(" %[^\n]", nome); 
                
                int limite_blocos = curso_encontrado->info.curso.qtd_blocos_curso;
                do {
                    printf("Bloco da Disciplina (Deve ser menor que %d): ", limite_blocos);
                    scanf("%d", &bloco_d);
                    if (bloco_d >= limite_blocos || bloco_d < 0) {
                        printf("Bloco invalido! Digite um valor entre 0 e %d.\n", limite_blocos - 1);
                    }
                } while (bloco_d >= limite_blocos || bloco_d < 0);
                
                int semanas_ref = curso_encontrado->info.curso.semanas_disciplina;
                do {
                    printf("Carga Horaria (Deve ser multiplo de %d): ", semanas_ref);
                    scanf("%d", &carga);
                    if (carga % semanas_ref != 0 || carga <= 0) {
                        printf("[ERRO] Valor incorreto! A carga horaria deve ser multipla de %d.\n", semanas_ref);
                    }
                } while (carga % semanas_ref != 0 || carga <= 0);

                res = inserirDisciplinaNoCurso(raiz_cursos, cod, cod_d, nome, bloco_d, carga);
                
                if (res == 1) 
                    printf("\n[OK] Disciplina '%s' cadastrada com sucesso!\n", nome);
                else 
                    printf("\n[ERRO] Esta disciplina ja existe neste curso.\n");
                break;

            case 3:
                printf("\n/---------- CADASTRO DE ALUNO ----------/\n");
                printf("Matricula: "); 
                scanf("%d", &cod); 

                if (buscar_no(raiz_alunos, cod, TIPO_ALUNO) != NULL) {
                    printf("[ERRO] Matricula ja cadastrada no sistema!\n");
                    break;
                }

                printf("Nome do Aluno: "); 
                scanf(" %[^\n]", nome);

                printf("Codigo do Curso: ");
                scanf("%d", &cod_curso);
                if (buscar_no(raiz_cursos, cod_curso, TIPO_CURSO) == NULL) {
                    printf("[ERRO] Curso %d nao existe! Cadastre o curso primeiro.\n", cod_curso);
                    break;
                }

                printf("Ano de Ingresso: "); scanf("%d", &ano);
                printf("Semestre (1 ou 2): "); scanf("%d", &sem);

                if (inserirAluno(&raiz_alunos, cod, nome, cod_curso, ano, sem))
                    printf("\n[OK] Aluno cadastrado com sucesso!\n");
                break;

            case 0:
                printf("\nFinalizando sessao. Ate logo, Flavio!\n");
                break;

            default:
                printf("\n[!] Opcao invalida. Tente novamente.\n");
        }

        printf("\nPressione ENTER para continuar...");
        getchar(); getchar(); 
        system("cls || clear"); 
    } while (opcao != 0);

    return 0;
}