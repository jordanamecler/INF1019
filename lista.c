#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

struct lista {

	struct no *cabeca;
	struct no *rabo;
	int numPIDs;

}

struct no {

	pid_t pid;
	char *path;
	struct no * prox;
	struct no * ant;

}

Lista * criaLista() {
	
	Lista *nova = (Lista *)malloc(sizeof(Lista));
	nova->cabeca = NULL;
	nova->rabo = NULL;
	nova->numPIDs = 0;

	return nova;
}

No * criaNo() {
	return NULL;
}

Lista *insereNo(Lista * lst, pid_t pid, char *path) {

	No * novo = (No *)malloc(sizeof(No));
	novo->pid = pid;
	novo->path = path;
	novo->prox = lst->cabeca;
	novo->ant = NULL;

	if (lista != NULL) {
		novo->prox->ant = novo;	
	}

	lst->cabeca = novo;

	if (lst->rabo == NULL) {
		lst->rabo == lst->cabeca;
	}

	lst->numPIDs++;
	return lst;
}

pid_t retiraNo(Lista *lst) {

	No *antigoRabo;
	pid_t pid;

	if(lst->rabo == NULL) {
		return -1;
	}

	antigoRabo = lst->rabo;
	pid = antigoRabo->pid;
	lst->rabo = lst->rabo->ant;

	free(antigoRabo);
	lst->numPIDs--;
	
	return pid;
}

void liberaLista(Lista * lst) {
	No * p = lista->cabeca;
	while(p != NULL) {
		No * t = p->prox;
		free(p);
		p = t;
	}
 }