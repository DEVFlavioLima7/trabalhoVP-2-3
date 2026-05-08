#ifndef ARVORE_VP_H
#define ARVORE_VP_H
#include "estruturas_vp.h"


RBNode* criar_no(Info info, TipoInfo tipo);
int obter_cor(RBNode* no);
void trocar_cores(RBNode* H);

RBNode* rotar_esquerda(RBNode* A);
RBNode* rotar_direita(RBNode* A);


RBNode* balancear(RBNode* H);


int inserir_no(RBNode** raiz, Info info, TipoInfo tipo);


RBNode* buscar_no(RBNode* raiz, int chave, TipoInfo tipo);

#endif