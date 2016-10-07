/* 5) Faça um programa que leia 2 números e imprima o resultado das 4 operações básicas sobre estes 2 números. 
Verifique o que acontece se o 2º. número da entrada for 0 (zero). 
Capture o sinal de erro de floating point (SIGFPE) e repita a experiência anterior  */
// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 5

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void errorHandler(int signal);

int main() {

	int num1, num2;
	int soma, subtracao, multiplicacao, divisao;

	signal(SIGFPE, errorHandler);
	
	printf("Digite dois numeros \n");
	scanf("%f %f", &num1, &num2);
	
	soma = num1 + num2;
	subtracao = num1 - num2;
	multiplicacao = num1 * num2;
	divisao = num1 / num2;
	
	printf("%d \n", soma);
	printf("%d \n", subtracao);
	printf("%d \n", multiplicacao);
	printf("%d \n", divisao);


	return 0;
}

void errorHandler(int signal) {
	printf("Oops, divisão por 0!\n");
	exit(-1);
}

/* Execução do programa: gcc aula3ex5.c -o aula3ex5
./aula3ex5
   
   Saída: 
Digite dois numeros 
5 0
Floating exception (core dumped)

   Saída: 
Digite dois numeros 
5 0
Oops, divisão por 0!

   Reflexão: Ao passarmos o segundo numero 0, obtivemos um floating exception. Ao utilizarmos um novo handler para o SIGFPE, a mensagem do novo handler é exibida e o programa acaba, como esperado. Percebemos que ao utilizar float para os números, ao invés de uma exceção, o resultado da divisão é inf (infinito) e por isso não é enviado o sinal, e portanto não entra no novo handler.
*/

/* 6) Faça um programa para monitorar e informar o preço de chamadas telefônicas. O programa deverá ser executado em background.
O início e o término de uma chamada são informados através dos sinais SIGUSR1 e SIGUSR2, respectivamente. 
O custo da ligação é de 2 centavos por segundo, para ligações de até 1 minuto ou de 1 centavo por segundo a partir do 2º. minuto, ou seja, uma ligação de 1m30s custa R$1,50. */
// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 6

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void inicioHandler(int signal);
void terminoHandler(int signal);

time_t inicio = 0, final = 0;

int main(int argc, char *argv[]) {

	pid_t pid;
	
	signal(SIGUSR1, inicioHandler);
	signal(SIGUSR2, terminoHandler);
	
	if ((pid = fork()) < 0) {
		printf("Erro ao criar processo filho.\n");
		exit(1);
	}
	else if (pid == 0) {
		while(1) {
			//Esperando ligacao acabar
			
		}
	}
	else {
		kill(pid, SIGUSR1);
		int tempo;
		sscanf(argv[1], "%d", &tempo);
		sleep(tempo);
		kill(pid, SIGUSR2);
		int status;
		wait(&status);
	}
	
	return 0;
}

void inicioHandler(int signal) {
	inicio = time(NULL);
}
void terminoHandler(int signal) {
	final = time(NULL);
	long int tempoChamada = final- inicio;
	printf("Tempo da chamada: %ld \n", tempoChamada);
	long int custo;
	if(tempoChamada <= 60) {
		custo = tempoChamada*2;
	}
	else {
		custo = 60*2 + (tempoChamada - 60);
	}
	
	printf("Custo R$: %ld,%.2ld\n", custo/100, custo%100);
	
	exit(1);
}

/* Execução do programa: gcc aula3ex6.c -o aula3ex6
   
./aula3ex6 2
   Saída: 
Tempo da chamada: 2 
Custo R$: 0,04

./aula3ex6 70
   Saída: 
Tempo da chamada: 70 
Custo R$: 1,30


   Reflexão: Começamos criando um processo filho. O pai manda um sinal de início para o processo filho referente ao começo da ligação e depois um sinal de final para o processo filho. Entre esses dois sinais, o pai espera o tempo passado por argv de tempo de chamada. Ao receber um sinal de início, o filho, por meio do handler, guarda o tempo referente ao inicio da chamada. Ao receber um sinal de término, o filho, por meio do handler, guarda o tempo referente ao final da chamada, e calcula o custo baseado nas regras.
*/

/* 7) Elabore três programas I/O bound que não terminem (loop de mensagens no vídeo).
Elabore um programa que seja capaz de executar os três programas indicados anteriormente e que simule o compartilhamento da CPU entre os três processos com escalonamento Round-Robin com uma fatia de tempo de 1 segundo para o primeiro processo e 2 segundos para os demais processos. Execute os programas e relate o que aconteceu. */
// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 7

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


int main(int argc, char *argv[]) {

	pid_t pid1, pid2, pid3;
	pid_t vetorDeProcessos[3];
	
	if((pid1 = fork()) < 0) {
		printf("Erro no fork 1\n");
		exit(1);
	}
	else if(pid1 == 0) {
		execv("aula3ex7.1", NULL);
	}
	else {
		if((pid2=fork()) < 0) {
			printf("Erro no fork 1\n");
			exit(1);
		}
		else if(pid2 == 0) {
			execv("aula3ex7.2", NULL);
		}
		else {
			if((pid3 = fork()) < 0) {
				printf("Erro no fork 1\n");
				exit(1);
			}	
			else if(pid3 == 0) {
				execv("aula3ex7.3", NULL);
			}
			else {
				int status;
				
				vetorDeProcessos[0] = pid1;
				vetorDeProcessos[1] = pid2;
				vetorDeProcessos[2] = pid3;
				
				kill(pid1, SIGSTOP);
				kill(pid2, SIGSTOP);
				kill(pid3, SIGSTOP);
				
				while(1) {
					kill(vetorDeProcessos[0], SIGCONT);
					sleep(1);
					kill(vetorDeProcessos[0], SIGSTOP);
					kill(vetorDeProcessos[1], SIGCONT);
					sleep(2);
					kill(vetorDeProcessos[1], SIGSTOP);
					kill(vetorDeProcessos[2], SIGCONT);
					sleep(2);
					kill(vetorDeProcessos[2], SIGSTOP);					
				}

			}
		}
	}
	
	return 0;
}


/* Execução do programa: gcc aula3ex6.c -o aula3ex6
./aula3ex6
   
   Saída: 
Sequência infinita de prints de Processo 1, Processo 2 e Processo 3, sendo os prints do Processo 1 exibidos ao longo de 1 segundo, e o dos outros ao longo de 2 segundos.

   Reflexão: Criamos três processos filhos que chamam os três programas complementares respectivamente por meio do execv. Dessa forma o programa chamado substitui o processo filho. O pai primeiramente dá SIGSTOP em todos os processos. Depois, ele alterna a execução dos programas usando SIGSTOP e SIGCONT de acordo com o Round-Robin. Um sleep de 1 segundo é dado para que o processo 1 execute por 1 segundo, e os outros têm um sleep de 2 segundos para que executem por 2 segundos. Isso se repete infinitamente.
*/

// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 7.1

#include <stdio.h>


int main() {

	while(1) {
		printf("Processo 1.\n");
	}
	return 0;
}

// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 7.2

#include <stdio.h>


int main() {

	while(1) {
		printf("Processo 2.\n");
	}
	return 0;
}

// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 7.2

#include <stdio.h>


int main() {

	while(1) {
		printf("Processo 2.\n");
	}
	return 0;
}
