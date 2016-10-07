/* 1) Soma de matrizes 
Faça um programa para somar matrizes de acordo com o seguinte algoritmo:
O primeiro processo irá criar duas matrizes preenchidas e uma terceira vazia em 3 áreas de memória compartilhada. 
Para cada linha da matriz solução, o seu programa deverá gerar um processo para o seu cálculo.  */
// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 1

#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main () {

	int segmento1, segmento2, segmento3, segmento1_2, segmento2_2, segmento3_2, id, pid, status;
	int **matriz1, **matriz2, **matriz3;
	int i, j;
	
	
	
	segmento1 = shmget (IPC_PRIVATE, sizeof(int) * 2, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	matriz1 = (int **) shmat (segmento1, 0, 0);
	
	for(i=0;i<2;i++) {
		segmento1_2 = shmget (IPC_PRIVATE, sizeof(int) * 2, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
		matriz1[i] = (int *) shmat (segmento1_2, 0, 0);
	}
	
	segmento2 = shmget (IPC_PRIVATE, sizeof(int) * 2, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	matriz2 = (int **) shmat (segmento2, 0, 0);
	
	for(i=0;i<2;i++) {
		segmento2_2 = shmget (IPC_PRIVATE, sizeof(int) * 2, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
		matriz2[i] = (int *) shmat (segmento2_2, 0, 0);
	}
	
	segmento3 = shmget (IPC_PRIVATE, sizeof(int) * 2, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	matriz3 = (int **) shmat (segmento3, 0, 0);
	
	for(i=0;i<2;i++) {
		segmento3_2 = shmget (IPC_PRIVATE, sizeof(int) * 2, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
		matriz3[i] = (int *) shmat (segmento3_2, 0, 0);
	}
	
	
	for(i=0;i<2;i++) {
		for(j=0; j<2; j++) {
			matriz1[i][j] = 1;
		}
	}

	for(i=0;i<2;i++) {
		for(j=0; j<2; j++) {
			matriz2[i][j] = 2;
		}
	}
	
	for(i=0;i<2;i++) {
		
		
		if ((id = fork()) < 0) {
			puts("Erro na criação do novo processo");
			exit(-2);
		}
		else if (id==0) {
			for(j=0;j<2;j++) {
				matriz3[i][j] = matriz1[i][j] + matriz2[i][j];
			}
			shmdt(matriz1);
			shmdt(matriz2);
			shmdt(matriz3);
			exit(0);
		}
		else {
			int status;
			wait(&status);
		}
	}
	
	for(i=0;i<2;i++) {
		for(j=0;j<2;j++) {
			printf("%d ", matriz3[i][j]);
		}
		printf("\n");
	}
	
	for(i=0;i<2;i++) {
		shmdt(matriz1[i]);
		shmdt(matriz2[i]);
		shmdt(matriz3[i]);
		shmctl(segmento1_2, IPC_RMID, 0);
		shmctl(segmento2_2, IPC_RMID, 0);
		shmctl(segmento3_2, IPC_RMID, 0);
	}
	
	shmdt(matriz1);
	shmdt(matriz2);
	shmdt(matriz3);
	shmctl(segmento1, IPC_RMID, 0);
	shmctl(segmento2, IPC_RMID, 0);
	shmctl(segmento2, IPC_RMID, 0);
	return 0;
}

/* Execução do programa: gcc aula2ex1.c -o aula2ex1
./aula2ex1
   
   Saída: 
3 3 
3 3 

   Reflexão: Percebemos que a matriz criada na memoria compartilhada realmente foi acessada nos dois processos filhos. Criamos um vetor na memoria compartilhada que representaria a primeira coluna da matriz, e em seguida criamos vetores na memoria compartilhada que seriam as linhas de cada coluna. Fizemos essa solucao pois nao estavamos conseguindo tratar o espaco alocado como uma matriz.
*/

/* 2) Mensagem do Dia 
Faça um programa que: 
Leia uma mensagem do dia 
Crie uma memória compartilhada com a chave 8752 
Salve a mensagem na memória 
Faça um outro programa que utilize a mesma chave (8752) e exiba a mensagem do dia para o usuário   */
// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 2

#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {

	int segmento, id, pid, status;
	char *msgDoDiaCopy;
	char *msgDoDia = "Fora corruptos";
	
	segmento = shmget (8752, sizeof(char) * 17, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	if(segmento == -1) {
		printf("erro");
		exit(1);
	}
	msgDoDiaCopy = (char *) shmat (segmento, 0, 0);
	
	strcpy(msgDoDiaCopy, msgDoDia);
	
	
	printf("pai: %s \n", msgDoDiaCopy);
	shmdt(msgDoDiaCopy);
	return 0;
}

/* Execução do programa: gcc aula2ex2.c -o aula2ex2
./aula2ex2
   Saída: 
pai: Fora corruptos

gcc aula2ex2comp.c -o aula2ex2comp
./aula2ex2comp
novo processo: Fora corruptos

   Reflexão: Ao inves de usar o IPC_PRIVATE, utilizamos uma chave especifica (8752). Tivemos um problema pois da primeira vez que rodamos, o arquivo aula2ex2comp deu erro e a memoria compartilhada nao foi liberada. Portanto, da segunda vez que rodamos o programa dava erro ao tentar alocar a memoria. Resolvemos isso utilizando o comando ipcrm -M (endereco de memoria em hexadecimal). No aula2ex2comp tiramos as flags IPC_CREAT e IPC_EXCL pois a memoria ja tinha sido alocada.
*/

// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 2 complementar

#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {

	char *msgDoDia;
	int segmento;
	
	segmento = shmget (8752, sizeof(char) * 17, S_IRUSR | S_IWUSR);
	if(segmento == -1) {
		printf("erro");
		exit(1);
	}
	msgDoDia = (char *) shmat (segmento, 0, 0);
	
	printf("novo processo: %s \n", msgDoDia);
	
	shmdt(msgDoDia);
	shmctl(segmento, IPC_RMID, 0);
	return 0;
}

/* 3) Busca paralela em vetor 
Faça um programa paralelo para localizar uma chave em um vetor. 
Crie uma memória compartilhada com dados numéricos inteiros e desordenados e a divida pelo número de processos 
Cada processo deve procurar o dado na sua área de memória e informar a posição onde o dado foi localizado.  */
// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 3

#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int buscaLinear(int *vet, int tam, int chave) {
	int i;
	for(i=0;i<tam;i++) {
		if(vet[i] == chave) {
			return i;
		}
	}
	return -1;
}

int main () {

	int *vet;
	int chave = 5;
	int segmento, id, i, posicao;
	
	segmento = shmget (IPC_PRIVATE, sizeof(int) * 9, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	if(segmento == -1) {
		printf("erro");
		exit(1);
	}
	vet = (int *) shmat (segmento, 0, 0);
	vet[0] = 4;
	vet[1] = 0;
	vet[2] = 2;
	vet[3] = 7;
	vet[4] = 6;
	vet[5] = 3;
	vet[6] = 1;
	vet[7] = 5;
	vet[8] = 8;

	for(i=0;i<3;i++) {
		if ((id = fork()) < 0) {
			puts("Erro na criação do novo processo");
			exit(-2);
		}
		else if (id==0) {
			
			posicao = buscaLinear(vet + i*3, 3, chave);
			if(posicao != -1) {
				printf("Processo %d achou na posição %d \n", i+1, posicao + i*3);
			}
			else {
				printf("nao achou \n");
			}
			shmdt(vet);
			exit(0);
		}
		else {
			int status;
			wait(&status);
		}
	}
	
	shmdt(vet);
	shmctl(segmento, IPC_RMID, 0);
	return 0;
}

/* Execução do programa: gcc aula2ex3.c -o aula2ex3
./aula2ex3
   Saída: 
nao achou 
nao achou 
Processo 3 achou na posição 7 

   Reflexão: Criamos um vetor de 9 posicoes, preenchemos ele com numeros de 0 a 8 desordenadamente. Escolhemos uma chave, de valor 5, e dividimos o vetor em 3 partes iguais para cada processo procurar a chave em apenas uma parte, com busca linear. O processo que achar, imprime o seu numero (1 a 3) e a posicao da chave no vetor (0 a 8). O vetor foi criado em memoria compartilhada para que os tres processos possam ter controle sobre o mesmo espaco de memoria.
*/
