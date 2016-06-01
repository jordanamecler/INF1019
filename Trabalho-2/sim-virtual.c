#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

unsigned pegaIndicePagina(unsigned addr, int paginaTam) {

	int p = 13 ;

	if( paginaTam == 16 ) p = 14;
	else if (paginaTam == 32 ) p = 15;

	return addr >> p;
}


int main(int argc, char *argv[])
{
	// algoritmo pode ser: LRU, NRU, SEG
	// tamanhos de pagina suportados: 8 a 32 KB
	// tamanho total de memoria fisica
	// hipoteticamente disponivel entre: 128 kB e 16 MB

	FILE * arq;
	unsigned addr, page;
	char algoritmo[5], path[20];
	int paginaTam, memoriaFisicaTam, int rw;

	strcpy(algoritmo, argv[1]);
	strcpy(path, argv[2]);

	paginaTam =  atoi(argv[3]);
	memoriaFisicaTam = atoi(argv[4]) ;

	// printf("%s %s %d %d\n", algoritmo, path, paginaTam, memoriaFisicaTam);

	arq = fopen(path, "r");
	if( arq == NULL ) {
		printf("Erro ao abrir o arquivo %s\n", path);
		exit(1);
	}

	while( fscanf(arq, "%x %c", &addr, &rw) == 2 ) {

	}

	return 0;
}
