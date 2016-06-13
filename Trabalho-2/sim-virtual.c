#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

typedef struct tabelaPagina {

	int r;
	int m;
	int ultimoAcesso;	
} TabelaPagina;

unsigned pegaIndicePagina(unsigned addr, int paginaTam) {

	return addr >> log2(paginaTam << 10);;
}

unsigned pegaTamanhoTabelaPaginas(int paginaTam) {

	return pow(2, 32 - log2(paginaTam << 10));
}

int * criaVetorPagina(int vetorPaginasTam) {

	int i;
	int *vetorPaginas = (int *)malloc(sizeof(int) * vetorPaginasTam);

	if(!vetorPaginas) {
		printf("Erro na alocacao do vetor de paginas.\n");
		exit(1);
	}
	for(i = 0; i < vetorPaginasTam; i++) {
		vetorPaginas[i] = -1;
	}

	return vetorPaginas;
}

TabelaPagina * criaVetorTabelaPaginas(int paginaTam) {

	int tabelaPaginaTam = pegaTamanhoTabelaPaginas(paginaTam);
	TabelaPagina *vetorTabelaPaginas;

	printf("Tamanha tabela de paginas: %d\n", tabelaPaginaTam);

	vetorTabelaPaginas = (TabelaPagina *)malloc(sizeof(TabelaPagina) * tabelaPaginaTam);
	if(!vetorTabelaPaginas) {
		printf("Erro na alocacao do vetor de tabela de paginas\n");
		exit(1);
	}

	memset(vetorTabelaPaginas, 0, tabelaPaginaTam * sizeof(TabelaPagina));
	return vetorTabelaPaginas;

}

int buscaPaginaNaMemoria(int *vetorPaginas, int vetorPaginasTam, int indicePagina) {
	int i;
	for(i = 0; i < vetorPaginasTam; i++) {
		if(vetorPaginas[i] == indicePagina) {
			return 1;
		}
	}
	return -1;
}


int main(int argc, char *argv[])
{
	// algoritmo pode ser: LRU, NRU, SEG
	// tamanhos de pagina suportados: 8 a 32 KB
	// tamanho total de memoria fisica
	// hipoteticamente disponivel entre: 128 kB e 16 MB

	FILE *arq;
	unsigned addr, page;
	char algoritmo[5], path[20];
	int paginaTam, memoriaFisicaTam, rw, time = 0;
	int faltasDePagina = 0;
	int paginasEscritas = 0;

	int *vetorPaginas;
	int vetorPaginasTam;

	TabelaPagina * vetorTabelaPaginas;

	// Trata input

	strcpy(algoritmo, argv[1]);
	strcpy(path, argv[2]);

	paginaTam =  atoi(argv[3]);
	memoriaFisicaTam = atoi(argv[4]) ;

	printf("Executando o simulador...\n");

	printf("Arquivo de entrada: %s\n", path);
	printf("Tamanho da memoria fisica: %d\n", memoriaFisicaTam);
	printf("Tamanho das paginas: %d\n", paginaTam);
	printf("Alg de substituicao: %s\n", algoritmo);

	// Testa input

	if(paginaTam < 8 || paginaTam > 32) {
		printf("Tamanho de pagina invalido. Deve ser entre 8 e 32KB.\n");
		exit(1);
	}
	if(memoriaFisicaTam < 128 || memoriaFisicaTam > (16 * 1024)) {
		printf("Tamanho de memoria fisica invalido. Deve ser entre 128KB e 16MB.\n");
		exit(1);
	}

	// Cria vetor de paginas da memoria

	vetorPaginasTam = memoriaFisicaTam / paginaTam;
	printf("Tamanho vetor de paginas: %d\n",vetorPaginasTam );

	vetorPaginas = criaVetorPagina(vetorPaginasTam);

	// Cria vetor de tabela de paginas

	vetorTabelaPaginas = criaVetorTabelaPaginas(paginaTam);

	// Le o log

	arq = fopen(path, "r");
	if( arq == NULL ) {
		printf("Erro ao abrir o arquivo %s\n", path);
		exit(1);
	}

	while( fscanf(arq, "%x %c", &addr, &rw) == 2 ) {

		int indicePagina = pegaIndicePagina(addr, paginaTam);
		int paginaEstaNaMemoria = buscaPaginaNaMemoria(vetorPaginas, vetorPaginasTam, indicePagina);

		// Pagina nao esta na memoria
		if(!paginaEstaNaMemoria) {

		}


		time++;
	}

	// Libera memoria alocada

	free(vetorTabelaPaginas);
	free(vetorPaginas);
	fclose(arq);

	return 0;
}
