#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>

#define CHAVE_INFO_FLAG 8662
#define CHAVE_TIPO 8663
#define CHAVE_NUM_TICKETS 8664
#define CHAVE_PRIORIDADE 8665
#define CHAVE_PATH 8666
#define CHAVE_END 8667

int main() {


	// Para rodar:

	// gcc -Wall teste1.c -o teste1
	// gcc -Wall escalonador.c lista.c -o escalonador
	// gcc -Wall main.c -o main
	// ./main


	FILE *execarq;
	char comando[5], argumento[15];
	int status;
	pid_t pidEscalonador;

	// Memoria compartilhada

	int segmentoEnd,segmentoNovaInfoFlag, segmentoPath, segmentoNumTickets, segmentoPrioridade, segmentoTipo;
	char *path; //[15];
	int *end,*numTickets, *prioridade, *tipo, *novaInfoFlag;

	// Aloca memoria compartilhada

	segmentoNovaInfoFlag = shmget(CHAVE_INFO_FLAG, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
	if( segmentoNovaInfoFlag < 0 ) { 
		printf(" erro ao criar segmento de novainfoflag\n");
		exit(1);
	}
	novaInfoFlag = (int *) shmat(segmentoNovaInfoFlag, 0, 0);

	segmentoEnd = shmget(CHAVE_END, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
	if( segmentoEnd < 0 ) { 
		printf(" erro ao criar segmento de end\n");
		exit(1);
	}
	end = (int *) shmat(segmentoEnd, 0, 0);

	segmentoPrioridade = shmget(CHAVE_PRIORIDADE, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
	if( segmentoPrioridade < 0 ) { 
		printf(" erro ao criar segmento de prioridade\n");
		exit(1);
	}
	prioridade = (int *) shmat(segmentoPrioridade, 0, 0);

	segmentoTipo = shmget(CHAVE_TIPO, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
	if( segmentoTipo < 0 ) { 
		printf(" erro ao criar segmento de tipo\n");
		exit(1);
	}
	tipo = (int *) shmat(segmentoTipo, 0, 0);

	segmentoNumTickets = shmget(CHAVE_NUM_TICKETS, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
	if( segmentoNumTickets < 0 ) { 
		printf(" erro ao criar segmento de numtickets\n");
		exit(1);
	}
	numTickets = (int *) shmat(segmentoNumTickets, 0, 0);

	segmentoPath = shmget(CHAVE_PATH, sizeof(char) * 15, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
	if( segmentoPath < 0 ) { 
		printf(" erro ao criar segmento de path\n");
		exit(1);
	}
	path = (char *) shmat(segmentoPath, 0, 0);
	*novaInfoFlag = 0;
	*end = 0;

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


	// Interpretador

	printf("O inicio dos tempos.\n\n");
	execarq = fopen("exec.txt","r");
	if(execarq == NULL) {
		printf("Erro ao abrir execarq.txt\n");
		exit(1);
	}

	while(fscanf(execarq, "%s %s %s", comando, path, argumento) == 3) {

		if (argumento[0] == 'r') {
			*tipo = 0;
			printf("Round-Robin\n\n");
		}
		else if (argumento[0] == 'p') {
			*tipo = 1;
			*prioridade = argumento[11] - '0';
			// printf("Prioridade %d\n\n", *prioridade);
		}
		else if (argumento[0] == 'n') {
			*tipo = 2;

			if( argumento[12] == '\0' ) {
				*numTickets = argumento[11] - '0';
			}
			else {
				*numTickets = (argumento[11] - '0') * 10 + argumento[12] - '0';	
			}
		}
		else {
			printf("Comando invalido.\n");
			exit(1);
		}

		*novaInfoFlag = 1;
		// TODO: LEVANTAR FLAG DE NOVAS INFORMACOES PARA ESCALONADOR LER E INSERIR!

		// informacoes ja compartilhadas, falta receber no escalonador e tratar!!!
		
		
		sleep(3);
	}

	*end = 1;

	// espera escalonador terminar tudo
	wait(&status);
	fclose(execarq);

	// libera memoria compartilhada do processo
	shmdt(tipo);
	shmdt(prioridade);
	shmdt(numTickets);
	shmdt(novaInfoFlag);
	shmdt(path);
	shmdt(end);

	// libera memoria compartilhada
	shmctl(segmentoPath, IPC_RMID, 0);
	shmctl(segmentoTipo, IPC_RMID, 0);
	shmctl(segmentoPrioridade, IPC_RMID, 0);
	shmctl(segmentoNumTickets, IPC_RMID, 0);
	shmctl(segmentoNovaInfoFlag, IPC_RMID, 0);
	shmctl(segmentoEnd, IPC_RMID, 0);


	printf("Main terminou de executar.\n");

	return 0;
}
