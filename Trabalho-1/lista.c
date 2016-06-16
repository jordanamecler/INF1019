#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lista.h"

#define MAX_BILHETES 20

int bilhetesUsados[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

No *insereNo(No * p, pid_t pid, char *path, int tipo, int prioridade, int numBilhetes) {

	No * novo = (No *)malloc(sizeof(No));
	novo->pid = pid;
	novo->path = path;
	novo->ant = NULL;
	novo->prox = NULL;
	novo->tipo = tipo;
	novo->prioridade = prioridade;
	novo->numBilhetes = numBilhetes;

	// Insere no final
	if (tipo == 0) {
		No * temp = p;
		if (temp != NULL) {
			while (temp->prox != NULL) {
				temp = temp->prox;
			}
			temp->prox = novo;
			novo->ant = temp;
			return p;
		}
		else {
			return novo;
		}
	}
	// Insere no inicio
	else if (tipo == 1) {
		if (p != NULL) {
			p->ant = novo;
		}	
		novo->prox = p;
	}
	else if (tipo == 2) {
		int i, r;
		No * temp = p;
		srand((unsigned int)time(NULL));

		// zera vBilhetes

		for( i = 0; i < MAX_BILHETES; i++) {
			novo->vBilhetes[i] = 0;
		}
		// gera bilhetes 

		for(i = 0; i < numBilhetes; i++){
			do {
				r = rand() % MAX_BILHETES;
			}
			while( bilhetesUsados[r] == 1 );
			
			bilhetesUsados[r] = 1;
			novo->vBilhetes[r] = 1;
			printf("bilhetes premiados %d\n",r);
		}
		// insere no final

		if (temp != NULL) {
			while (temp->prox != NULL) {
				temp = temp->prox;
			}
			temp->prox = novo;
			novo->ant = temp;
			return p;
		}
		else {
			return novo;
		}


	}

	return novo;
}

void imprimeListaPrioridade(No * p) {

	No* ant;
	while( p != NULL ) {
		printf("PID: %d  prioridade:%d\n", p->pid, p->prioridade);
		ant = p;
		p = p->prox;
	}
}

void imprimeListaLoteria(No * p) {

	No* ant;
	int i;

	while( p != NULL ) {
		printf("PID: %d", p->pid);
		for ( i = 0; i < MAX_BILHETES; ++i) {
			if(p->vBilhetes[i] == 1) {
				printf(" %d", i);
			}
		}
		printf("\n");
		ant = p;
		p = p->prox;
	}
}

void liberaLista(No * p) {
	
	while(p != NULL) {
		No * t = p->prox;
		free(p);
		p = t;
	}
 }