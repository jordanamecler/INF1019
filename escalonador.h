#ifndef ESCALONADOR_H
#define ESCALONADOR_H

typedef struct escalonador Escalonador;

Escalonador *criaEscalonador();

Escalonador *insereProcesso(Escalonador *esc, pid_t pid, char *path, int tipo, int prioridade, int numBilhetes);

pid_t retiraPID(Escalonador *esc, int tipo);

void liberaEscalonador(Escalonador *esc);

#endif
