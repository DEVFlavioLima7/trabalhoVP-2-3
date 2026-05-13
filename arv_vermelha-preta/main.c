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
                printf("Nome do Curso: "); 
                scanf(" %[^\n]", nome);
                printf("Qtd. Blocos: "); 
                scanf("%d", &blocos);
                printf("Semanas/Semestre: "); 
                scanf("%d", &semanas);
                
                if (inserirCurso(&raiz_cursos, cod, nome, blocos, semanas))
                    printf("\n[OK] Curso registrado com sucesso!\n");
                else
                    printf("\n[ERRO] Codigo %d ja existe ou falha na memoria.\n", cod);
                break;

            case 2:
                printf("\n/---------- VINCULAR DISCIPLINA ----------/\n");
                printf("Codigo do Curso: "); 
                scanf("%d", &cod_curso);
                printf("Codigo da Disciplina: "); 
                scanf("%d", &cod_d);
                printf("Nome da Disciplina: "); 
                scanf(" %[^\n]", nome);
                printf("Bloco: "); 
                scanf("%d", &bloco_d);
                printf("Carga Horaria: "); 
                scanf("%d", &carga);

                res = inserirDisciplinaNoCurso(raiz_cursos, cod_curso, cod_d, nome, bloco_d, carga);
                
                if (res == 1) 
                    printf("\n[OK] Disciplina cadastrada com sucesso!\n");
                else if (res == -1)
                    printf("\n[ERRO] Curso %d nao encontrado.\n", cod_curso);
                else if (res == -2)
                    printf("\n[ERRO] Regras violadas (Bloco invalido ou Carga horaria nao permitida).\n");
                else
                    printf("\n[ERRO] Disciplina ja existe neste curso.\n");
                break;

            case 3:
                printf("\n/---------- CADASTRO DE ALUNO ----------/\n");
                printf("Matricula: "); 
                scanf("%d", &cod); 
                printf("Nome do Aluno: "); 
                scanf(" %[^\n]", nome);
                printf("Codigo do Curso: "); 
                scanf("%d", &cod_curso);
                printf("Ano de Ingresso: "); 
                scanf("%d", &ano);
                printf("Semestre (1 ou 2): "); 
                scanf("%d", &sem);

                
                if (buscar_no(raiz_cursos, cod_curso, TIPO_CURSO) == NULL) {
                    printf("\n[ERRO] Curso %d nao existe!\n", cod_curso);
                } else {
                    if (inserirAluno(&raiz_alunos, cod, nome, cod_curso, ano, sem))
                        printf("\n[OK] Aluno cadastrado com sucesso!\n");
                    else
                        printf("\n[ERRO] Matricula ja cadastrada.\n");
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