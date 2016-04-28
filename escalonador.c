#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>
#include <time.h>
#include "lista.h"

#define TIME_SHARE 1
#define MAX_BILHETES 20

#define CHAVE_INFO_FLAG 8662
#define CHAVE_TIPO 8663
#define CHAVE_NUM_TICKETS 8664
#define CHAVE_PRIORIDADE 8665
#define CHAVE_PATH 8666
#define CHAVE_END 8667

No *listaPrioridade = NULL;
No *listaRoundRobin = NULL;
No *listaLoteria = NULL;

int bilheteSorteado = -1;


void rodaProcessoPrioridade(No *processoAnterior);
// pid_t retiraPID (int tipo);
void realocaProcessoRoundRobin ();
void liberaEscalonador();
void childHandler(int sinal);
void alarmHandler(int sinal);
void rodaProcessoRoundRobin ();

void insereProcesso(char *path, int tipo, int prioridade, int numBilhetes) {
	
	pid_t pid = 0;

	// Cria processo filho que será substituido por programa em 'path'
	if((pid = fork()) < 0) {
		printf("Erro ao criar processo filho.\n");
		exit(1);
	}
	else if(pid == 0) {
		if(execv(path, NULL) < 0) {
			printf("Erro ao executar o programa %s.\n", path);
		}
	}
	else if(pid > 0) {
		int status;

		kill(pid, SIGSTOP);
		printf("Escalonador criou processo filho %s.\n", path);


		if (tipo == 0) {
			listaRoundRobin = insereNo(listaRoundRobin, pid, path, tipo, prioridade, numBilhetes);
		}
		else if (tipo == 1) {

			// Procura no com prioridade menor (ex:  prioridade 3 < prioridade 2) e insere antes dele
			printf("tipo Prioridade, inserindo na listaPrioridade...\n");
			No * p = listaPrioridade;
			No * ant = NULL;
			No * novo;

			if( p == NULL ) {
				listaPrioridade = insereNo(listaPrioridade, pid, path, tipo, prioridade, numBilhetes);
			}
			else if (p != NULL) {
				while(p != NULL && p->prioridade <= prioridade ){
					ant = p;
					p = p->prox;
				}
				printf("achou no\n");
				
				novo = insereNo(p, pid, path, tipo, prioridade, numBilhetes);
				novo->ant = ant;

				if (ant == NULL) {
					listaPrioridade = novo;
				}
				else {
					ant->prox = novo;
				}
			}	

			imprimeListaPrioridade(listaPrioridade);
			printf("\n\n");
		}
		else if (tipo == 2) {
			listaLoteria = insereNo(listaLoteria, pid, path, tipo, prioridade, numBilhetes);
		}
	}
}

void childHandler(int sinal) {
	printf("Child handler\n");
	int status, tipo;
	No *p = listaPrioridade;
	No *r = listaRoundRobin;
	No *l = listaLoteria;
	id_t id = -1;
	siginfo_t info; 
	pid_t pid;

	// pid = waitpid(-1, &status, WNOWAIT);
	//waitid(P_ALL, id, &info, WNOWAIT | WEXITED | WCONTINUED | WSTOPPED);
	//pid = info.si_pid;
	pid = waitpid(-1, &status, WUNTRACED | WCONTINUED | WNOHANG);
	// esta sempre entrando em CLD_STOPPED, mesmo quando deveria ser CLD_EXITED
	// printf("si_code: %d\n", info.si_code);
	// if (info.si_code == CLD_EXITED) {
	// 	printf("code exited\n");
	// }
	// else if (info.si_code == CLD_STOPPED) {
	// 	printf("code stopped\n");
	// }
	// else if (info.si_code == CLD_CONTINUED) {
	// 	printf("code continued\n");
	// }

	if (WIFEXITED(status) == 1){ 
		printf("terminou normal\n");
		while( p != NULL ){
			if(p->pid == pid) {
				tipo = 1;
			}
			p = p->prox;
	    }
	    while( r != NULL ){
	    	if(r->pid == pid) {
	    		tipo = 0;
	    	}
	    	r = r->prox;
	    }
	    while( l != NULL ){
	    	if(l->pid == pid) {
	    		tipo = 2;
	    	}
	    	l = l->prox;
	    }
	    printf("\nProcesso terminou!\n");

	    if (tipo == 1) {
	    	if (listaPrioridade != NULL) {
				if (listaPrioridade->prox == NULL) {
					free(listaPrioridade);
					listaPrioridade = NULL;
				}
				else {
					listaPrioridade = listaPrioridade->prox;
					free(listaPrioridade->ant);
					listaPrioridade->ant = NULL;
				}
			}
	    }
	    else if (tipo == 0) {
	    	if (listaRoundRobin != NULL) {
	    		if (listaRoundRobin->prox == NULL) {
	    			free(listaRoundRobin);
	    			listaRoundRobin = NULL;
	    		}
	    		else {
	    			listaRoundRobin = listaRoundRobin->prox;
	    			free(listaRoundRobin->ant);
	    			listaRoundRobin->ant = NULL;
	    		}
	    	}

	    }
	    else if (tipo == 2) {
	    	if (listaLoteria != NULL) {

	    		if (listaLoteria->prox == NULL) {
	    			free(listaLoteria);
	    			listaLoteria = NULL;
	    		}
	    		else {
	    			No *tempLista = listaLoteria;
	    			No *noRetirado;

	    			while( tempLista != NULL ) {

	    				if( tempLista->vBilhetes[bilheteSorteado] == 1) {
	    					if(tempLista->ant == NULL) {
	    						noRetirado = tempLista;
	    						listaLoteria = tempLista->prox;
	    						free(noRetirado);
	    						listaLoteria->ant = NULL;
	    					}
	    					else if( tempLista->prox == NULL ){
	    						noRetirado = tempLista;
	    						tempLista->ant->prox = NULL;
	    						free(noRetirado);
	    					}
	    					else {
	    						tempLista->ant->prox = tempLista->prox;
		    					tempLista->prox->ant = tempLista->ant;
		    					free(tempLista);
	
	    					}					
	    					break;
	    				}
	    				tempLista = tempLista->prox;
	    			}

	    		}
	    	}
	    }

		
	}
	else if( WIFCONTINUED(status) == 1 ) {
			printf("foi continued\n");
		}
	else if (WIFSTOPPED(status) == 1 ){
		printf("foi stoped\n");
	}
}

void alarmHandler(int sinal) {
	if (listaRoundRobin != NULL) {
		kill(listaRoundRobin->pid, SIGSTOP);
		printf("stop depois de 1s\n");
		realocaProcessoRoundRobin ();
		if (listaPrioridade == NULL) {
			alarm(TIME_SHARE);
		}
	}
	else if (listaLoteria != NULL) {
		No *tempLista = listaLoteria;
		No *novaLista = listaLoteria;
		int r, achou = 0 ;

		printf("Handler loteria\n");
		
		while (tempLista != NULL ) {
			if(tempLista->vBilhetes[bilheteSorteado] == 1) {
				kill(listaLoteria->pid, SIGSTOP);

				do {
					r = rand() % MAX_BILHETES;

					while( novaLista != NULL ) {

						if( novaLista->vBilhetes[r] == 1) {
							printf("Achei o bilhete premiado!\n");
							achou = 1;
							bilheteSorteado = r;
							kill(novaLista->pid, SIGCONT);

							if( listaPrioridade == NULL && listaRoundRobin == NULL ) {
								alarm(TIME_SHARE);
							}
							break;
						}
						novaLista = novaLista->prox;
					}
				}
				while( achou == 0 );

				break;
			}
			tempLista = tempLista->prox;
		}
	}
}

void rodaProcessoPrioridade(No *processoAnterior) {
	// printf("comecou a rodar\n");
	if (listaPrioridade != NULL) {
		if( processoAnterior == NULL){
			printf("primeiro a rodar\n");
			kill(listaPrioridade->pid, SIGCONT);
		}
		else if( processoAnterior != listaPrioridade) {
			printf("trocou o processo a rodar");
			kill(processoAnterior->pid, SIGSTOP);
			kill(listaPrioridade->pid, SIGCONT);
		}
		else {
			kill(listaPrioridade->pid, SIGCONT);	
		}
	}

}

void rodaProcessoRoundRobin (No *processoAnterior) {
	if (listaPrioridade == NULL && listaRoundRobin != NULL) {
		// VERIFICAR ISSO AQUI
		kill(listaRoundRobin->pid, SIGCONT);
		if (listaPrioridade == NULL && processoAnterior == NULL) {
			alarm(TIME_SHARE);
		}
	}
}

void rodaProcessoLoteria(No* processoAnterior) {

	int r;
	No* tempLista = listaLoteria;
	int achou = 0;
	int i;
	//imprimeListaLoteria(listaLoteria);

	if (listaPrioridade == NULL && listaRoundRobin == NULL && listaLoteria != NULL ) {

		if ( processoAnterior == NULL ) {
			printf("Roda loteria\n");
			
			do {
				r = rand() % MAX_BILHETES;
				while( tempLista != NULL ) {
					if( tempLista->vBilhetes[r] == 1) {
						printf("Achei o bilhete premiado!\n");
						achou = 1;
						bilheteSorteado = r;
						kill(tempLista->pid, SIGCONT);

						if( listaPrioridade == NULL && listaRoundRobin == NULL ) {
							printf("Primeiro alarme loteria\n");
							alarm(TIME_SHARE);
						}
						break;
					}
					tempLista = tempLista->prox;
				}

			}
			while( achou == 0 );
			
		}
	}
}

void realocaProcessoRoundRobin () {

	No *processo;
	No *tempLista;

	if (listaRoundRobin != NULL && listaRoundRobin->prox != NULL) {

		processo = listaRoundRobin;

		listaRoundRobin = listaRoundRobin->prox;
		processo->prox = NULL;
		listaRoundRobin->ant = NULL;
		tempLista = listaRoundRobin;

		while (tempLista->prox != NULL) {
			tempLista = tempLista->prox;
		}

		tempLista->prox = processo;
		processo->ant = tempLista;
	}
}

void liberaEscalonador() {
	liberaLista(listaRoundRobin);
	liberaLista(listaPrioridade);
	liberaLista(listaLoteria);
}

int main() {

	int segmentoEnd, segmentoNovaInfoFlag, segmentoPath, segmentoNumTickets, segmentoPrioridade, segmentoTipo;
	char *path; //[15];
	int *end, *numTickets, *prioridade, *tipo, *novaInfoFlag;
	
	srand((unsigned int)time(NULL));

	signal(SIGCHLD, childHandler);
	signal(SIGALRM, alarmHandler);

	segmentoNovaInfoFlag = shmget(CHAVE_INFO_FLAG, sizeof(int), S_IRUSR | S_IWUSR );
	if( segmentoNovaInfoFlag < 0 ) { 
		printf(" erro ao criar segmento de novainfoflag\n");
		exit(1);
	}
	novaInfoFlag = (int *) shmat(segmentoNovaInfoFlag, 0, 0);

	segmentoEnd = shmget(CHAVE_END, sizeof(int), S_IRUSR | S_IWUSR );
	if( segmentoEnd < 0 ) { 
		printf(" erro ao criar segmento de end\n");
		exit(1);
	}
	end = (int *) shmat(segmentoEnd, 0, 0);


	segmentoPrioridade = shmget(CHAVE_PRIORIDADE, sizeof(int), S_IRUSR | S_IWUSR );
	if( segmentoPrioridade < 0 ) { 
		printf(" erro ao criar segmento de prioridade\n");
		exit(1);
	}
	prioridade = (int *) shmat(segmentoPrioridade, 0, 0);

	segmentoTipo = shmget(CHAVE_TIPO, sizeof(int), S_IRUSR | S_IWUSR );
	if( segmentoTipo < 0 ) { 
		printf(" erro ao criar segmento de tipo\n");
		exit(1);
	}
	tipo = (int *) shmat(segmentoTipo, 0, 0);

	segmentoNumTickets = shmget(CHAVE_NUM_TICKETS, sizeof(int), S_IRUSR | S_IWUSR );
	if( segmentoNumTickets < 0 ) { 
		printf(" erro ao criar segmento de numtickets\n");
		exit(1);
	}
	numTickets = (int *) shmat(segmentoNumTickets, 0, 0);

	segmentoPath = shmget(CHAVE_PATH, sizeof(char) * 15, S_IRUSR | S_IWUSR );
	if( segmentoPath < 0 ) { 
		printf(" erro ao criar segmento de path\n");
		exit(1);
	}
	path = (char *) shmat(segmentoPath, 0, 0);
	
	while(1) {

		No *processoAnteriorPrioridade = listaPrioridade;
		No *processoAnteriorRoundRobin = listaRoundRobin;
		No *processoAnteriorLoteria    = listaLoteria;

		if(*novaInfoFlag == 1){
	
			printf("path: %s, tipo: %d, numTickets: %d, prioridade: %d \n", path, *tipo, *numTickets, * prioridade );			
			*novaInfoFlag = 0;
			insereProcesso(path, *tipo, *prioridade, *numTickets);
		}
		// mudar para && dps, ta assim pra testes q nao esvazia a lista
		else if (*end == 1 && (listaPrioridade == NULL && listaLoteria == NULL && listaRoundRobin == NULL)) {
			break;
		}
		rodaProcessoPrioridade(processoAnteriorPrioridade);
		rodaProcessoRoundRobin(processoAnteriorRoundRobin);
		rodaProcessoLoteria(processoAnteriorLoteria);
	}

	printf("Escalonador terminou de executar todos programas.\n");
	// libera memoria compartilhada do processo
	shmdt(tipo);
	shmdt(prioridade);
	shmdt(numTickets);
	shmdt(novaInfoFlag);
	shmdt(path);

	// libera memoria compartilhada
	shmctl(segmentoPath, IPC_RMID, 0);
	shmctl(segmentoTipo, IPC_RMID, 0);
	shmctl(segmentoPrioridade, IPC_RMID, 0);
	shmctl(segmentoNumTickets, IPC_RMID, 0);
	shmctl(segmentoNovaInfoFlag, IPC_RMID, 0);

	liberaEscalonador();
	
	return 0;
}