#ifndef LISTA_H
#define LISTA_H

typedef struct no No;
typedef struct lista Lista;

No * criaNo();

Lista * criaLista();

Lista *insereNo(Lista * lst, pid_t pid);

pid_t retiraNo(Lista *lst, pid_t pid) ;

void liberaLista(Lista * lst) ;

#endif