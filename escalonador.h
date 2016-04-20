#ifndef ESCALONADOR_H
#define ESCALONADOR_H

typedef struct escalonador Escalonador;

Escalonador criaEscalonador();

Escalonador inserePID(Escalonador esc, char *path);

pid_t retiraPID(Escalonador esc);

void liberaEscalonador(Escalonador esc);

#endif