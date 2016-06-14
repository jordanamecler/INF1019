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

int testaInputs(int paginaTam, int memoriaFisicaTam, char *algoritmo) {
	int ret = 1;

	if(paginaTam < 8 || paginaTam > 32) {
		printf("Tamanho de pagina invalido. Deve ser entre 8 e 32KB.\n");
		ret = 0;
	}
	if(memoriaFisicaTam < 128 || memoriaFisicaTam > (16 * 1024)) {
		printf("Tamanho de memoria fisica invalido. Deve ser entre 128KB e 16MB.\n");
		ret = 0;
	}
	if(strcmp(algoritmo, "LRU") && strcmp(algoritmo, "NRU") && strcmp(algoritmo, "SEG")) {
		printf("Algoritmo nao reconhecido.\n");
		ret = 0;
	}
	return ret;
}

unsigned pegaIndicePagina(unsigned addr, int paginaTam) {

	return addr >> (int)log2(paginaTam << 10);
}

unsigned pegaTamanhoTabelaPaginas(int paginaTam) {

	return pow(2, 32 - (int)log2(paginaTam << 10));
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

	printf("Tamanho tabela de paginas: %d\n", tabelaPaginaTam);

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

int buscaEspacoVazioVetor(int *vetor, int vetorTam) {
	int i;
	for(i = 0; i < vetorTam; i++) {
		if(vetor[i] == -1) return i;
	}
	return -1;
}

void atualizaTabelaLRU(TabelaPagina * vetorTabelaPaginas, int *vetorPaginas, int vetorPaginasTam) {
	int i;
	int indice;
	
	for(i = 0; i < vetorPaginasTam; i++) {
		indice = vetorPaginas[i];
		if(indice != -1) {
			vetorTabelaPaginas[indice].r = 0;
		}
	}
}

int escolhePaginaParaRemover(TabelaPagina *vetorTabelaPaginas, int *vetorPaginas, int vetorPaginasTam, char *algoritmo) {

	if(algoritmo[0] == 'L'){

		// LRU

		int i;
		int minNotAccessedPageVectorIndex = -1;
	    int minNotAccessedPageTime;
	    int minPageVectorIndex = 0;
	    int minPageTime = vetorTabelaPaginas[vetorPaginas[0]].ultimoAcesso;

	    for(i = 0; i < vetorPaginasTam; ++i) {
	        if(vetorTabelaPaginas[vetorPaginas[i]].ultimoAcesso < minPageTime) {
	            minPageVectorIndex = i;
	            minPageTime = vetorTabelaPaginas[vetorPaginas[i]].ultimoAcesso;
	            if(vetorTabelaPaginas[vetorPaginas[i]].r == 0) {
	                minNotAccessedPageVectorIndex = i;
	                minNotAccessedPageTime = minPageTime;
	            }
	        }
	    }

	    if(minNotAccessedPageVectorIndex != -1)
	        return minNotAccessedPageVectorIndex;
	    return minPageVectorIndex;
	}
	else if(algoritmo[0] == 'N') {

		// NRU

		int i, teste;
		int prioridadeR[] = {0,0,1,1};
		int prioridadeM[] = {0,1,0,1};

		for(teste = 0; teste < 4; teste++) {
			for(i = 0; i < vetorPaginasTam; i++) {

				TabelaPagina *pag = &vetorTabelaPaginas[vetorPaginas[i]];
				if(pag->r == prioridadeR[teste] && pag->m == prioridadeM[teste]) {
					return i;
				}
			}
		}
	}
	else if(algoritmo[0] == 'S') {

		// SEG

		do {
			int indice = vetorPaginas[0];
			int i;

			for(i = 0; i < vetorPaginasTam - 1; i++) {
				vetorPaginas[i] = vetorPaginas[i + 1];
			}
			vetorPaginas[vetorPaginasTam - 1] = indice;

			if(vetorTabelaPaginas[indice].r == 0) return vetorPaginasTam - 1;
			else vetorTabelaPaginas[indice].r = 0;

		} while(1);
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
	char algoritmo[5], path[20], fullpath[30], rw;
	int paginaTam, memoriaFisicaTam, time = 0;
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
	printf("Tamanho da memoria fisica: %d KB\n", memoriaFisicaTam);
	printf("Tamanho das paginas: %d KB\n", paginaTam);
	printf("Alg de substituicao: %s\n", algoritmo);

	// Testa input

	if(!testaInputs(paginaTam, memoriaFisicaTam, algoritmo)) exit(1);

	// Cria vetor de paginas da memoria

	vetorPaginasTam = memoriaFisicaTam / paginaTam;
	printf("Tamanho vetor de paginas: %d\n",vetorPaginasTam );

	vetorPaginas = criaVetorPagina(vetorPaginasTam);

	// Cria vetor de tabela de paginas

	vetorTabelaPaginas = criaVetorTabelaPaginas(paginaTam);

	// Le o log

	strcpy(fullpath, "logs/");
	strcat(fullpath, path);

	arq = fopen(fullpath, "r");
	if( arq == NULL ) {
		printf("Erro ao abrir o arquivo %s\n", path);
		exit(1);
	}

	while( fscanf(arq, "%x %c", &addr, &rw) == 2 ) {

		int pos;
		int indicePagina = pegaIndicePagina(addr, paginaTam);
		int paginaEstaNaMemoria = buscaPaginaNaMemoria(vetorPaginas, vetorPaginasTam, indicePagina);

		// Para o alg NRU, a cada interrupcao do reloogio, seta-se r = 0

		if( !(strcmp(algoritmo, "NRU")) && !(time % 100) ) {
			atualizaTabelaLRU(vetorTabelaPaginas, vetorPaginas, vetorPaginasTam);
		}

		// Testa de pagina esta na memoria

		if(paginaEstaNaMemoria == -1) {

			// Pagina nao esta na memoria
			// Checa se existe espaco vazio na memoria para a pagina

			pos = buscaEspacoVazioVetor(vetorPaginas, vetorPaginasTam);

			// Existe espaco vazio na memoria para a pagina

			if(pos != -1) vetorPaginas[pos] = indicePagina;
			else {

				// Nao ha espaco vazio na memoria
				// Procurar pagina a ser removida para adicionar pagina nova de acordo com o algoritmo escolhido

				faltasDePagina++;

				pos = escolhePaginaParaRemover(vetorTabelaPaginas, vetorPaginas, vetorPaginasTam, algoritmo);

				if(pos != -1) {
					TabelaPagina *paginaRetirada = &vetorTabelaPaginas[vetorPaginas[pos]];
					if(paginaRetirada->m == 1) paginasEscritas++;

					paginaRetirada->r = 0;
					paginaRetirada->m = 0;

					vetorPaginas[pos] = indicePagina;	
				}
				else {
					printf("Nao achou pagina para retirar.\n");
				}
			}
		}

		// Pagina deve ser atualizada

		TabelaPagina *pag = &vetorTabelaPaginas[indicePagina];
		pag->ultimoAcesso = time;

		if(rw == 'R') {
			pag->r = 1;
		}
		else if(rw == 'W') {
			pag->m = 1;
		}

		time++;
	}

	printf("Numero de faltas de paginas: %d\n", faltasDePagina);
	printf("Numero de paginas escritas: %d\n", paginasEscritas);
	printf("time: %d\n", time );

	// Libera memoria alocada

	free(vetorTabelaPaginas);
	free(vetorPaginas);
	fclose(arq);

	return 0;
}
