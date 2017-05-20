#include "escalonador.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#include <sys/msg.h>
#include <sys/wait.h>

#include <unistd.h>
#include <time.h>
#include "estrutura.h"

int obterHorarioAtual(){
	time_t t = time(NULL);
	struct tm *horario = localtime(&t);
	return ((horario->tm_hour*60*60 + horario->tm_min*60 + horario->tm_sec) % 86400);
}

void adicionaExecucaoPostergada(struct mensagem recebido){
	int horario_atual = obterHorarioAtual();
	struct exec novo;
	novo.tipo = 1;
	novo.job = recebido.job;
	strcpy(novo.arq,recebido.arq);
	novo.tempo = horario_atual + recebido.delay;

	//printf("\n%d, %s, %d\n", (int)novo.tipo, novo.arq, novo.tempo);

	execucoes_pendentes.push_back(novo);	
}

void checaEscalonador(){
	int horarioAtual = obterHorarioAtual();

	if(!execucoes_pendentes.empty()){
		std::vector<struct exec>::iterator it = execucoes_pendentes.begin();
		for( ; it != execucoes_pendentes.end(); ){

			if(it->tempo <= horarioAtual){
				struct exec aux = execucoes_pendentes[std::distance(execucoes_pendentes.begin(), it)];
				execucoes_pendentes.erase(it);
				int msgsize = sizeof(struct exec) - sizeof(long);

				//printf("\n %d, %s, %d", (int)aux.tipo, aux.arq, aux.tempo);

				msgsnd(msgqid_envio, &aux, msgsize ,0); //manda para filho
			} else {
				it++;
			}
		}
	}
}

void criarGerentes(){
	int pid_filho_aux[2];
	char parametro1[10];
	char parametro2[10];
	char parametronivel[5];
	int nivel1 = 1, nivel2 = 2, nivel3 = 3, nivel4 = 4;


	if((pid_filho = fork()) < 0 ){ /*GERENTE 1 NIVEL 1*/
		printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }
	if(pid_filho == 0){ 

		if((pid_filho_aux[0] = fork()) < 0 ){ 
			printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }
		if(pid_filho_aux[0] == 0){ /* GERENTE 1 NIVEL 2*/
			if((pid_filho_aux[0] = fork()) < 0){ 
				printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }

			if(pid_filho_aux[0] == 0){ /* GERENTE 1 NIVEL 3*/

				if((pid_filho_aux[0] = fork()) < 0){ 
					printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }
				if(pid_filho_aux[0] == 0){ /* GERENTE 1 NIVEL 4*/
					sprintf (parametronivel, "%d", 8);
					execl("gerente", "gerente", parametronivel,  (char*) NULL);
				}
				if((pid_filho_aux[1] = fork()) < 0){ 
					printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }
				if(pid_filho_aux[1] == 0){ /* GERENTE 2 NIVEL 4*/
					sprintf (parametronivel, "%d", 9);
					execl("gerente", "gerente", parametronivel, (char*) NULL);
				}

				//GERENTE 1-3
				sprintf (parametronivel, "%d", 4);
				sprintf (parametro1, "%d", pid_filho_aux[0]);
				sprintf (parametro2, "%d", pid_filho_aux[1]);
				execl("gerente", "gerente", parametronivel, parametro1, parametro2, (char*) NULL);
			}
			if((pid_filho_aux[1] = fork()) < 0){ 
				printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }

			if(pid_filho_aux[1] == 0){ /* GERENTE 2 NIVEL 3*/

				if((pid_filho_aux[0] = fork()) < 0){ 
					printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }
				if(pid_filho_aux[0] == 0){ /* GERENTE 3 NIVEL 4*/
					sprintf (parametronivel, "%d", 10);
					execl("gerente", "gerente", parametronivel, (char*) NULL);
				}
				if((pid_filho_aux[1] = fork()) < 0){ 
					printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }
				if(pid_filho_aux[1] == 0){ /* GERENTE 4 NIVEL 4*/	
					sprintf (parametronivel, "%d", 11);
					execl("gerente", "gerente", parametronivel, (char*) NULL);
				}
				//GERENTE 2-3
				sprintf (parametronivel, "%d", 5);
				sprintf (parametro1, "%d", pid_filho_aux[0]);
				sprintf (parametro2, "%d", pid_filho_aux[1]);
				execl("gerente", "gerente", parametronivel, parametro1, parametro2, (char*) NULL);			
			}

			//Gerente 1-2
			sprintf (parametronivel, "%d", 2);
			sprintf (parametro1, "%d", pid_filho_aux[0]);
			sprintf (parametro2, "%d", pid_filho_aux[1]);
			execl("gerente", "gerente", parametronivel, parametro1, parametro2, (char*) NULL);
		}
		if((pid_filho_aux[1] = fork()) < 0 ){
			printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }
		if(pid_filho_aux[1] == 0){ /* GERENTE 2 NIVEL 2*/
			if((pid_filho_aux[0] = fork()) < 0){ /* GERENTE 3 NIVEL 3*/
				printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }

			if(pid_filho_aux[0] == 0){

				if((pid_filho_aux[0] = fork()) < 0){ 
					printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }
				if(pid_filho_aux[0] == 0){ /* GERENTE 5 NIVEL 4*/
					sprintf (parametronivel, "%d", 12);
					execl("gerente", "gerente", parametronivel, (char*) NULL);
				}
				if((pid_filho_aux[1] = fork()) < 0){ 
					printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }
				if(pid_filho_aux[1] == 0){ /* GERENTE 6 NIVEL 4*/
					sprintf (parametronivel, "%d", 13);
					execl("gerente", "gerente", parametronivel, (char*) NULL);
				}

				//GERENTE 3-3
				sprintf (parametronivel, "%d", 6);
				sprintf (parametro1, "%d", pid_filho_aux[0]);
				sprintf (parametro2, "%d", pid_filho_aux[1]);
				execl("gerente", "gerente", parametronivel, parametro1, parametro2, (char*) NULL);
			}

			if((pid_filho_aux[1] = fork()) < 0){ /* GERENTE 4 NIVEL 3*/
				printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }

			if(pid_filho_aux[1] == 0){
				
				if((pid_filho_aux[0] = fork()) < 0){ 
					printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }
				if(pid_filho_aux[0] == 0){ /* GERENTE 7 NIVEL 4*/
					sprintf (parametronivel, "%d", 14);
					execl("gerente", "gerente", parametronivel, (char*) NULL);
				}
				if((pid_filho_aux[1] = fork()) < 0){ 
					printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }
				if(pid_filho_aux[1] == 0){ /* GERENTE 8 NIVEL 4*/
					sprintf (parametronivel, "%d", 15);
					execl("gerente", "gerente", parametronivel, (char*) NULL);
				}
				//GERENTE 4-3

				sprintf (parametronivel, "%d", 7);
				sprintf (parametro1, "%d", pid_filho_aux[0]);
				sprintf (parametro2, "%d", pid_filho_aux[1]);
				execl("gerente", "gerente", parametronivel, parametro1, parametro2, (char*) NULL);
			}

			//Gerente 2-2
			sprintf (parametronivel, "%d", 3);
			sprintf (parametro1, "%d", pid_filho_aux[0]);
			sprintf (parametro2, "%d", pid_filho_aux[1]);
			execl("gerente", "gerente", parametronivel, parametro1, parametro2, (char*) NULL);
		}

		sprintf (parametronivel, "%d", 1);
		sprintf (parametro1, "%d", pid_filho_aux[0]);
		sprintf (parametro2, "%d", pid_filho_aux[1]);
		execl("gerente", "gerente", parametronivel, parametro1, parametro2, (char*) NULL);
	}
}


void shutdown(int sig){
	printf("\nServidor deligando...\n");
	kill(pid_filho, SIGUSR1);
	
	int status;
	wait(&status);

	msgctl(msgqid_recebimento, IPC_RMID, NULL);
	msgctl(msgqid_envio, IPC_RMID, NULL);
	exit(0); //exit(1);
}


void executaEscalonador(){
	signal(SIGUSR1, shutdown);
	job_id = 0;

	//FILA DE VOLTA
	key_t msgkey_recebimento = 0x14002713;
	if((msgqid_recebimento = msgget(msgkey_recebimento, IPC_CREAT | 0x1B6)) < 0){
		printf("Erro na criação da fila a partir do msgget");
		exit(1);
    }

    //FILA DE IDA
    key_t msgkey_envio = 0x14002712;
	if((msgqid_envio = msgget(msgkey_envio, IPC_CREAT | 0x1B6)) < 0){
		printf("Erro na criação da fila a partir do msgget");
		exit(1);
    }

	criarGerentes();


	struct mensagem msg;
	int msgsize = sizeof(struct mensagem) - sizeof(long);
	while(true){

		if(msgrcv(msgqid_recebimento, &msg, msgsize, -50, IPC_NOWAIT) != -1){
			switch(msg.tipo){
				case 49:
					adicionaExecucaoPostergada(msg);
					break;
				case 50:
					raise(SIGUSR1);
				break;
			}	
		}

		checaEscalonador();
	}
}

int main(){
	executaEscalonador();
	return 0; 
}
