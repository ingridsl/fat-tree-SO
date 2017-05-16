#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string>

#define N 100

using namespace std;

int main (int argc, char *argv[]){
	if(argc != 3){
		printf("ERRO >> quantidade errada de argumentos");
		exit(1);
	}

	int seg = atoi(argv[1]); 
	char arq_executavel[N];
	strcpy(arq_executavel, argv[2]);
	printf("%d, %s\n", seg, arq_executavel);

	/*Se não houver parâmetro invalido, é atribuido um número de job único à tupla <arq_exec><seq> q é colocada numa estrutura de dados compartilhada*/

	time_t start = time(NULL);

	while(1) 
		if(difftime(start, time(NULL))*(-1) == seg)
			break;

	printf("\nInicio da execucao apos %d segundos\n", seg);
	/*A partir daqui o processo escalonador de execução solicita que todos os processos gerentes de execução executem o programa <arq_executavel>, marcando-os como ocupados*/

return 0;
}
