#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <sys/msg.h>
#include <sys/wait.h>
#include <signal.h>

#include <unistd.h>
#include <time.h>
#include "estrutura.h"

#define OCUPADO 1
#define LIVRE 0

int identificador;
int pid_filho_1, pid_filho_2;
int msgqid_up, msgqid_down;

int estado;

int obterHorarioAtual(){
	time_t t = time(NULL);
	struct tm *horario = localtime(&t);
	return ((horario->tm_hour*60*60 + horario->tm_min*60 + horario->tm_sec) % 86400);
}

void shutdown(int sig){
	int status1, status2;

	if(identificador < 8){ //Exclui nivel 4 da arvore
		kill(pid_filho_1, SIGUSR1);
		kill(pid_filho_2, SIGUSR1);
		wait(&status1);
		wait(&status2);
	}

	exit(0); //exit(1);
}

void trabalha(){
	struct exec msg, msgfilho;
	int pid_aux;
	int msgsize = sizeof(struct exec) - sizeof(long);
	int wait_status;
	int identificador_pai = identificador/2;
	//int conta_terminos = 0;

	if(identificador < 8){
		while(true){
			
			if(msgrcv(msgqid_down, &msg, msgsize, identificador, IPC_NOWAIT) != -1){
			
				msgfilho = msg;
				msgfilho.tipo = identificador*2;
				msgsnd(msgqid_down, &msgfilho, msgsize ,0);
				msgfilho.tipo = identificador*2+1;
				msgsnd(msgqid_down, &msgfilho, msgsize ,0);

				if(estado == LIVRE){
					estado = OCUPADO;
					
					pid_aux = fork();
					if(pid_aux < 0 ){
						printf("Erro na criação de processo a partir do Fork()\n"); exit(1); }
					if(pid_aux == 0){
						execl(msg.arq, msg.arq, (char*) NULL);
					}else{
						wait(&wait_status);

						if(identificador != 1){
							msg.tipo = identificador_pai;
						}else{
							msg.tipo = 51;
						}
						msg.tempo_termino = obterHorarioAtual();
						msgsnd(msgqid_up, &msg, msgsize ,0);
						estado = LIVRE; 
					}		
				}
			}
			if(msgrcv(msgqid_up, &msg, msgsize, identificador, IPC_NOWAIT) != -1){
				if(identificador != 1){
						msg.tipo = identificador_pai;
					}else{
						msg.tipo = 51;
				}
				msgsnd(msgqid_up, &msg, msgsize ,0);
			}
		}
	}else{
		while(true){
			msgrcv(msgqid_down, &msg, msgsize, identificador, 0);
			if(estado == LIVRE){
				estado = OCUPADO;
				pid_aux = fork();
				if(pid_aux < 0 ){
				printf("Erro na criação de processo a partir do Fork()\n"); exit(1); }
				if(pid_aux == 0){
					execl(msg.arq, msg.arq, (char*) NULL);
				}else{
					wait(&wait_status);
					estado = LIVRE; 

					msg.tempo_termino = obterHorarioAtual();
					msg.tipo = identificador_pai;
					msgsnd(msgqid_up, &msg, msgsize ,0);
				}		
			}
		}
	}
}

int main (int argc, char *argv[]){
	signal(SIGUSR1, shutdown);

	identificador = atoi(argv[1]);
	if(argc > 2){
		pid_filho_1 = atoi(argv[2]);
		pid_filho_2 = atoi(argv[3]);
	}

	key_t msgkey_up = 0x14002713;
	if((msgqid_up = msgget(msgkey_up, 0x1B6)) < 0){
		printf("Erro na obtencao da fila a partir do msgget");
		exit(1);
    }

    //FILA DE IDA
    key_t msgkey_down = 0x14002712;
	if((msgqid_down = msgget(msgkey_down, 0x1B6)) < 0){
		printf("Erro na obtencao da fila a partir do msgget");
		exit(1);
    }

	trabalha();

	return 0;
}