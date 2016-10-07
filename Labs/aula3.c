/* 1) Execute o programa “ctrl-c.c”. Digite Ctrl-C e Ctrl-\. Analise o resultado. */
// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 1

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define EVER ;;

void intHandler(int signal);
void quitHandler(int signal);

int main() {

	void (*p)(int);
	
	//p = signal(SIGINT, intHandler);
	//printf("Endereco do manipulador anterior %p\n", p);
	//p = signal(SIGQUIT, quitHandler);
	//printf("Endereco do manipulador anterior %p\n", p);
	puts("Ctrl-C desabilitado. Use Ctrl-\\ para terminar \n");
	for(EVER);
}
void intHandler(int sinal) {
	printf("Voce pressionou Ctrl-C (%d) \n", sinal);
}
void quitHandler(int sinal) {
	puts("Terminando o processo... \n");
	exit(0);
}

/* Execução do programa: gcc aula3ex1.c -o aula3ex1
./aula3ex1
   
   Saída: 
Endereco do manipulador anterior (nil)
Endereco do manipulador anterior (nil)
Ctrl-C desabilitado. Use Ctrl-\ para terminar
^CVoce pressionou Ctrl-C (%d), sinal
^\Terminando o processo...

   Reflexão: O comando signal substitui o handler default pelos que nós definimos. O input do Ctrl-C ao invés de terminar imediatamente o processo, apenas imprime uma string no terminal. Ao comentar os comandos signal(), os handlers voltam a ser os default, sendo ambos o término imediato do processo.
*/

/* 2) Tente fazer um programa para interceptar o sinal SIGKILL. Você conseguiu? Explique. */
// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 2

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define EVER ;;

void killHandler(int signal);
void quitHandler(int sinal);

int main() {

	void (*p)(int);
	
	p = signal(SIGKILL, killHandler);
	printf("Endereco do manipulador anterior %p\n", p);
	puts("kill desabilitado. Use Ctrl-\\ para terminar \n");
	p = signal(SIGQUIT, quitHandler);
	printf("Endereco do manipulador anterior %p\n", p);
	
	for(EVER);
}
void killHandler(int signal) {
	printf("Vamos testar :)");
}
void quitHandler(int sinal) {
	puts("Terminando o processo... \n");
	exit(0);
}

/* Execução do programa: gcc aula3ex2.c -o aula3ex2
./aula3ex2 &
   
   Saída: 
[9] 19155
[prata.grad.inf.puc-rio.br:~/inf1019] Endereco do manipulador anterior 0xffffffffffffffff
kill desabilitado. Use Ctrl-\ para terminar 

Endereco do manipulador anterior (nil)

kill -s KILL 19155

[9]    Killed                        ./aula3ex2

   Reflexão: Percebemos que não é possível modificar o handler do SIGKILL. Nós tentamos alterar o handler do SIGKILL. Rodamos o programa em background e chamamos o comando "kill -s KILL 19155", com KILL no lugar de SIGKILL pois era o aceito pela nossa máquina. Ao chamarmos esse comando, imediatamente o processo foi morto, não utilizando o novo handler que havíamos colocado.
*/

/* 3) Execute e explique o funcionamento de filhocidio.c */
// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 3

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void childHandler(int signo);
int delay;
int tempo;
int main(int argc, char *argv[]) {
	pid_t pid;
	signal(SIGCHLD, childHandler);
	if((pid = fork()) < 0) {
		fprintf(stderr, "Erro ao criar filho\n");
		exit(-1);
	}
	if(pid == 0) {
		sscanf(argv[2], "%d", &tempo);
		sleep(tempo);
		printf("Acordei!");
	}
	else {
		sscanf(argv[1], "%d", &delay);
		sleep(delay);
		printf("Program exceeded limit of %d seconds!\n", delay);
		kill(pid, SIGKILL);
	}
	return 0;
}
void childHandler(int signo) {
	int status;
	pid_t pid = wait(&status);
	printf("Child %d terminated within %d seconds com estado %d.\n", pid, delay, status);
	exit(0);
}

/* Execução do programa: gcc aula3ex3.c -o aula3ex3
./aula3ex3 10 5
   
   Saída: 
Acordei!Child 19015 terminated within 10 seconds com estado 0.

./aula3ex3 10 15
	Saída:
Program exceeded limit of 10 seconds!

   Reflexão: Um novo handler foi definido para o SIGCHLD, onde caso o processo filho demorasse menos que o limite definido pelo processo pai para acordar, seria impresso o pid do processo filho. Caso o processo filho demorasse mais que o limite definido pelo processo pai para acordar, o pai mataria o filho. O tempo limite definido foi 10. Sendo assim, quando passamos um tempo de 5 segundos para o filho dormir, o processo conseguiu terminar dentro do tempo limite. QUando passamos um tempo de 15 segundos, o processo foi morto pelo pai.
*/

/* 4) Usando SIGSTOP e SIGCONT faça um programa que crie 2 processos filho e alterne a execução dos filhos. Após  10 trocas de contexto, o processo pai mata os processos filho.  
OBS: Os processos filho são formados por loops infinitos. */
// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 4

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	pid_t pid1;
	pid_t pid2;
	pid_t pid_aux;
	int i;

	if((pid1 = fork()) < 0) {
		fprintf(stderr, "Erro ao criar filho\n");
		exit(-1);
	}
	else if(pid1 == 0) {
		printf("cheguei processo 1\n");
		while(1) {
			printf("To no processo 1\n");
		}
	}
	else {
		if((pid2 = fork()) < 0) {
			fprintf(stderr, "Erro ao criar filho\n");
			exit(-1);
		}
		if(pid2 == 0) {
			printf("cheguei processo 2\n");
			while(1) {
				printf("To no processo 2\n");
			}				
		}
		else {
			kill(pid1, SIGSTOP);
			kill(pid2, SIGSTOP);
			for(i=0;i<10;i++) {
				printf("TO AQUI\n");
				if(i!=0) {
					kill(pid1, SIGSTOP);
				}
				sleep(1);
				kill(pid2, SIGCONT);	
				sleep(1);
				pid_aux = pid1;
				pid1 = pid2;
				pid2 = pid_aux;
			}
			kill(pid1, SIGKILL);
			kill(pid2, SIGKILL);	
			exit(1);
			}
		}


	return 0;
}

/* Execução do programa: gcc aula3ex4.c -o aula3ex4
./aula3ex4
   
   Saída: 
A saida foi a alternancia de "To no processo 1" e "To no processo 2" a cada passagem de "TO AQUI". Foram muitas impressoes.

   Reflexão: Nos fizemos dois forks para criar os 2 processos filhos e demos stop nos processos filhos dentro do processo pai. O processo pai entao faz a troca de um processo stop para cont e vice versa. Quando terminam as 10 vezes de trocas, o pai mata os dois processos filhos. Ficamos um bom tempo nao entendendo porque nao estavam sendo impressas as mensagens dos processos filhos, e descobrimos que colocando um sleep era possivel visualizar. Sem o sleep ocorria de forma muito rapida.
*/
