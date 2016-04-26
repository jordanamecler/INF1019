#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "lista.h"
#include "escalonador.h"

#define TIME_SHARE = 0.5

struct escalonador {

	Lista *listaPrioridade;
	Lista *listaRoundRobin;
	Lista *listaLoteria;

};

Escalonador *criaEscalonador() {
	Escalonador *esc = (Escalonador *)malloc(sizeof(Escalonador));
	esc->listaPrioridade = criaLista();
	esc->listaRoundRobin = criaLista();
	esc->listaLoteria = criaLista();
	return esc;
}

Escalonador *insereProcesso(Escalonador *esc, pid_t pid, char *path, int tipo, int prioridade, int numBilhetes) {
	
	if (tipo == 0) {
		esc->listaRoundRobin = insereNo(esc->listaPrioridade, pid, path, tipo, prioridade, numBilhetes);
	}
	else if (tipo == 1) {
		esc->listaPrioridade = insereNo(esc->listaPrioridade, pid, path, tipo, prioridade, numBilhetes);
	}
	else if (tipo == 2) {
		esc->listaLoteria = insereNo(esc->listaPrioridade, pid, path, tipo, prioridade, numBilhetes);
	}

	return esc;

}

pid_t retiraPID(Escalonador *esc, int tipo) {

	if (tipo == 0) {
		return retiraNo(esc->listaRoundRobin);
	}
	else if (tipo == 1) {
		return retiraNo(esc->listaPrioridade);
	}
	else if (tipo == 2) {
		return retiraNo(esc->listaLoteria);
	}
	return -1;
}

void liberaEscalonador(Escalonador *esc) {
	liberaLista(esc->listaRoundRobin);
	liberaLista(esc->listaPrioridade);
	liberaLista(esc->listaLoteria);
	free(esc);
}
