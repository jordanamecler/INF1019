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

// nao ajeitei retira no ainda, entao comentei
// ficou complicado retirar por aqui, pois ou retornamos o pid e o nó se perde, ou retornamos o no e pid se perde
// isso pq nao temos mais cabeca da lista
// sepa retirmos o no na marra mesmo no escalonador.c

// No * retiraNo(No *p) {

// 	No *ant;
// 	pid_t pid;

// 	if ( p == NULL ) {
// 		return -1;
// 	}

// 	p->prox->ant = NULL;

// 	antigoRabo = lst->rabo;
// 	pid = antigoRabo->pid;
// 	lst->rabo = lst->rabo->ant;

// 	free(antigoRabo);
// 	lst->numPIDs--;
	
// 	return pid;
// }

void liberaLista(No * p) {
	
	while(p != NULL) {
		No * t = p->prox;
		free(p);
		p = t;
	}
 }