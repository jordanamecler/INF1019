/*
	Jordana Mecler e Leonardo Wajnsztok
	
	Exercicio 1
*/

/* Exemplo de uso de semáforo*/ 
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 

union semun 
{ 
	int	val; 
	struct	semid_ds *buf; 
	ushort *array; 
};

// inicializa o valor do semáforo
int setSemValue(int	semId);
// remove o semáforo
void delSemValue(int semId);
// operação P
int semaforoP(int semId);
//operação V
int semaforoV(int semId);

int main (int argc, char * argv[]) { 
	int	i; 
	char letra = 'o'; 
	int	semId; 
	if(argc > 1) { 
		semId = semget (8752, 1, 0666 | IPC_CREAT); 
		setSemValue(semId); 
		letra = 'x'; 
		sleep (2); 
	} 
	else { 
		while((semId = semget (8752, 1, 0666)) < 0) { 
			putchar ('.'); 
			fflush(stdout); 
			sleep (1); 
		} 
	} 
	for(i=0; i<10; i++) { 
		semaforoP(semId); 
		putchar (toupper(letra)); 
		fflush(stdout); 
		sleep (rand() %3); 
		putchar (letra); fflush(stdout); 
		semaforoV(semId); 
		sleep (rand() %2); 
	} 
	printf ("\nProcesso %d terminou\n", getpid()); 
	if(argc > 1) { 
		sleep(10); 
		delSemValue(semId); 
	} 
	return 0; 
}
int setSemValue( int semId) {
	union semun semUnion; 
	semUnion.val = 1; 
	return semctl(semId, 0, SETVAL, semUnion); 
}

void delSemValue(int semId) { 
	union semun semUnion; 
	semctl(semId, 0, IPC_RMID, semUnion); 
}
int semaforoP(int semId) { 
	struct sembuf semB; 
	semB.sem_num = 0; 
	semB.sem_op = -1; 
	semB.sem_flg = SEM_UNDO; 
	semop(semId, &semB, 1); 
	return 0; 
}
int semaforoV(int semId) { 
	struct sembuf semB; 
	semB.sem_num = 0; 
	semB.sem_op = 1; 
	semB.sem_flg = SEM_UNDO; 
	semop(semId, &semB, 1); 
	return 0; 
} 

/*
./aula7.1 &
./aula7.1 1

Saida:
...........
OoOoXxOoXxOoXxOoXxOoXxXxOoXxOoXxOoXxOo
Processo 3088 terminou
X
[1]    Done                          ./aula7.1
x
Processo 3092 terminou

Explicacao:
Primeiro rodamos ./aula7.1.c & em background e esse programa vai imprimir '.' ate executarmos o proximo.Assim q executamos o proximo, os dois comecam a imprmir seus caracteres 'x' e 'o'. Cada vez que for imprmir,um programa da down no semaforo, imprime maiusculo, da sleep, imprime minusculo e depois da up no semaforo, quando termina da um pequeno sleep para o outro programa fazer a mesma rotina. 
*/


/*
	Jordana Mecler e Leonardo Wajnsztok
	
	Exercicio 2
*/

/* Exemplo de uso de semáforo*/ 
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

union semun 
{ 
	int	val; 
	struct	semid_ds *buf; 
	ushort *array; 
};

// inicializa o valor do semáforo
int setSemValue(int	semId);
// remove o semáforo
void delSemValue(int semId);
// operação P
int semaforoP(int semId);
//operação V
int semaforoV(int semId);

int main(int argc, char const *argv[])
{
	int	semId, i; 
	int pid;		
	int segmento;
	char *mensagem;
	segmento = shmget(6666, sizeof(char) * 17, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
	mensagem = (char *) shmat(segmento, 0, 0);
	mensagem[16] = '\0';
	mensagem[15] = '\0'; // reconhecer q esta cheio
	semId = semget (8752, 1, 0666 | IPC_CREAT); 
	setSemValue(semId); 

	
	if((pid = fork()) < 0) {
		printf("Erro no fork\n");
		shmdt(mensagem);
		shmctl(segmento, IPC_RMID, 0);
		delSemValue(semId); 
		exit(0);
	}
	else if(pid == 0) {
		//Produtor
		char leitura, lixo;
		int cont = 0;

		sleep(1);

		while(1) {

			semaforoP(semId); 

			printf("Digite um caracater: ");
			scanf("%c%c", &leitura, &lixo);
			mensagem[cont] = leitura;
			cont++;

			if (cont == 16) cont = 0;

			semaforoV(semId); 
			sleep(1);
		}


	}
	else {
		//Consumidor

		sleep(2);
		while(1) {

			semaforoP(semId); 
			fflush(stdout); 

			if(mensagem[15] != '\0') {
				printf("%s\n", mensagem);
				mensagem[15] = '\0';
			}
		
			semaforoV(semId); 
			sleep(2);
		}

	}
	

	shmdt(mensagem);
	shmctl(segmento, IPC_RMID, 0);
	delSemValue(semId); 

	return 0; 
}

int setSemValue( int semId) {
	union semun semUnion; 
	semUnion.val = 1; 
	return semctl(semId, 0, SETVAL, semUnion); 
}

void delSemValue(int semId) { 
	union semun semUnion; 
	semctl(semId, 0, IPC_RMID, semUnion); 
}
int semaforoP(int semId) { 
	struct sembuf semB; 
	semB.sem_num = 0; 
	semB.sem_op = -1; 
	semB.sem_flg = SEM_UNDO; 
	semop(semId, &semB, 1); 
	return 0; 
}
int semaforoV(int semId) { 
	struct sembuf semB; 
	semB.sem_num = 0; 
	semB.sem_op = 1; 
	semB.sem_flg = SEM_UNDO; 
	semop(semId, &semB, 1); 
	return 0; 
} 

/*

Saida:
Digite um caracater: l
Digite um caracater: e
Digite um caracater: o
Digite um caracater: n
Digite um caracater: a
Digite um caracater: r
Digite um caracater: d
Digite um caracater: o
Digite um caracater:  
Digite um caracater: e
Digite um caracater:  
Digite um caracater: l
Digite um caracater: e
Digite um caracater: g
Digite um caracater: a
Digite um caracater: l
leonardo e legal
Digite um caracater: 
...

Explicacao: Um processo eh criado com fork e este sera o produtor e o processo pai eh o consumidor.
			O processo fiho da down no semaforo e faz leitura de um caracter, o add no buffer na memoria compartilhada e da up no semaforo.
			O processo pai entao da down e ve se o buffer ja esta cheio, se estiver printa a mensagem, depois da down.
*/


/*
	Jordana Mecler e Leonardo Wajnsztok
	
	Exercicio 3.1
*/

/* Exemplo de uso de semáforo*/ 
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

union semun 
{ 
	int	val; 
	struct	semid_ds *buf; 
	ushort *array; 
};

// inicializa o valor do semáforo
int setSemValue(int	semId);
// remove o semáforo
void delSemValue(int semId);
// operação P
int semaforoP(int semId);
//operação V
int semaforoV(int semId);

int main(int argc, char const *argv[])
{
	int	semId, i; 
		
	int segmento, *soma;
	segmento = shmget(6666, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
	soma = (int *) shmat(segmento, 0, 0);

	if (argc == 1) *soma = 0;

	semId = semget (8752, 1, 0666 | IPC_CREAT); 
	setSemValue(semId); 

	sleep (2); 
	
	for (i = 0; i < 10; i++)
	{
		semaforoP(semId); 
		fflush(stdout); 
		*soma += 1;
		printf("%d\n", *soma);
		semaforoV(semId); 
		sleep (rand() %2); 
	}

	shmdt(soma);
	shmctl(segmento, IPC_RMID, 0);
	delSemValue(semId); 

	return 0; 
}
int setSemValue( int semId) {
	union semun semUnion; 
	semUnion.val = 1; 
	return semctl(semId, 0, SETVAL, semUnion); 
}

void delSemValue(int semId) { 
	union semun semUnion; 
	semctl(semId, 0, IPC_RMID, semUnion); 
}
int semaforoP(int semId) { 
	struct sembuf semB; 
	semB.sem_num = 0; 
	semB.sem_op = -1; 
	semB.sem_flg = SEM_UNDO; 
	semop(semId, &semB, 1); 
	return 0; 
}
int semaforoV(int semId) { 
	struct sembuf semB; 
	semB.sem_num = 0; 
	semB.sem_op = 1; 
	semB.sem_flg = SEM_UNDO; 
	semop(semId, &semB, 1); 
	return 0; 
} 

/*

Saida:

[1] 3452
1
2
3

4
5
10
11
16
21
22
23
24
29
30
35

[1]    Done                          ./aula7.3.1
40
45
50
55
60

Explicacao: Criamos uma variavel em memoria compartilhada em que os programas vao somar 1 e 5. Criamos um semaforo para os dois programas e dentro da regiao critica fizemos a soma a variavel. Dessa forma enquanto um programa esta somando, o outro fica em espera ate o outro dar up no semaforo e ele poder entrar e dar down.
*/


/*
	Jordana Mecler e Leonardo Wajnsztok
	
	Exercicio 3.2
*/

/* Exemplo de uso de semáforo*/ 
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

union semun 
{ 
	int	val; 
	struct	semid_ds *buf; 
	ushort *array; 
};
// inicializa o valor do semáforo
int setSemValue(int	semId);
// remove o semáforo
void delSemValue(int semId);
// operação P
int semaforoP(int semId);
//operação V
int semaforoV(int semId);


int main(int argc, char const *argv[])
{
	int	semId, i; 
		
	int segmento, *soma;
	segmento = shmget(6666, sizeof(int), IPC_EXCL | S_IRUSR | S_IWUSR );
	soma = (int *) shmat(segmento, 0, 0);



	semId = semget (8752, 1, 0666 | IPC_CREAT); 
	setSemValue(semId); 

	sleep (2); 
	
	for (i = 0; i < 10; i++)
	{
		semaforoP(semId); 
		fflush(stdout); 
		*soma += 5;
		printf("%d\n", *soma);
		semaforoV(semId); 
		sleep (rand() %2); 
	}


	delSemValue(semId); 


	shmdt(soma);
	shmctl(segmento, IPC_RMID, 0);

	return 0; 
}
int setSemValue( int semId) {
	union semun semUnion; 
	semUnion.val = 1; 
	return semctl(semId, 0, SETVAL, semUnion); 
}

void delSemValue(int semId) { 
	union semun semUnion; 
	semctl(semId, 0, IPC_RMID, semUnion); 
}
int semaforoP(int semId) { 
	struct sembuf semB; 
	semB.sem_num = 0; 
	semB.sem_op = -1; 
	semB.sem_flg = SEM_UNDO; 
	semop(semId, &semB, 1); 
	return 0; 
}
int semaforoV(int semId) { 
	struct sembuf semB; 
	semB.sem_num = 0; 
	semB.sem_op = 1; 
	semB.sem_flg = SEM_UNDO; 
	semop(semId, &semB, 1); 
	return 0; 
} 

