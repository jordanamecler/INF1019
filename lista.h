#ifndef LISTA_H
#define LISTA_H

typedef struct no No;

No * criaNo();

No *insereNo(No * lista, pid_t pid);

No *retiraNo(No *lista, pid_t pid);

void liberaLista(No * lista);

#endif