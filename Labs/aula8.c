/*

	Jordana Mecler e Leonardo E. Wajnsztok

	Exercicio 1

*/

#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pthread.h>

void *contaCrescente() {
	int i;
	for (i = 0; i<20; i++) {
		printf("crescente %d\n", i);
		sleep(1);
	}
	pthread_exit(NULL);
}

void *contaDecrescente() {
	int i;
	for (i = 30; i>0; i--) {
		printf("decrescente %d\n", i);
		sleep(2);	
	} 
	pthread_exit(NULL);
}


int main() {


	pthread_t thCrescente, thDecrescente;

	pthread_create(&thCrescente, NULL, contaCrescente, NULL);
	pthread_create(&thDecrescente, NULL, contaDecrescente, NULL);

	pthread_join(thCrescente, NULL);
	pthread_join(thDecrescente, NULL);

	return 0;
}

/*

Resultado:

decrescente 30
crescente 0
crescente 1
decrescente 29
crescente 2
crescente 3
decrescente 28
crescente 4
crescente 5
decrescente 27
crescente 6
crescente 7
decrescente 26
crescente 8
crescente 9
decrescente 25
crescente 10
crescente 11
decrescente 24
crescente 12
crescente 13
decrescente 23
crescente 14
crescente 15
decrescente 22
crescente 16
crescente 17
decrescente 21
crescente 18
crescente 19
decrescente 20
decrescente 19
decrescente 18
decrescente 17
decrescente 16
decrescente 15
decrescente 14
decrescente 13
decrescente 12
decrescente 11
decrescente 10
decrescente 9
decrescente 8
decrescente 7
decrescente 6
decrescente 5
decrescente 4
decrescente 3
decrescente 2
decrescente 1


Explicação:

Criamos duas threads, uma que incrementa e outra decrementa suas variaves locais. As threads sao criadas e como as variaves sao locais, os valores ficam ordenados crescente e decrescente.
*/

/*

	Jordana Mecler e Leonardo E. Wajnsztok

	Exercicio 2

*/

#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pthread.h>

int num = 0;

void *contaCrescente() {
	int i;
	for (i = 0; i<20; i++) {
		num ++;
		printf("crescente %d\n", num);
		sleep(1);
	}
	pthread_exit(NULL);
}

void *contaDecrescente() {
	int i;
	for (i = 30; i>0; i--) {
		num--;
		printf("decrescente %d\n", num);
		sleep(2);	
	} 
	pthread_exit(NULL);
}


int main() {


	pthread_t thCrescente, thDecrescente;

	pthread_create(&thCrescente, NULL, contaCrescente, NULL);
	pthread_create(&thDecrescente, NULL, contaDecrescente, NULL);

	pthread_join(thCrescente, NULL);
	pthread_join(thDecrescente, NULL);

	return 0;
}

/*

Resultado:

crescente 1
decrescente 0
crescente 1
decrescente 0
crescente 1
crescente 2
decrescente 1
crescente 2
crescente 3
decrescente 2
crescente 3
crescente 4
decrescente 3
crescente 4
crescente 5
decrescente 4
crescente 5
crescente 6
decrescente 5
crescente 6
crescente 7
decrescente 6
crescente 7
crescente 8
decrescente 7
crescente 8
crescente 9
decrescente 8
crescente 9
crescente 10
decrescente 9
decrescente 8
decrescente 7
decrescente 6
decrescente 5
decrescente 4
decrescente 3
decrescente 2
decrescente 1
decrescente 0
decrescente -1
decrescente -2
decrescente -3
decrescente -4
decrescente -5
decrescente -6
decrescente -7
decrescente -8
decrescente -9
decrescente -10


Explicação:

Criamos duas threads, uma que incrementa e outra decrementa a mesma variavel global. As threads sao criadas e atuam sem se preocupar com o que outra faz e terminam. Como a variavel é global, a variavel é toda  hora incrementada e decrementada.


*/

/*

	Jordana Mecler e Leonardo E. Wajnsztok

	Exercicio 3

*/

#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#define MAX 8

typedef struct fila
{
	int info[MAX];
	int primeiro;
	int numElementos;
} Fila ;



Fila *fila_cria (void)
{
	Fila *fila = (Fila *) malloc (sizeof (Fila));
	if (fila == NULL)
	{
		printf ("Erro ao alocar espaco para a fila\n");
		return NULL;
	}

	fila->primeiro = fila->numElementos = 0;

	return fila;
}

//
// GLOBAIS
//

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

Fila *f; 


void fila_destroi (Fila *fila)
{
	free (fila);
}

int fila_vazia (Fila *fila)
{
	return (fila->numElementos == 0);
}

int fila_insere (Fila *fila, int numero)
{
	int final;

	if (fila->numElementos == MAX)
	{
		printf("Fila cheia, nao foi possivel inserir o elemento %lf\n", numero);
		return 0;
	}

	final = (fila->primeiro + fila->numElementos) % MAX;
	fila->info[final] = numero;
	(fila->numElementos)++;

	return 1;
}

int fila_remove (Fila *fila, int *numero)
{
	if (fila_vazia (fila))
	{
		printf ("Fila vazia, nao foi possivel remover um elemento %lf\n", *numero);
		return 0;
	}

	*numero = fila->info[fila->primeiro];
	fila->primeiro = (fila->primeiro + 1) % MAX;
	(fila->numElementos)--;

	return 1;
}

void fila_exibe (Fila *fila)
{
	int p = fila->primeiro, n = fila->numElementos, contador = 1;
	if (fila_vazia (fila))
	{
		printf ("Fila vazia, nao foi possivel imprimir\n");
		return;
	}

	printf ("Fila:\n");
	do
	{
		if (contador == n)
			break;

		printf ("%lf \n", fila->info[p]);

		p = (p + 1) % MAX;
		contador++;
	} while (contador != n);

	printf ("%lf \n", fila->info[p]);
}

void *produtor() {

	int i = 0, num;
	srand(time(NULL));
	while (i < 64)
	{
		sleep(1);

		if (f->numElementos < 8 ) {
			pthread_mutex_lock(&mutex);
			i++;
			num = (rand() % 64) + 1;
			fila_insere(f, num);
			printf("Produtor inseriu %d\n", num);
			pthread_mutex_unlock(&mutex);	
		}
		else {
			printf("Fila cheia, produtor esperando\n");
		}
		
	}

	pthread_exit(NULL);
}

void *consumidor() {

	int cont = 0, num;

	while(cont < 64) {

		sleep(2);	

		if (!fila_vazia(f)) {
			pthread_mutex_lock(&mutex);
			cont++;
			fila_remove(f, &num);
			printf("Consumidor removeu %d\n", num);
			pthread_mutex_unlock(&mutex);
		}
		else {
			printf("Fila vazia, consumidor nao consome\n");
		}
	}
	

	pthread_exit(NULL);
}


int main() {

	f = fila_cria();

	pthread_t thConsumidor, thProdutor;

	pthread_create(&thProdutor, NULL, produtor, NULL);
	pthread_create(&thConsumidor, NULL, consumidor, NULL);

	pthread_join(thProdutor, NULL);
	pthread_join(thConsumidor, NULL);

	return 0;
}

/*

Resultado:

	Produtor inseriu 22
Consumidor removeu 22
Produtor inseriu 27
Produtor inseriu 54
Consumidor removeu 27
Produtor inseriu 37
Produtor inseriu 31
Consumidor removeu 54
Produtor inseriu 38
Produtor inseriu 13
Consumidor removeu 37
Produtor inseriu 46
Produtor inseriu 29
Consumidor removeu 31
Produtor inseriu 53
Produtor inseriu 36
Consumidor removeu 38
Produtor inseriu 62
Produtor inseriu 26
Consumidor removeu 13
Produtor inseriu 30
Produtor inseriu 1
Consumidor removeu 46
Produtor inseriu 63
Fila cheia, produtor esperando
Consumidor removeu 29
Produtor inseriu 59
Fila cheia, produtor esperando
Consumidor removeu 53
Produtor inseriu 30
Fila cheia, produtor esperando
Consumidor removeu 36
Produtor inseriu 13
Fila cheia, produtor esperando
Consumidor removeu 62
Produtor inseriu 62
Fila cheia, produtor esperando
Consumidor removeu 26
Produtor inseriu 23
Fila cheia, produtor esperando
Consumidor removeu 30
Produtor inseriu 55
Fila cheia, produtor esperando
Consumidor removeu 1
Produtor inseriu 46
Fila cheia, produtor esperando
Consumidor removeu 63
Produtor inseriu 23
Fila cheia, produtor esperando
Consumidor removeu 59
Produtor inseriu 45
Fila cheia, produtor esperando
Consumidor removeu 30
Produtor inseriu 6
Fila cheia, produtor esperando
Consumidor removeu 13
Produtor inseriu 30
Fila cheia, produtor esperando
Consumidor removeu 62
Produtor inseriu 28
Fila cheia, produtor esperando
Consumidor removeu 23
Produtor inseriu 6
Fila cheia, produtor esperando
Consumidor removeu 55
Produtor inseriu 15
Fila cheia, produtor esperando
Consumidor removeu 46
Produtor inseriu 37
Fila cheia, produtor esperando
Consumidor removeu 23
Produtor inseriu 27
Fila cheia, produtor esperando
Consumidor removeu 45
Produtor inseriu 41
Fila cheia, produtor esperando
Consumidor removeu 6
Produtor inseriu 27
Fila cheia, produtor esperando
Consumidor removeu 30
Produtor inseriu 63
Fila cheia, produtor esperando
Consumidor removeu 28
Produtor inseriu 8
Fila cheia, produtor esperando
Consumidor removeu 6
Produtor inseriu 64
Fila cheia, produtor esperando
Consumidor removeu 15
Produtor inseriu 11
Fila cheia, produtor esperando
Consumidor removeu 37
Produtor inseriu 53
Fila cheia, produtor esperando
Consumidor removeu 27
Produtor inseriu 28
Fila cheia, produtor esperando
Consumidor removeu 41
Produtor inseriu 63
Fila cheia, produtor esperando
Consumidor removeu 27
Produtor inseriu 24
Fila cheia, produtor esperando
Consumidor removeu 63
Produtor inseriu 25
Fila cheia, produtor esperando
Consumidor removeu 8
Produtor inseriu 25
Fila cheia, produtor esperando
Consumidor removeu 64
Produtor inseriu 53
Fila cheia, produtor esperando
Consumidor removeu 11
Produtor inseriu 25
Fila cheia, produtor esperando
Consumidor removeu 53
Produtor inseriu 23
Fila cheia, produtor esperando
Consumidor removeu 28
Produtor inseriu 47
Fila cheia, produtor esperando
Consumidor removeu 63
Produtor inseriu 54
Fila cheia, produtor esperando
Consumidor removeu 24
Produtor inseriu 35
Fila cheia, produtor esperando
Consumidor removeu 25
Produtor inseriu 44
Fila cheia, produtor esperando
Consumidor removeu 25
Produtor inseriu 13
Fila cheia, produtor esperando
Consumidor removeu 53
Produtor inseriu 25
Fila cheia, produtor esperando
Consumidor removeu 25
Produtor inseriu 25
Fila cheia, produtor esperando
Consumidor removeu 23
Produtor inseriu 35
Fila cheia, produtor esperando
Consumidor removeu 47
Produtor inseriu 5
Fila cheia, produtor esperando
Consumidor removeu 54
Produtor inseriu 31
Fila cheia, produtor esperando
Consumidor removeu 35
Produtor inseriu 64
Fila cheia, produtor esperando
Consumidor removeu 44
Produtor inseriu 32
Fila cheia, produtor esperando
Consumidor removeu 13
Produtor inseriu 36
Fila cheia, produtor esperando
Consumidor removeu 25
Produtor inseriu 14
Fila cheia, produtor esperando
Consumidor removeu 25
Produtor inseriu 5
Fila cheia, produtor esperando
Consumidor removeu 35
Produtor inseriu 62
Fila cheia, produtor esperando
Consumidor removeu 5
Produtor inseriu 55
Consumidor removeu 31
Consumidor removeu 64
Consumidor removeu 32
Consumidor removeu 36
Consumidor removeu 14
Consumidor removeu 5
Consumidor removeu 62
Consumidor removeu 55


Explicação: Criamos uma thread para o produtor e uma para o consumidor. Fizemos as implementações de fila como buffer circular para fazer as insercoes e remocoes da fila. Na thread do produtor, quando a fila ainda nao estiver cheia, inserimos de um em um segundo. Na thread do consumidor, quando a fila nao esta vazia, removemos de dois em dois segundos. Usamos mutex para estabelecer uma regiao critica em que se uma thread estiver, a outra nao pode entrar.


*/
