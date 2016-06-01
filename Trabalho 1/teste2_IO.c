#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

// compilar esse programa antes de rodar main.c

int main()
{
	// IObound

	int i, j;
	FILE *entrada, *saida;
	char string[200];

	entrada = fopen("entrada.txt","r");
	saida = fopen("saida.txt", "a");	

	
		
	while (fscanf(entrada," %s", string) == 1) {
		fprintf(saida, " Teste2 printed:  %s\n", string);
	}
	
	fclose(entrada);
	fclose(saida);
	
	return 0;
}