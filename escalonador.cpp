#include "escalonador.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

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

void listaProcessos(){
	std::cout << "\njob\tarq_exec\t\thh:mm:ss" << std::endl;

	std::vector<Processo>::iterator it = proc_scheduled.begin();
	for( ; it != proc_scheduled.end(); it++){
		// Com as variaveis do processo senfo privadas nao consegue acessar as funcoes de get por causa do wait
		int runtime = it->GetRuntime();
		int hora = (runtime / (60*60));
		int minuto = ((runtime - (hora*60*60))/60);
		int segundo = (runtime - hora*60*60 - minuto*60);
		printf("%d\t%s\t\t%d:%d:%d\n", it->GetId(), it->GetArq().c_str(), hora, minuto, segundo);
	}
}

void rodaProcessos(){
	if(!proc_running.empty()){
		//printf("\nAQUI2");
		pid_t pid = proc_running.front().Rodar();
		//pid_filho[n_pid++] = pid;
		//printf("\nPID_FILHO: %d", pid);
		proc_running.pop();
	}
}

void removeProcesso(int id){
	if(!proc_scheduled.empty()){
		std::vector<Processo>::iterator it = proc_scheduled.begin();
		for( ; it != proc_scheduled.end(); ){
			if(it->GetId() == id){
				it = proc_scheduled.erase(it);
			} else it++;
		}
	}
}

void adicionaEscalonador(int job, char* arq, int offset){
	std::cout << "\njob_id = " << job_id << ": " << arq << " com offset de " << 
		offset << " segundo(s) e executando " << std::endl;
	int horarioAtual = obterHorarioAtual();
	
		proc_scheduled.push_back(Processo(job_id, horarioAtual + offset, arq));
	
	job_id++;
}

void checaEscalonador(){
	if(!proc_scheduled.empty()){
		int horarioAtual = obterHorarioAtual();
		std::vector<Processo>::iterator it = proc_scheduled.begin();
		for( ; it != proc_scheduled.end(); ){
			//printf("\n(%d <= %d)", it->GetRuntime(), horarioAtual);
			if(it->GetRuntime() <= horarioAtual){
				//printf("\nAQUI");
				it->SetStatus(Pstatus::DORMINDO);
				proc_running.push(*it);
				it = proc_scheduled.erase(it);
			} else {
				it++;
				//printf("THERE");
			}
		}
	}
}

void criarGerentes(){
	int pid_filho_aux[2];
	char parametro1[10];
	char parametro2[10];
	char parametronivel[1];
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
					parametronivel[0] = nivel4 + '0';
					execl("gerente", "gerente", parametronivel,  (char*) NULL);
				}
				if((pid_filho_aux[1] = fork()) < 0){ 
					printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }
				if(pid_filho_aux[1] == 0){ /* GERENTE 2 NIVEL 4*/
					parametronivel[0] = nivel4 + '0';
					execl("gerente", "gerente", parametronivel, (char*) NULL);
				}

				//GERENTE 1-3
				parametronivel[0] = nivel3 + '0';
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
					parametronivel[0] = nivel4 + '0';
					execl("gerente", "gerente", parametronivel, (char*) NULL);
				}
				if((pid_filho_aux[1] = fork()) < 0){ 
					printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }
				if(pid_filho_aux[1] == 0){ /* GERENTE 4 NIVEL 4*/	
					parametronivel[0] = nivel4 + '0';
					execl("gerente", "gerente", parametronivel, (char*) NULL);
				}
				//GERENTE 2-3
				parametronivel[0] = nivel3 + '0';
				sprintf (parametro1, "%d", pid_filho_aux[0]);
				sprintf (parametro2, "%d", pid_filho_aux[1]);
				execl("gerente", "gerente", parametronivel, parametro1, parametro2, (char*) NULL);			
			}

			//Gerente 1-2
			parametronivel[0] = nivel2 + '0';
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
					parametronivel[0] = nivel4 + '0';
					execl("gerente", "gerente", parametronivel, (char*) NULL);
				}
				if((pid_filho_aux[1] = fork()) < 0){ 
					printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }
				if(pid_filho_aux[1] == 0){ /* GERENTE 6 NIVEL 4*/
					parametronivel[0] = nivel4 + '0';
					execl("gerente", "gerente", parametronivel, (char*) NULL);
				}

				//GERENTE 3-3
				parametronivel[0] = nivel3 + '0';
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
					parametronivel[0] = nivel4 + '0';
					execl("gerente", "gerente", parametronivel, (char*) NULL);
				}
				if((pid_filho_aux[1] = fork()) < 0){ 
					printf("Erro na criação de árvore a partir do Fork()\n"); exit(1); }
				if(pid_filho_aux[1] == 0){ /* GERENTE 8 NIVEL 4*/
					parametronivel[0] = nivel4 + '0';
					execl("gerente", "gerente", parametronivel, (char*) NULL);
				}
				//GERENTE 4-3

				parametronivel[0] = nivel3 + '0';
				sprintf (parametro1, "%d", pid_filho_aux[0]);
				sprintf (parametro2, "%d", pid_filho_aux[1]);
				execl("gerente", "gerente", parametronivel, parametro1, parametro2, (char*) NULL);
			}

			//Gerente 2-2
			parametronivel[0] = nivel2 + '0';
			sprintf (parametro1, "%d", pid_filho_aux[0]);
			sprintf (parametro2, "%d", pid_filho_aux[1]);
			execl("gerente", "gerente", parametronivel, parametro1, parametro2, (char*) NULL);
		}

		parametronivel[0] = nivel1 + '0';
		sprintf (parametro1, "%d", pid_filho_aux[0]);
		sprintf (parametro2, "%d", pid_filho_aux[1]);
		execl("gerente", "gerente", parametronivel, parametro1, parametro2, (char*) NULL);
	}
}


void shutdown(int sig){
	printf("\nServidor deligando...\n");
	kill(pid_filho, SIGTERM);
	
	int status;
	wait(&status);

	msgctl(msgqid, IPC_RMID, NULL);
	exit(0); //exit(1);
}


void executaEscalonador(){
	signal(SIGTERM, shutdown);
	job_id = 0;

	//FILA DE VOLTA
	key_t msgkey = 0x14002713;
	if((msgqid = msgget(msgkey, IPC_CREAT | 0x1B6)) < 0){
		printf("Erro na criação da fila a partir do msgget");
		exit(1);
    }

	criarGerentes();


	struct mensagem msg;
	int msgsize = sizeof(struct mensagem) - sizeof(long);
	while(true){

		if(msgrcv(msgqid, &msg, msgsize, -50, IPC_NOWAIT) != -1){
			switch(msg.tipo){
				case 49:
					adicionaEscalonador(msg.job, msg.arq, msg.offset);
					break;
				case 50:
					raise(SIGTERM);
				break;
			}	
		}

		checaEscalonador();
		rodaProcessos();
	}
}

int main(){
	executaEscalonador();
	return 0; 
}
