#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstddef>

#include <sys/msg.h>
#include <sys/wait.h>
#include <signal.h>

#include <unistd.h>
#include <time.h>

#include "gerente.h"


std::string obterNomeArq(char *arq){
	std::string nome_arq(arq);
	std::size_t pos = nome_arq.find_last_of("/");
	if(pos != std::string::npos) nome_arq = nome_arq.substr(pos+1);
	return nome_arq;
}

int obterHorarioAtual(){
	time_t t = time(NULL);
	struct tm *horario = localtime(&t);
	return ((horario->tm_hour*60*60 + horario->tm_min*60 + horario->tm_sec) % 86400);
}

void shutdown(int sig){
	int status1, status2;

	if(identificador == ID_1 && estado == OCUPADO) //A verificacao para ID_1 serve apenas para verificar apenas 1 job
		printf("Job que está executando sob a solicitação de shutdown: %d, arquivo %s\n", msg.job, msg.arq);
	
	if(identificador < ID_8){ //Envia sinais de shutdown para os filhos (somente os ńos que possuem filhos enviam)
		kill(pid_filho_1, SIGUSR1); 
		kill(pid_filho_2, SIGUSR1);
		wait(&status1); //Espera o termino da execucao dos filhos que receberam o sinal de termino 
		wait(&status2);
	}

	exit(0); //exit(1);
}

void trabalha(){
	int pid_aux;
	int msgsize = sizeof(struct exec) - sizeof(long);
	int wait_status;
	int identificador_pai = identificador/2; //Calculo básico para acha o ID do no pai (considerando arredondamento)

	//O gerente irá trabalhar de maneira diferente para os nós que possuem ou não possuem filhos
	if(identificador < ID_8){
		while(true){
			//Verifica mensagens que são recebidas do escalonador (de execucao)
			if(msgrcv(msgqid_down, &msg, msgsize, identificador, IPC_NOWAIT) != -1){
				
				msgfilho = msg;
				//Envia as mensagens para os filhos do nó na árvore
				//Envia a mesma mensagem, porém com destinos (tipo) diferente
				msgfilho.tipo = identificador*2; //Calculo basico para achar identificador do filho
				msgsnd(msgqid_down, &msgfilho, msgsize ,0);
				msgfilho.tipo = identificador*2+1; //Calculo basico para achar identificador do filho
				msgsnd(msgqid_down, &msgfilho, msgsize ,0);

				//Controle de acesso de execucao do Gerente
				if(estado == LIVRE){
					estado = OCUPADO;
					
					//Cria processo para execucao do arquivo solicitado
					pid_aux = fork();
					if(pid_aux < 0 ){ printf("Erro na criação de processo a partir do Fork()\n"); exit(1); }
					if(pid_aux == 0){
						const char* nome_arq = obterNomeArq(msg.arq).c_str();
						execl(msg.arq, nome_arq, (char*) NULL); //Execucao de processo filho do gerente
					}else{ //Gerente entra em modo espera até o termino da execucao do arquivo
						wait(&wait_status);

						//Utiliza dessa struct exec para mandar informacoes acerca do termino da execucao para o escalonador
						//Necessario para verificar a possibilidade de envio de um novo Job para os gerentes
						if(identificador != ID_1){ //Se não for no 1, direciona confirmacao para o pai
							msg.tipo = identificador_pai;
						}else{ //Se for o no 1 manda direto para o escalonador
							msg.tipo = CONFIRMACAO_EXECUCAO;
						}
						msg.tempo_termino = obterHorarioAtual();
						//Utiliza a fila de envio para o escalonador
						msgsnd(msgqid_up, &msg, msgsize ,0);
						estado = LIVRE; 
					}		
				}
			}
			//Caso um no intermediario receba uma confirmacao (Fila de mensagens UP) reenvia para o pai ate chegar
			//ao escalonador
			if(msgrcv(msgqid_up, &msg, msgsize, identificador, IPC_NOWAIT) != -1){
				if(identificador != ID_1){ //Se não for no 1, direciona confirmacao para o pai
						msg.tipo = identificador_pai;
					}else{ //Se for o no 1 manda direto para o escalonador
						msg.tipo = CONFIRMACAO_EXECUCAO;
				}
				msgsnd(msgqid_up, &msg, msgsize ,0);
			}
		}
	//Execucao para nos sem filhos diferenciada (não haverá propagacao, nem verificar se recebe confirmacoes)
	}else{
		while(true){
			//Verifica o recebimento de solicitacao de execucao
			msgrcv(msgqid_down, &msg, msgsize, identificador, 0);
			if(estado == LIVRE){
				estado = OCUPADO;
				pid_aux = fork();
				if(pid_aux < 0 ){ printf("Erro na criação de processo a partir do Fork()\n"); exit(1); }
				if(pid_aux == 0){
					const char* nome_arq = obterNomeArq(msg.arq).c_str();
					execl(msg.arq, nome_arq, (char*) NULL); //Executa arquivo
				}else{
					wait(&wait_status); //Gerente espera o fim do programa executado
					estado = LIVRE; 
					msg.tempo_termino = obterHorarioAtual();
					msg.tipo = identificador_pai;
					msgsnd(msgqid_up, &msg, msgsize ,0); //Manda confirmacao de termino da execucao para o pai
				}		
			}
		}
	}
}

int main (int argc, char *argv[]){
	signal(SIGUSR1, shutdown);

	//Conversão de parametros recebidos
	identificador = atoi(argv[1]);
	if(argc > 2){
		pid_filho_1 = atoi(argv[2]);
		pid_filho_2 = atoi(argv[3]);
	}

	//Fila de mensagens direcionadas ao escalonador ("Volta/Subida")
	key_t msgkey_up = 0x14002713;
	if((msgqid_up = msgget(msgkey_up, 0x1B6)) < 0){ //permissão: 110 110 110 (RWX)
		printf("Erro na obtencao da fila a partir do msgget");
		exit(1);
   	}

	//Fila de mensagens que saem do escalonador ("Ida/Descida")
	key_t msgkey_down = 0x14002712;
	if((msgqid_down = msgget(msgkey_down, 0x1B6)) < 0){//permissão: 110 110 110 (RWX)
		printf("Erro na obtencao da fila a partir do msgget");
		exit(1);
   	 }

	trabalha(); //Rotina de funcionamento do gerente

	return 0;
}
