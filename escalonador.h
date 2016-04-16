#ifndef ESCALONADOR_H
#define ESCALONADOR_H

typedef struct escalonador Escalonador;

Escalonador criaEscalonador(Escalonador esc);

Escalonador inserePID(Escalonador esc, pid_t pid);

Escalonador retiraPID(Escalonador esc, pid_t pid);

Escalonador liberaEscalonador(Escalonador esc);

#endif