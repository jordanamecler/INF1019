/* 1)Faça um programa para criar dois processos que se comunicam via pipe. O pai lê do pipe enquanto o filho escreve no pipe. Exiba o resultado do que foi escrito e lido. */
// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 1

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {

	pid_t pid;
	int fd[2];
	char *msg;
	int tamMax = 51;

	if (pipe(fd) < 0) {
		printf("Erro ao abrir os pipes\n");
		exit(-1);
	}

	if ((pid = fork()) < 0) {
		printf("Erro no fork 1\n");
		exit(1);
	}
	else if (pid == 0) {
		
		close(fd[0]);
		msg = "Filho escreveu";
		write(fd[1], msg, sizeof(char) * tamMax);
	}
	else {
		close(fd[1]);
		read(fd[0], msg, sizeof(char) * tamMax);
		printf("%s\n", msg);
	}

	return 0;
}

/* Execução do programa: gcc aula5ex1.c -o aula5ex1
			 			 ./aula5ex1
   Saída: 
Filho escreveu
 

   Reflexão: Criamos um processo filho através do fork que escreve no pipe a mensagem "Filho escreveu" e lemos do pipe no pai a mensagem.

*/

/* 
2)Faça um programa para redirecionar a entrada e a saida,
 lendo os dados de um arquivo e gerando a saida em outro.
 
 Jordana Mecler e Leonardo Wajnsztok
	Exercicio 2
*/

#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>



int main() {

	int ent, saida;
	int retorno, retorno2;
	char string[100];

	if ((ent = open("entrada.txt", O_RDWR | O_CREAT , 0666)) == -1) {

		perror("Error open()");
		printf("erro0\n");
		return -1;
	}

	if ((saida = open("saida.txt", O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1) {

		perror("Error open()");
		printf("erro0\n");
		return -1;
	}
	close(0);

	if((retorno = dup(ent)) == -1) {
		printf("erro1\n");
		perror("Error dup2()");
		return -2;
	}
	
	scanf(" %100s", string);
	close(0);
	// printf("%s\n", string );

	if((retorno2 = dup2(saida,1)) == -1) {
		printf("erro2\n");
		perror("Error dup2()");
		return -3;
	}
	printf("%s \n", string);

	return 0;
}

/* Execução do programa: gcc aula5x2.c -o aula5ex2
./aula5ex2
   
   Saída: 
Texto lido do entrada.txt foi escrito em saida.txt
Inicio:
 - entrada.txt: "entrada"
 - saida.txt : ""
Final:
 - entrada.txt: "entrada"
 - saida.txt : "entrada"


   Reflexão: Usamos as funcoes dup e dup2 para duplicar o descritor de dos arquivos entrada e saida
   e armazena-los no descritor 0 e 1, stdin e stdout. Lemos com scanf entao o conteudo de entrada.txt
   e imprimimos com printf o que foi lido em saida.txt
*/


/* 3) Faça um programa para criar um pipe e executar dois processos que são utilitários do Unix que se comuniquem através do pipe criado (a shell faz isso) */
// Jordana Mecler e Leonardo Wajnsztok
// Exercicio 3

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {

	pid_t pid;
	int fd[2];
	char *msg;
	int tamMax = 51;
	int saida;

	if (pipe(fd) < 0) {
		printf("Erro ao abrir os pipes\n");
		exit(-1);
	}

	if ((saida=open("saida.txt", O_RDWR | O_CREAT)) == -1) {
		perror("Error open()");
		exit(1);
	}

	if ((pid = fork()) < 0) {
		printf("Erro no fork\n");
		exit(1);
	}
	else if (pid == 0) {
		printf("%d\n", getpid());
		dup2(fd[1], 1);
		printf("ls\n");
		char *argv[] = {"ls", NULL};
       	execv("/bin/ls", argv);
       	printf("Failed to execute /bin/ls\n");
        exit(1);
	}
	else {
		int status;
		close(fd[1]);
		waitpid(pid, &status, 0);
		printf("%d\n",pid);
		close(0);
		dup2(fd[0], 0);
		dup2(saida, 1);
		printf("wc\n");
		char *argv[] = {"wc", NULL};
       	execv("/usr/bin/wc", argv);
       	printf("Failed to execute /usr/bin/wc\n");
        exit(1);
	}

	return 0;
}

/* Execução do programa: gcc aula5ex3.c -o aula5ex3
			 			 ./aula5ex3
   Saída no arquivo saida.txt: 
   wc
     54      54     579
 

   Reflexão: Criamos um processo filho através do fork para executar o ls e no pai executamos o wc. Fizemos um dup2 para que o ls escrevesse no pipe e não no terminal. Fizemos um dup2 para que a entrada viesse do pipe e não do terminal, e um outro para dup 2 para que a saida o wc fosse para o arquivo saida.txt e não para o pipe, para que pudéssemos visualizar.
*/
