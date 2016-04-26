#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

// int bilhetesUsados[20] = {0,0,0,0,0,0,0,0}

No *insereNo(No * p, pid_t pid, char *path, int tipo, int prioridade, int numBilhetes) {

	No * novo = (No *)malloc(sizeof(No));
	novo->pid = pid;
	novo->path = path;
	novo->ant = NULL;
	novo->tipo = tipo;
	novo->prioridade = prioridade;
	novo->numBilhetes = numBilhetes;

	if (p != NULL) {
		p->ant = novo;
	}	
	novo->prox = p;
	return novo;
}

No * retiraNo(No ** lista) {

	No *ant;

	if ( (*lista) == NULL ) {
		return -1;
	}

	ant = *lista;
	(*lista) = (*lista)->prox;
	free((*lista)->ant);

	return ant;
}

void liberaLista(No * p) {
	
	while(p != NULL) {
		No * t = p->prox;
		free(p);
		p = t;
	}
 }