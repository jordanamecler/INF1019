#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "lista.h"

#define TIME_SHARE = 0.5

No *listaPrioridade = NULL;
No *listaRoundRobin = NULL;
No *listaLoteria = NULL;


void insereProcesso(char *path, int tipo, int prioridade, int numBilhetes) {
	
	pid_t pid = 0;

	// Cria processo filho que será substituido por programa em 'path'
	if((pid = fork()) < 0) {
		printf("Erro ao criar processo filho.\n");
		exit(1);
	}
	else if(pid == 0) {
		if(execv(path, NULL) < 0) {
			printf("Erro ao executar o programa %s.\n", path);
		}
	}
	else if(pid > 0) {
		kill(pid, SIGSTOP);
		printf("Escalonador criou processo filho %s.\n", path);
	}


	if (tipo == 0) {
		listaRoundRobin = insereNo(listaRoundRobin, pid, path, tipo, prioridade, numBilhetes);
	}
	else if (tipo == 1) {

		// Procura no com prioridade menor (ex:  prioridade 3 < prioridade 2) e insere antes dele
		printf("tipo Prioridade\n");
		No * p = listaPrioridade;
		No * ant = NULL;
		No * novo;

		if (p != NULL) {
			while(p->prioridade <= prioridade){
				p = p->prox;
			}
			printf("achou no\n");
			ant = p->ant;	
		}
		
		novo = insereNo(p, pid, path, tipo, prioridade, numBilhetes);
		novo->ant = ant;
	}
	else if (tipo == 2) {
		listaLoteria = insereNo(listaLoteria, pid, path, tipo, prioridade, numBilhetes);
	}
}

pid_t retiraPID (int tipo) {

	if (tipo == 0) {
		return retiraNo(&listaRoundRobin);
	}
	else if (tipo == 1) {
		return retiraNo(&listaPrioridade);
	}
	else if (tipo == 2) {
		return retiraNo(&listaLoteria);
	}
	return -1;
}

void realocaProcesso (int tipo) {

	if (tipo == 0) {
		No *processoRealocado = realocaNo(&listaRoundRobin);
	}
	else if (tipo == 1) {
		retiraNo(&listaPrioridade);
	}
	else if (tipo == 2) {
		retiraNo(&listaLoteria);
	}
	else {
		return;
	}
}

void liberaEscalonador() {
	liberaLista(listaRoundRobin);
	liberaLista(listaPrioridade);
	liberaLista(listaLoteria);
}

int main() {

	No *listaPrioridade = NULL;
	No *listaRoundRobin = NULL;
	No *listaLoteria = NULL;

	// aqui vamos ter q ficar rodando esperando informacoes novas a partir do flag q a main disparar
	// quando ler coisas nova insere em alguma lista
	// se nao le nada novo, continua escalonando todos os processos até eles terminarem

	// while(1) {

		printf("escalonador esperando...\n");

	// }

	liberaEscalonador();
	return 0;
}
