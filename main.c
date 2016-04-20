#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "escalonador.h"

int main() {

	FILE *execarq;
	char comando[5], path[15], argumento[15];
	int prioridade = 0, numTickets = 0;
	Escalonador *esc;
	pid_t pid1;

	execarq = fopen("exec.txt","r");
	if(execarq == NULL) {
		printf("Erro ao abrir execarq.txt\n");
		exit(1);
	}

	// Cria escalonador

	esc = criaEscalonador();

	// Interpretador

	while(fscanf(execarq, "%s %s %s", comando, path, argumento) == 3) {

		if (argumento[0] == 'r') {
			printf("Round-Robin\n\n");
		}
		else if (argumento[0] == 'p') {
		
			prioridade = argumento[11] - '0';
			printf("Prioridade %d\n\n", prioridade);

			inserePID(esc, 

		}
		else if (argumento[0] == 'n') {

			if( argumento[12] == '\0' ) {
				numTickets = argumento[11] - '0';
			}
			else {
				numTickets = (argumento[11] - '0') * 10 + argumento[12] - '0';	
			}
			printf("Sorteio de %d tickets\n\n", numTickets);
		}
		else {
			printf("Comando invalido.\n");
			exit(1);
		}
	}

		// if((pid1 = fork()) < 0) {
		// 	printf("Erro ao criar processo filho.\n");
		// 	exit(1);
		// }
		// else if(pid1 == 0) {
		// 	if(execv(path, NULL) < 0) {
		// 		printf("Erro ao executar o programa %s.\n", path);
		// 	}
		// }
		// else if(pid1 > 0) {
		// 	int status;

		// 	wait(&status);
		// 	printf("De volta ao pai.\n");
		// }


	fclose(execarq);

	return 0;
}