#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

	FILE *entrada;
	char comando[5], path[15], prioridade_string[15];
	int prioridade = 0;
	pid_t pid1;

	entrada = fopen("entrada.txt","r");
	if(entrada == NULL) {
		printf("Erro ao abrir entrada.txt\n");
		exit(1);
	}

	while(fscanf(entrada, "%s", comando) == 1) {
		fscanf(entrada, "%s %s", path, prioridade_string);

		printf("%s %s %s\n", comando, path, prioridade_string);


		prioridade = prioridade_string[11] - '0';

		if((pid1 = fork()) < 0) {
			printf("Erro ao criar processo filho.\n");
			exit(1);
		}
		else if(pid1 == 0) {
			if(execv(path, NULL) < 0) {
				printf("Erro ao executar o programa %s.\n", path);
			}
		}
		else if(pid1 > 0) {
			int status;

			wait(&status);
			printf("De volta ao pai.\n");
		}

	}

	fclose(entrada);

	return 0;
}