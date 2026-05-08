#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_vp.h"
#include "disciplina.h"

// Função auxiliar para validar as regras de inserção de disciplina
static int validarRegras(int bloco, int qtd_blocos_curso, int carga, int semanas) {
    int status = 1;// Assume que é válido até provar o contrário

    if (bloco >= qtd_blocos_curso || bloco < 0 || carga % semanas != 0 || carga <= 0) {
        status = 0;
    }

    return status;
}

int inserirDisciplinaNoCurso(RBNode* raiz_cursos, int cod_curso, int cod_disc, char nome[], int bloco, int carga) {
    int res = 0;
    
    // 1. Busca o curso
    RBNode* no_curso = buscar_no(raiz_cursos, cod_curso, TIPO_CURSO);
    
    if (no_curso == NULL) {
        res = -1; // Curso não encontrado
    } else {
        // 2. Valida as regras usando a função static acima
        if (!validarRegras(bloco, no_curso->info.curso.qtd_blocoscurso, carga, no_curso->info.curso.semanas_disciplina)) {
            res = -2; // Violação de regras
        } else {
            // 3. Verifica duplicata na árvore interna de disciplinas
            if (buscar_no(no_curso->info.curso.raiz_disciplinas, cod_disc, TIPO_DISCIPLINA) != NULL) {
                res = 0; // Já existe
            } else {
                // 4. Prepara a inserção
                Info info_disc;
                info_disc.disciplina.codigodisciplina = cod_disc;
                strcpy(info_disc.disciplina.nomedisciplina, nome);
                info_disc.disciplina.blocodisciplina = bloco;
                info_disc.disciplina.carga_horaria = carga;

                res = inserir_no((RBNode**)&(no_curso->info.curso.raiz_disciplinas), info_disc, TIPO_DISCIPLINA);
            }
        }
    }

    return res;
}