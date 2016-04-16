#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "lista.h"
#include "escalonador.h"

int main() {

	FILE *entrada;
	char comando[5], path[15], argumento[15];
	int prioridade = 0, numTickets = 0;

	pid_t pid1;

	entrada = fopen("entrada.txt","r");
	if(entrada == NULL) {
		printf("Erro ao abrir entrada.txt\n");
		exit(1);
	}

	fscanf(entrada, "%s", comando);

	while(fscanf(entrada, "%s %s", path, argumento) == 2) {

		if(argumento[0] == 'e') {
			printf("%s %s\n", comando, path);
			printf("Round-Robin\n\n");
		}
		else {
			printf("%s %s %s\n", comando, path, argumento);
			if(argumento[0] == 'p') {

				prioridade = argumento[11] - '0';
				printf("Prioridade %d\n\n", prioridade);
			}	
			else if(argumento[0] == 'n') {
				numTickets = argumento[11] - '0';
				printf("Sorteio de %d tickets\n\n", numTickets);
			}
			else {
				printf("Comando invalido.\n");
				exit(1);
			}
			fscanf(entrada, "%s", comando);
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

	}

	fclose(entrada);

	return 0;
}