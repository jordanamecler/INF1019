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

	No *lista_pids;
	int numPIDs;

}

Escalonador criaEscalonador(Escalonador esc) {
	esc->lista_pids = criaNo();
	esc->numPIDs = 0;
	return esc;
}

Escalonador inserePID(Escalonador esc, pid_t pid) {
	esc->lista_pids = insereNo(esc->lista_pids, pid);
	esc->numPIDs++;
}

Escalonador retiraPID(Escalonador esc, pid_t pid) {
	esc->lista_pids = retiraNo(esc->lista_pids, pid);
	esc->numPIDs--;
}

Escalonador liberaEscalonador(Escalonador esc) {
	liberaLista(esc->lista_pids);
	esc->numPIDs = 0;
}