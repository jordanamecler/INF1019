/* 1) Abra duas seções de terminais
Na primeira, execute um programa que fica em loop lendo de uma FIFO para depois escrever na saída padrão (tela)
Na segunda, execute um programa que fica lendo da entrada padrão (teclado) e depois escreve na mesma FIFO */
// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 1

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
	int fifo;
	char ch;

	puts ("inicio");
	if (access ("minhaFifo", F_OK) == -1) {
		if (mkfifo ("minhaFifo", S_IRUSR | S_IWUSR) != 0) {
			puts ("Erro na criação FIFO");
			return -1;
		}
	}

	if ((fifo = open ("minhaFifo", O_RDONLY)) < 0) {
		puts ("Erro ao abrir a FIFO para leitura");
		return -1;
	}

	puts ("vai comecar a ler");
	while (read (fifo, &ch, sizeof (ch)) > 0) {
		puts ("leu\n");
		putchar (ch);
	}

	puts ("fim leitura");

	close (fifo);

	return 0;
}

/* Execução do programa: gcc aula6ex1.c -o aula6ex1
			 			 ./aula6ex1
   Saída:
inicio
vai comecar a ler
leu

aleu


leu

bleu


fim leitura

   Reflexão: Criamos a fifo somente caso ela não existisse. Abrimos em cada programa a fifo, um no modo read only e outro no modo write only. No programa que lê da fifo, ele bloqueia no commando read, e espera até a fifo ter algum caracter para ser lido, quando acha o caracter, imprime na tela. No programa write, ele espera o scanf par que possa escrever na fifo. Quando terminamos o programa que escreve, automaticamente o programa que lê termina.


   */

/* 1) Abra duas seções de terminais
Na primeira, execute um programa que fica em loop lendo de uma FIFO para depois escrever na saída padrão (tela)
Na segunda, execute um programa que fica lendo da entrada padrão (teclado) e depois escreve na mesma FIFO */
// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 1

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main() {
	int fifo;
	char ch;

	if (access ("minhaFifo", F_OK) == -1) {
		if (mkfifo ("minhaFifo", S_IRUSR | S_IWUSR) != 0) {
			puts ("Erro na criação FIFO");
			return -1;
		}
	}

	if ((fifo = open ("minhaFifo", O_WRONLY)) < 0) {
		puts ("Erro ao abrir a FIFO");
		return -1;
	}

	puts ("Escrevendo na fifo o caracter");
	while (ch != '0') {
		scanf ("%c", &ch);
		write (fifo, &ch, 1);
	}

	close (fifo);

	return 0;
}

/* Execução do programa: gcc aula6ex1.2.c -o aula6ex1.2
			 			 ./aula6ex1.2
   Saída:
Escrevendo na fifo o caracter
a
b
^C 

/* 2) Escreva um programa que primeiro cria uma FIFO e  em seguida cria dois processos filho que escrevem uma  string  na  FIFO.  O  pai dá  um  waitpid em seguida lê  as  strings  desse  FIFO e  exibe na tela */
// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 2

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>

int main() {
	int fifo, fifo2;
	char mensagem[21];
	int pid1, pid2;

	puts ("inicio");
	if (access ("minhaFifo", F_OK) == -1) {
		if (mkfifo ("minhaFifo", S_IRUSR | S_IWUSR) != 0) {
			puts ("Erro na criação FIFO");
			return -1;
		}
	}

	if ((pid1 = fork()) < 0) {
		puts( "Erro no fork 1");
	}
	else if (pid1 == 0) {
		if ((fifo = open ("minhaFifo", O_WRONLY)) < 0) {
			puts ("Erro ao abrir a FIFO para leitura e escrita");
			return -1;
		}
		strcpy(mensagem, "Oi");
		write (fifo, mensagem, strlen (mensagem));

		return 0;
	}
	else {
		if ((pid2 = fork()) < 0) {
			puts( "Erro no fork 2");
		}
		else if (pid2 == 0) {
			if ((fifo = open ("minhaFifo", O_WRONLY)) < 0) {
				puts ("Erro ao abrir a FIFO para leitura e escrita");
				return -1;
			}
			strcpy(mensagem, "Tudo bem?");
			write (fifo, mensagem, strlen (mensagem));

			return 0;
		}
		// pai
		else {
			if ((fifo2 = open ("minhaFifo", O_RDONLY)) < 0) {
				puts ("Erro ao abrir a FIFO para leitura e escrita");
				return -1;
			}
			int status;
			waitpid (pid1, &status, 0);
			waitpid (pid2, &status, 0);
			while (read (fifo2, mensagem, 100) > 0) {
				printf ("%s\n", mensagem);
			}
		}
	}


	close (fifo);

	return 0;
}

/* Execução do programa: gcc aula6ex2.c -o aula6ex2
			 			 ./aula6ex2
   Saída:
inicio
OiTudo bem?

   Reflexão: Criamos a fifo caso ainda não existisse. Demos open em cada processo filho, para modo escrita, e no pai para modo leitura. Nos filhos escrevemos mensagens na fifo. No pai esperamos os dois filhos terminarem e escrevemos na tela as strings da fifo.


   */