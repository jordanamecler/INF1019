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
	long int i;
	for( i = 0; i < 5; i++) {
		// roda por um tempo e printa 10x 
		//if ((i % 50000) == 0) {
			printf("Teste 4 rodando.\n");	
		//}
		sleep(1);
	}

	return 0;
}
