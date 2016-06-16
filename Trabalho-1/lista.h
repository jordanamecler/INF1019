#ifndef LISTA_H
#define LISTA_H

typedef struct no No;

struct no {

	pid_t pid;
	int tipo;
	int vBilhetes[20];
	int numBilhetes;
	int prioridade;
	char *path;
	struct no * prox;
	struct no * ant;

};

No *insereNo (No * p, pid_t pid, char *path, int tipo, int prioridade, int numBilhetes);

void imprimeListaLoteria(No * p);

void liberaLista (No * p) ;

void imprimeListaPrioridade(No * p);

#endif