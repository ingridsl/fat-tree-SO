#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <vector>

#include <unistd.h>
#include <time.h>

#include "estrutura.h"
#include "escalonador.h"


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
	novo.tempo_solicitacao = horario_atual;
	novo.tempo_termino = 0;
	novo.tempo_inicio = 0;

	//printf("\n\n%d, %s, %d\n", (int)novo.job, novo.arq, novo.tempo);

	execucoes_pendentes.push_back(novo);	
}

void checaEscalonador(){
	int horarioAtual = obterHorarioAtual();

	if(!execucoes_pendentes.empty()){
		std::vector<struct exec>::iterator it = execucoes_pendentes.begin();
		for( ; it != execucoes_pendentes.end(); ){

			if(it->tempo <= horarioAtual && ocupado != 1){
				ocupado = 1;
				struct exec aux = execucoes_pendentes[std::distance(execucoes_pendentes.begin(), it)];
				execucoes_pendentes.erase(it);
				int msgsize = sizeof(struct exec) - sizeof(long);

				//printf("\n %d, %s, %d", (int)aux.tipo, aux.arq, aux.tempo);
				aux.tempo_inicio = obterHorarioAtual();
				msgsnd(msgqid_down, &aux, msgsize ,0); //manda para filho
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

void imprimeConcluido(struct exec finalizado){
	int hora_ini = (finalizado.tempo_inicio / (60*60));
	int minuto_ini = ((finalizado.tempo_inicio - (hora_ini*60*60))/60);
	int segundo_ini = (finalizado.tempo_inicio - hora_ini*60*60 - minuto_ini*60);
	int hora_fim = (finalizado.tempo_termino / (60*60));
	int minuto_fim = ((finalizado.tempo_termino - (hora_fim*60*60))/60);
	int segundo_fim = (finalizado.tempo_termino - hora_fim*60*60 - minuto_fim*60);
	printf("\nJob: %d terminado, arquivo: %s, inicio de execucao as %dh%dm%ds e fim %dh%dm%ds, com turnaround de: %d\n",
		finalizado.job, finalizado.arq, hora_ini,minuto_ini,segundo_ini,
		hora_fim,minuto_fim,segundo_fim, finalizado.tempo_termino - 
		finalizado.tempo_inicio);
}

void imprimeRestante(){
	if(!execucoes_pendentes.empty()){
		printf("\nExecucoes que ficaram pendentes:\n");
		for(std::vector<int>::size_type i = 0; i != execucoes_pendentes.size(); i++) {
   			printf("Job: %d, arquivo %s, delay %d\n", (int) execucoes_pendentes[i].job,
   			 execucoes_pendentes[i].arq, execucoes_pendentes[i].tempo - execucoes_pendentes[i].tempo_solicitacao );
		}
	}
	execucoes_pendentes.clear();
}

void imprimeExecutados(){

	if(!execucoes_terminadas.empty()){
		printf("\nExecucoes com exito:");
		for(std::vector<int>::size_type i = 0; i != execucoes_terminadas.size(); i++) {
   			imprimeConcluido(execucoes_terminadas[i]);
		}
	}
	execucoes_terminadas.clear();

}

void shutdown(int sig){
	printf("\nServidor desligando...\n");
	kill(pid_filho, SIGUSR1);
	
	int status;
	wait(&status);

	imprimeRestante();
	imprimeExecutados();

	msgctl(msgqid_up, IPC_RMID, NULL);
	msgctl(msgqid_down, IPC_RMID, NULL);
	exit(0); //exit(1);
}


void executaEscalonador(){
	signal(SIGUSR1, shutdown); //Rotina de desligamento caso receba sinal
	cont_termino = 0; // Contador de processos terminados (min 0, max 15)
	ocupado = 0; // Valor de ocupação dos gerentes de processos

	//Fila de mensagens direcionadas ao escalonador ("Volta/Subida")
	key_t msgkey_up = 0x14002713;
	if((msgqid_up = msgget(msgkey_up, IPC_CREAT | 0x1B6)) < 0){ //permissão: 110 110 110 (RWX)
		printf("Erro na criação da fila a partir do msgget"); exit(1);
    }
   	//Fila de mensagens que saem do escalonador ("Ida/Descida")
   	key_t msgkey_down = 0x14002712;
	if((msgqid_down = msgget(msgkey_down, IPC_CREAT | 0x1B6)) < 0){ //permissão: 110 110 110 (RWX)
		printf("Erro na criação da fila a partir do msgget"); exit(1);
   	}

   	//Cria nós da fila
	criarGerentes();

	//Estrutura de mensagem simples para receber solicitações
	struct mensagem msg;
	//Estrutura de mensagem simples para tratar de jobs finalizados (contém tempos)
	struct exec finalizado;

	//Tamanho das estruturas acima
	int msgsize = sizeof(struct mensagem) - sizeof(long);
	int execsize = sizeof(struct exec) - sizeof(long);

	while(true){
		//Recebe mensagens abaixo do valor do RANGE_REQUISICOES (Valores menores que 50), os valores dos cases utilizados 
		//se encontram no arquivo estrutura.h
		if(msgrcv(msgqid_up, &msg, msgsize, RANGE_REQUISICOES, IPC_NOWAIT) != -1){
			//Caso receba a mensagem verifica qual ação deve ser tomada baseada em seu tipo.
			switch(msg.tipo){
				case ADICIONA_JOB:
					adicionaExecucaoPostergada(msg);
					break;
				case SHUTDOWN:
					raise(SIGUSR1);
				break;
			}	
		}

		//Recebe a estrutura de jobs finalizados dos nós ao final da execucao, para realizar as estatisticas.
		if(msgrcv(msgqid_up, &finalizado, execsize, CONFIRMACAO_EXECUCAO, IPC_NOWAIT) != -1){
			cont_termino++; //Incremeta o valor de confirmações recebidas para verificar a quantidade de nos
			if(cont_termino == N_NOS){
				ocupado = 0; //Caso todos os processos tenham sido encerrados e com a devida confirmacao, libera para outro job
				cont_termino = 0; 
				finalizado.tempo_termino = obterHorarioAtual();

				execucoes_terminadas.push_back(finalizado); //Adiciona ao vetor de execucoes terminadas.
				finalizado = execucoes_terminadas.back(); //Imprime as informações do job que acabou de ser finalizado
				imprimeConcluido(finalizado);	
			}
		}

		//Rotina para testar os tempos de delay e comandar a execucao de jobs
		checaEscalonador();
	}
}

int main(){
	executaEscalonador();
	return 0; 
}
