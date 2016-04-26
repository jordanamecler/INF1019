#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {


	// Para rodar:

	// gcc -Wall escalonador.c lista.c -o escalonador
	// gcc -Wall main.c -o main
	// ./main


	FILE *execarq;
	char comando[5], path[15], argumento[15];
	int prioridade = 0, numTickets = 0;
	int tipo, status;
	pid_t pidEscalonador;

	printf("O inicio dos tempos.\n");
	execarq = fopen("exec.txt","r");
	if(execarq == NULL) {
		printf("Erro ao abrir execarq.txt\n");
		exit(1);
	}

	// Cria escalonador

	printf("Vamos criar o escalonador.\n");

	if((pidEscalonador = fork()) < 0) {
		printf("Erro ao criar processo escalonador filho.\n");
		exit(1);
	}
	else if(pidEscalonador == 0) {
		if(execv("escalonador", NULL) < 0) {
			printf("Erro ao executar o programa escalonador.\n");
		}
	}
	else if(pidEscalonador > 0) {
		printf("Escalonador criado.\n");
	}

	// TODO: CRIAR MEMORIA COMPARTILHADA PARA PASSAR INFO PRO ESCALONADOR

	// Interpretador

	while(fscanf(execarq, "%s %s %s", comando, path, argumento) == 3) {

		if (argumento[0] == 'r') {
			tipo = 0;
			printf("Round-Robin\n\n");
		}
		else if (argumento[0] == 'p') {
			tipo = 1;
			prioridade = argumento[11] - '0';
			printf("Prioridade %d\n\n", prioridade);

		}
		else if (argumento[0] == 'n') {
			tipo = 2;

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

		// TODO: ENVIAR AQUI INFORMACOES PARA ESCALONADOR POR MEMORIA COMPARTILHADA, LEVANTAR FLAG DE NOVAS INFORMACOES PARA ESCALONADOR LER E INSERIR!
		prioridade = 0;
		numTickets = 0;
		sleep(3);
	}

	// espera escalonador terminar tudo
	wait(&status);
	fclose(execarq);

	printf("Escalonador terminou de executar todos programas.\n");

	return 0;
}
