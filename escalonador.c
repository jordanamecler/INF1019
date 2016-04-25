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

	Lista *listaProcessos;

}

Escalonador criaEscalonador() {
	Escalonador *esc = (Escalonador *)malloc(sizeof(Escalonador));
	esc->listaProcessos = criaLista();
	return esc;
}

Escalonador insereProcesso(Escalonador esc, char *path, int tipo) {
	// fazer as paradas
	pid_t pid = 0;
	esc->listaProcessos = insereNo(esc->listaProcessos, pid, path);

}

pid_t retiraPID(Escalonador esc) {
	return retiraNo(esc->listaProcessos);
}

void liberaEscalonador(Escalonador esc) {
	liberaLista(esc->listaProcessos);
	free(esc);
}
