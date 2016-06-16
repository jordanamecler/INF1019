

//                                ,,                                                                     ,,
//    `7MMF'                    `7MM                                    `7MMM.     ,MMF'               `7MM
//      MM                        MM                                      MMMb    dPMM                   MM
//      MM  ,pW"Wq.`7Mb,od8  ,M""bMM   ,6"Yb.  `7MMpMMMb.   ,6"Yb.        M YM   ,M MM  .gP"Ya   ,p6"bo  MM  .gP"Ya `7Mb,od8
//      MM 6W'   `Wb MM' "',AP    MM  8)   MM    MM    MM  8)   MM        M  Mb  M' MM ,M'   Yb 6M'  OO  MM ,M'   Yb  MM' "'
//      MM 8M     M8 MM    8MI    MM   ,pm9MM    MM    MM   ,pm9MM        M  YM.P'  MM 8M"""""" 8M       MM 8M""""""  MM
// (O)  MM YA.   ,A9 MM    `Mb    MM  8M   MM    MM    MM  8M   MM        M  `YM'   MM YM.    , YM.    , MM YM.    ,  MM
//  Ymmm9   `Ybmd9'.JMML.   `Wbmd"MML.`Moo9^Yo..JMML  JMML.`Moo9^Yo.    .JML. `'  .JMML.`Mbmmd'  YMbmd'.JMML.`Mbmmd'.JMML.

// 1311354




//          .gP"Ya
//         ,M'   Yb
//         8M""""""
//         YM.    ,
//          `Mbmmd'



//                                                                  ,,                                           ,,
// `7MMF'                                                         `7MM               `7MMF'     A     `7MF'      db                               mm          `7MM
//   MM                                                             MM                 `MA     ,MA     ,V                                         MM            MM
//   MM         .gP"Ya   ,pW"Wq.`7MMpMMMb.   ,6"Yb.  `7Mb,od8  ,M""bMM  ,pW"Wq.         VM:   ,VVM:   ,V ,6"Yb.`7MM `7MMpMMMb.  ,pP"Ybd M"""MMV mmMMmm ,pW"Wq.  MM  ,MP'
//   MM        ,M'   Yb 6W'   `Wb MM    MM  8)   MM    MM' "',AP    MM 6W'   `Wb         MM.  M' MM.  M'8)   MM  MM   MM    MM  8I   `" '  AMV    MM  6W'   `Wb MM ;Y
//   MM      , 8M"""""" 8M     M8 MM    MM   ,pm9MM    MM    8MI    MM 8M     M8         `MM A'  `MM A'  ,pm9MM  MM   MM    MM  `YMMMa.   AMV     MM  8M     M8 MM;Mm
//   MM     ,M YM.    , YA.   ,A9 MM    MM  8M   MM    MM    `Mb    MM YA.   ,A9          :MM;    :MM;  8M   MM  MM   MM    MM  L.   I8  AMV  ,   MM  YA.   ,A9 MM `Mb.
// .JMMmmmmMMM  `Mbmmd'  `Ybmd9'.JMML  JMML.`Moo9^Yo..JMML.   `Wbmd"MML.`Ybmd9'            VF      VF   `Moo9^Yo.MM .JMML  JMML.M9mmmP' AMMmmmM   `Mbmo`Ybmd9'.JMML. YA.
//                                                                                                            QO MP
//                                                                                                            `bmP
// 1312737

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
		int menorIndiceNaoAcessado = -1;
	    int menorTempoNaoAcessado;
	    int menorIndiceVetorer = 0;
	    int menorTempoPag = vetorTabelaPaginas[vetorPaginas[0]].ultimoAcesso;

	    for(i = 0; i < vetorPaginasTam; ++i) {
	        if(vetorTabelaPaginas[vetorPaginas[i]].ultimoAcesso < menorTempoPag) {
	            menorIndiceVetor = i;
	            menorTempoPag = vetorTabelaPaginas[vetorPaginas[i]].ultimoAcesso;
	            if(vetorTabelaPaginas[vetorPaginas[i]].r == 0) {
	                menorIndiceNaoAcessado = i;
	                menorTempoNaoAcessado = menorTempoPag;
	            }
	        }
	    }

	    if(menorIndiceNaoAcessado != -1)
	        return menorIndiceNaoAcessado;
	    return menorIndiceVetor;
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
	int debug = 0;
	int passo = 0;

	int *vetorPaginas;
	int vetorPaginasTam;

	TabelaPagina * vetorTabelaPaginas;

	// Trata input

	strcpy(algoritmo, argv[1]);
	strcpy(path, argv[2]);

	paginaTam =  atoi(argv[3]);
	memoriaFisicaTam = atoi(argv[4]) ;

	if(argc > 5) {
		int i;
		for(i = 5; i < argc; i++){
			if(!strcmp(argv[i], "-d")) debug = 1;
			else if(!strcmp(argv[i], "-p")) passo = 1;
			else {
				printf("Opcao %s nao existe.\n", argv[i]);
				exit(1);
			}
		}
	}

	// Testa input

	if(!testaInputs(paginaTam, memoriaFisicaTam, algoritmo)) exit(1);

	printf("Executando o simulador...\n");

	// Cria vetor de paginas da memoria

	vetorPaginasTam = memoriaFisicaTam / paginaTam;
	

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

		// Modo detalhado

		if(debug || passo) {
			int i;
			printf("\nAlgoritmo: %s Time: %d\n", algoritmo, time);
			printf("Endereco: %x Modo: %c\n", addr, rw);
			printf("Indice: %d\n", indicePagina);

			if(passo) {
				printf("Memoria:\n");
				printf("  ____________\n" );
				for(i = 0; i < vetorPaginasTam; i++) {
					printf(" |  R:%d M:%d   |\n", vetorTabelaPaginas[vetorPaginas[i]].r, vetorTabelaPaginas[vetorPaginas[i]].m);
					printf(" |Ind:%-8d|\n", vetorPaginas[i]);
					printf(" |____________|\n");
				}	
			}
			
			printf("Falta de pagina: %d\n", faltasDePagina);
			printf("Paginas escritas: %d\n", paginasEscritas);
		}


		// Para o alg NRU, a cada interrupcao do reloogio, seta-se r = 0

		if( !(strcmp(algoritmo, "NRU")) && !(time % 100) ) {
			atualizaTabelaLRU(vetorTabelaPaginas, vetorPaginas, vetorPaginasTam);
		}

		// Testa de pagina esta na memoria

		if(paginaEstaNaMemoria == -1) {

			// Pagina nao esta na memoria
			// Checa se existe espaco vazio na memoria para a pagina

			faltasDePagina++;
			pos = buscaEspacoVazioVetor(vetorPaginas, vetorPaginasTam);

			// Existe espaco vazio na memoria para a pagina

			if(pos != -1) vetorPaginas[pos] = indicePagina;
			else {

				// Nao ha espaco vazio na memoria
				// Procurar pagina a ser removida para adicionar pagina nova de acordo com o algoritmo escolhido

				pos = escolhePaginaParaRemover(vetorTabelaPaginas, vetorPaginas, vetorPaginasTam, algoritmo);

				if(pos != -1) {

					if(debug || passo) printf("Posicao da pagina removida: %d\n", pos);

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
		else {
			if(debug || passo) printf("Pagina ja esta na memoria.\n");
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

		if(passo) {
			char prox;
			printf("Aperte qualquer tecla para proximo passo\n's': sair do passo-a-passo\n");
			scanf(" %c", &prox);
			if(prox == 's') passo = 0;
		}
	}

	printf("Arquivo de entrada: %s\n", path);
	printf("Tamanho da memoria fisica: %d KB\n", memoriaFisicaTam);
	printf("Tamanho das paginas: %d KB\n", paginaTam);
	printf("Alg de substituicao: %s\n", algoritmo);
	if(debug || passo) printf("Tamanho vetor de paginas: %d\n",vetorPaginasTam );
	printf("Numero de faltas de paginas: %d\n", faltasDePagina);
	printf("Numero de paginas escritas: %d\n", paginasEscritas);
	if(debug || passo) printf("time: %d\n", time );

	// Libera memoria alocada

	free(vetorTabelaPaginas);
	free(vetorPaginas);
	fclose(arq);

	return 0;
}
