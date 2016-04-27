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

void imprimeListaPrioridade(No * p) {

	No* ant;
	while( p != NULL ) {
		printf("PID: %d  prioridade:%d\n", p->pid, p->prioridade);
		ant = p;
		p = p->prox;
	}

	// Testar escadeamento da lista
	// while (ant != NULL) {
	// 	printf("PID: %d  prioridade:%d\n", ant->pid, ant->prioridade);
	// 	ant = ant->ant;
	// }

}

pid_t retiraNo(No ** lista) {

	pid_t pid;
	No * temp = *lista;

	if (*lista == NULL) {
		return -1;
	}

	pid = (*lista)->pid;
	if ((*lista)->prox == NULL) {
		free(*lista);
		*lista = NULL;
	}
	else {
		printf("iuUHDFSUAOIDHO");
		*lista = temp->prox;
		(*lista)->ant = NULL;
		free(temp);
		temp = NULL;

		// **lista = (*lista)->prox;
		// free((*lista)->ant);
		// (*lista)->ant = NULL;
	}

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

	while (tempLista->prox != NULL) {
		tempLista = tempLista->prox;
	}

	tempLista->prox = processo;
	processo->ant = tempLista;

	return processo;
}

void liberaLista(No * p) {
	
	while(p != NULL) {
		No * t = p->prox;
		free(p);
		p = t;
	}
 }