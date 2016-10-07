// Jordana Mecler e Leonardo Wajnsztok

/* 1) Faça um programa para criar dois processos, o pai escreve seu pid e espera o filho terminar e o filho escreve o seu pid e termina. */

// Exercicio 1

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main () {
	
	if(fork() != 0) {
		int status;
		printf("pid pai: %d \n", getpid());
		waitpid(-1, &status, 0);
	}
	else {
		printf("pid filho: %d \n", getpid());
		exit(1);
	}
	return 0;
}


/* Execução do programa: gcc aula1ex1.c -o aula1ex1
./aula1ex1

   Saída: 
pid pai: 19106 
pid filho: 19107 

   Reflexão: Obtivemos esse resultado porque após o fork ter sido chamado, é criada uma cópia do programa pai para o filho. No processo pai o fork retorna o id do filho, portanto entrará no if e executará tudo até o waitpid(). Enquanto no processo do filho, o fork retornará 0 e entrará no else, executando tudo até o exit. Após o exit, o processo filho termina e notifica o pai, que volta a executar após o waitpid.
*/

/* 2) Agora, usando a mesma estrutura de processos pai e filho, declare uma variável visível ao pai e ao filho, no pai inicialize a variável com1 e imprima seu valor antes do fork(). No filho, altere o valor da variável para 5 e imprima o seu 
valor antes do exit(). Agora, no pai, imprima novamente o valor da variável após o filho ter alterado a variável - após a waitpid(). Justifique os resultados obtidos. */

// Exercicio 2

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main () {
	
	// Usar \n ou fflush() para limpar o buffer, pois estava imprimindo um valor a mais (1) no fim.
	
	int var = 1;
	printf("%d\n", var);
	
	if(fork() != 0) {
		int status;
		waitpid(-1, &status, 0);
		printf("%d \n" , var);
	}
	else {
		var = 5;
		printf("%d\n", var);
		exit(1);
	}
	return 0;
}


/* Execução do programa: gcc aula1ex2.c -o aula1ex2
./aula1ex2

   Saída:
1
5
1 

   Reflexão: Primeiro a variável var é impressa com o valor 1. Após o fork, o processo filho altera o valor de var para 5 e imprime, porém a var do filho ocupa outro espaço de memória, diferente do pai. Por isso, ao terminar o processo filho e voltar para o processo pai, após o waitpid, o pai imprime 1, pois o valor de var não foi alterado.
*/

/* 3) Use o programa anterior para ler e ordenar um vetor de 10 posições. O filho ordena o vetor e o pai exibe os dados do vetor antes do fork() e depois do waitpid(). Eles usarão o mesmo vetor na memória? Justifique. */

// Exercicio 3

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int compara(const void *num1, const void *num2) {
	if(*(int*)num1 < *(int*)num2) {
		return 0;
	}
	else {
		return 1;
	}
}

int main () {
	
	int var[] = {4,6,1,3,9,7,8,2,5,0};
	int i;
	printf("Antes do fork:\n");
	for(i=0;i<10;i++) {
		printf("%d \n", var[i]);
	}

	if(fork() != 0) {
		int status;
		waitpid(-1, &status, 0);
		printf("Depois do waitpid:\n");
		for(i=0;i<10;i++) {
			printf("%d \n", var[i]);
		}
	}
	else {
		qsort(var, 10, 4, compara);
		printf("Vetor do filho:\n");
		for(i=0;i<10;i++) {
			printf("%d \n", var[i]);
		}
		exit(1);
	}
	return 0;
}

/* Execução do programa: gcc aula1ex3.c -o aula1ex3
./aula1ex3

   Saída: 
Antes do fork:
4 
6 
1 
3 
9 
7 
8 
2 
5 
0 
Vetor do filho:
0 
1 
2 
3 
4 
5 
6 
7 
8 
9 
Depois do waitpid:
4 
6 
1 
3 
9 
7 
8 
2 
5 
0 


   Reflexão: Assim como no exercício anterior, verificamos que o vetor do filho é diferente do pai, ou seja, eles não usam o mesmo vetor na memória. O do filho foi ordenado, pois utilizamos a qsort, e ao terminar e voltar para o processo pai, após o waitpid, o vetor não estava ordenado.
*/

/* 4) Modifique o programa anterior para que o filho execute um programa elaborado por você, que mande imprimir uma mensagem qualquer no vídeo, por exemplo, “alo mundo”. Em seguida altere o programa do item 4 para o filho executar o programa echo da shell. */

// Exercicio 4.1

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main () {
	
	if(fork() != 0) {
		int status;
		waitpid(-1, &status, 0);
	}
	else {
		execv("aula1ex4.1comp", NULL);
		exit(1);
	}
	return 0;
}

// Exercicio 4.1 complementar

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main () {
	printf("Alo mundo \n");
	return 0;
}

/* Execução do programa: gcc aula1ex4.1comp.c -o aula1excomp
gcc aula1ex4.1.c -o aula1ex4.1
./aula1ex4.1

   Saída:
Alo mundo

   Reflexão: Percebemos que quando o comando execv é chamado, o processo é substituído pelo novo programa. Dessa forma, o comando não volta para o processo que estava sendo executado.
*/

// Exercicio 4.2

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main () {
	
	if(fork() != 0) {
		int status;
		waitpid(-1, &status, 0);
	}
	else {
		char *vet[] = {"echo", "star wars", 0};
		execv("/bin/echo", vet);
		exit(1);
	}
	return 0;
}

/* Execução do programa: gcc aula1ex4.2.c -o aula1ex4.2
./aula1ex4.2
   Saída:
star wars

   Reflexão: O processo filho chama a execv que substituirá o processo atual pelo programa echo da shell, que está localizado na pasta bin.
*/


