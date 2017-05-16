#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#define N 100

using namespace std;

int main (int argc, char *argv[]){
	if(argc != 3){
		printf("ERRO >> quantidade errada de argumentos");
		exit(1);
	}

	int seq = atoi(argv[1]); 
	char arq_executavel[N];
	strcpy(arq_executavel, argv[2]);
	printf("%d, %s\n", seq, arq_executavel);

	/* se não houver parâmetro invalido, é atribuido um número de job único à tupla <arq_exec><seq> q é colocada numa estrutura de dados compartilhada*/

return 0;
}
