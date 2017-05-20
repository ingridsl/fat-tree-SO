#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <vector>
#include <queue>

#include <unistd.h>
#include <time.h>
#include "estrutura.h"
#include "executa_postergado.h"

using namespace std;

void checaQuantidade(int quantidade){
	if(quantidade != 3){
		printf("ERRO >> quantidade errada de argumentos\n");
		exit(1);
	}
}

void numOk(char * seg){
	int tam = strlen(seg);

	for (int i = 0; i < tam; ++i){
		if(!isdigit(seg[i])){
			printf("ERRO >> Argumento não é um número\n");
			exit(0);
		}
	}
}

void arquivoOK(char * arqExe){
	if(access(arqExe, F_OK)==-1){
		printf("ERRO >> Arquivo inexistente\n");
		exit(0);
	}
}

int encontraUltimoJob(){
	struct job msgjobrcv;
	struct job msgjobsnd;
	int jobaux;
	int jobsize = sizeof(struct job) - sizeof(long);

	msgjobsnd.tipo = 1;
	//Recebe o ultimo job
	if (msgrcv(msgqidjob, &msgjobrcv, jobsize, 0, IPC_NOWAIT) < 0){
		jobaux = msgjobsnd.job = 1;
		if(msgsnd(msgqidjob , &msgjobsnd, jobsize, 0) < 0)
			printf("Problema ao enviar numero unico de tupla\n");
	}else{
		jobaux = msgjobrcv.job+1;
		msgjobsnd.job = jobaux;

		if(msgsnd(msgqidjob , &msgjobsnd, jobsize, 0) < 0)
			printf("Problema ao enviar numero unico de tupla2\n");
	}

   	return jobaux;
}


void executaPostergado(int seg, char arq_executavel[N]){
	struct mensagem msg;
	

	key_t msgkey_job = 0x14002000;
	if((msgqidjob = msgget(msgkey_job, IPC_CREAT | 0x1B6)) < 0){
		printf("ERRO >> Não foi possível criar fila de tuplas \n");
		exit(1);
	}

	msg.tipo = 49;	//Define o tipo
	msg.job = encontraUltimoJob(); //Define o identificador unico do job
	strcpy(msg.arq, arq_executavel);
	msg.delay = seg;

	key_t msgkey = 0x14002713;
	if((msgqid = msgget(msgkey, 0x1B6)) < 0){
		printf("ERRO >> Nenhuma fila encontrada \n");
		exit(1);
	}

	//enviar o novo job inserido pelo usuário para tratamento
	int msgsize = sizeof(struct mensagem) - sizeof(long);
	printf("\nEnviado job = %d, arquivo = %s, delay = %d segundos\n", msg.job, msg.arq, msg.delay);
    if(msgsnd(msgqid , &msg, msgsize, 0) < 0){
		printf("Problema ao enviar as info do novo job\n");
	}
}

int main (int argc, char *argv[]){
	checaQuantidade(argc);
	numOk(argv[1]);
	arquivoOK(argv[2]);
	executaPostergado(atoi(argv[1]), argv[2]);
	return 0;
}
