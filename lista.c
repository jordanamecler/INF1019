#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

struct no {

	pid_t pid;
	struct no * prox;

}

No * criaNo() {
	return NULL;
}

No *insereNo(No * lista, pid_t pid) {

	No * novo = (No *)malloc(sizeof(No));
	novo->pid = pid;
	novo->prox = lista;
	return novo;
}

No *retiraNo(No *lista, pid_t pid) {

	No * a = NULL;
	No * p = lista;

	while(p != NULL && p->pid != pid) {
		a = p;
		p = p->prox;
	}

	if(p == NULL) {
		return lista;
	}
	if(a == NULL) {
		lista = p->prox;
	}
	else {
		a->prox = p->prox;
	}

	free(p);
	return lista;
}

void liberaLista(No * lista) {
	No * p = lista;
	while(p != NULL) {
		No * t = p->prox;
		free(p);
		p = t;
	}
 }