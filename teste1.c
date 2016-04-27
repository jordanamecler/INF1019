#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

// compilar esse programa antes de rodar main.c

int main()
{
	// CPU-BOUND
	int i, j;
	for( i = 0; i < 500000; i++) {
		// roda por um tempo e printa 10x 
		for (j = 0; j < 5000; j++) {
			if ((j % 1000 == 0) && (i % 100000 == 0)) {
				printf("Teste 1 rodando.\n");	
			}
		}
	}

	return 0;
}