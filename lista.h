#ifndef LISTA_H
#define LISTA_H

typedef struct no No;
typedef struct lista Lista;

struct lista {

	struct no *cabeca;
	struct no *rabo;
	int numPIDs;

};

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

No * criaNo();

Lista * criaLista();

Lista *insereNo(Lista * lst, pid_t pid, char *path, int tipo, int prioridade, int numBilhetes);

pid_t retiraNo(Lista *lst) ;

void liberaLista(Lista * lst) ;

#endif