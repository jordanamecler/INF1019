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

pid_t retiraNo(No ** lista) {

	pid_t pid;

	if (*lista == NULL) {
		return -1;
	}

	pid = (*lista)->pid;
	*lista = (*lista)->prox;

	free((*lista)->ant);

	return pid;
}

No *realocaNo (No **lista) {
	No *processo;
	No *tempLista;

	if (*lista == NULL) {
		return NULL;
	}
	processo = *lista;
	*lista = (*lista)->prox;
	processo->prox = NULL;
	(*lista)->ant = NULL;

	tempLista = *lista;

	while ((*tempLista)->prox != NULL) {
		tempLista = tempLista->prox;
	}

	tempLista->prox = processo;
	processo->ant = tempLista->prox;

	return processo;
}

void liberaLista(No * p) {
	
	while(p != NULL) {
		No * t = p->prox;
		free(p);
		p = t;
	}
 }