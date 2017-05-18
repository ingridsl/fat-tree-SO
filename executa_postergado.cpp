#include "executa_postergado.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>

#include <sys/msg.h>
#include <sys/wait.h>

#include <unistd.h>
#include <time.h>


using namespace std;

void checaQuantidade(int quantidade){
	if(quantidade != 3){
		printf("ERRO >> quantidade errada de argumentos\n");
		exit(1);
	}
}

//verifica se numero de sequencia enviado está ok
int numOk(char * seg){
	int tam = strlen(seg);

	for (int i = 0; i < tam; ++i){
		if(!isdigit(seg[i])){
			printf("ERRO >> Argumento não é um número\n");
			exit(0);
		}
	}
}

//verifica se arquivo executavel está ok
int arquivoOK(char * arqExe){
	if(access(arqExe, F_OK)==-1){
		printf("ERRO >> Arquivo inexistente\n");
		exit(0);
	}
}

int verificaFila(key_t msgkey){
	if(msgget(msgkey, 0x1B6) < 0){
		printf("ERRO >> Nenhuma fila encontrada \n");
		exit(1);
	}

	msgqid = msgget(msgkey, 0x1B6);
	return msgqid;
}


int encontraUltimoJob(){
	struct job previousJob;
	previousJob.job = 1;
	int num_aux = 0;
	int msgqid;

	//Verifica a existencia de filas 
	msgqid = verificaFila(0x14002713);

	//Recebe o ultimo job
	if (msgrcv(msgqid, &previousJob, sizeof(previousJob), 0, IPC_NOWAIT) < 0){
		//printf("ERRO >> Nenhum numero de job encontrado na fila\n");
		//exit(1);
		return 1;
	}
   	num_aux = previousJob.job;

   	return num_aux;
}


void executaPostergado(int seg, char arq_executavel[N]){

	// checagem de quantidade de parametros

	int msgqid;
	struct mensagem msg;

	//Define o tipo
	msg.tipo = 4;
	msg.times = 1;
	//define o identificador unico do job
	msg.job = encontraUltimoJob();


	strcpy(msg.arq, arq_executavel);

	msg.offset = seg;

    //Soh pode escrever na fila caso ela jah exista
	msgqid = verificaFila(0x14002713);

	//enviar o novo job inserido pelo usuário para tratamento
	int msgsize = sizeof(struct mensagem) - sizeof(long);
	printf("\njob = %d, arquivo = %s, delay = %d segundos", msg.job, msg.arq, msg.offset);
    	if(msgsnd(msgqid , &msg, msgsize, 2) < 0){

		printf("Problema ao enviar as info do novo job\n");
	}

	// Essa parte já está implementada pelo escalonador
	/*time_t start = time(NULL);

	while(1) 
		if(difftime(start, time(NULL))*(-1) == seg)
			break;*/

	printf("\nInicio da execucao apos %d segundos\n", seg);
	/*A partir daqui o processo escalonador de execução solicita que todos os processos gerentes de execução executem o programa <arq_executavel>, marcando-os como ocupados*/
}

int main (int argc, char *argv[]){
	checaQuantidade(argc);
	numOk(argv[1]);
	arquivoOK(argv[2]);
	executaPostergado(atoi(argv[1]), argv[2]);
	return 0;
}
