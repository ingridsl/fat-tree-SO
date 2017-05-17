#include "escalonador.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <sys/msg.h>
#include <sys/wait.h>

#include <unistd.h>
#include <time.h>

//#include <sys/ipc.h>
//#include <signal.h>
//#include <map>
//#include <sstream>

int obterHorarioAtual(){
	time_t t = time(NULL);
	struct tm *horario = localtime(&t);
	return ((horario->tm_hour*60 + horario->tm_min) % 1440);
}

void criarFila(key_t msgkey){
	// Cria a fila de mensagens 0x1B6, 0666 = 0x29A
	if((msgqid = msgget(msgkey, IPC_CREAT | 0x1B6)) < 0){
        perror("Erro ao executar msgget: ");
        exit(1);
    }
}

void shutdown(int sig){
	std::cout << "\nDESLIGANDO SERVIDOR...\nESSES PROCESSOS NAO SERAO EXECUTADOS: ";
	listaProcessos();
	std::cout << "\nESSES PROCESSOS FORAM EXECUTADOS: \n" << log.str();
	msgctl(msgqid, IPC_RMID, NULL);
	// Pode ter que dar kill nos processos e wait
	exit(0);
}

void listaProcessos(){
	std::cout << "\njob\tarq_exec\t\t\thhmm" << std::endl;

	std::vector<Processo>::iterator it = proc_scheduled.begin();
	for( ; it != proc_scheduled.end(); it++){
		std::cout << it->GetId() << "\t" << it->GetArq() << 
			"\t\t" << (int)(it->GetRuntime() / 60) << ":" << (int)(it->GetRuntime() % 60) << std::endl;
	}
}

void rodaProcessos(){
	if(!proc_running.empty()){
		std::string aux = proc_running.front().Rodar();
		log << aux;
		proc_running.pop();
	}
}

void removeProcesso(int id){
	std::vector<Processo>::iterator it = proc_scheduled.begin();
	for( ; it != proc_scheduled.end(); ){
		if(it->GetId() == id){
			it = proc_scheduled.erase(it);
		} else it++;
	}
}

void adicionaEscalonador(char* arq, int offset, int times){
	std::cout << "\nJOB ID" << job_id << ": PROCESSO " << arq << " COM OFFSET DE " << 
		offset << " MINUTOS E EXECUTANDO " << times << " VEZES" << std::endl;
	int horarioAtual = obterHorarioAtual();
	for(int i = 1; i <= times; ++i){
		proc_scheduled.push_back(Processo(job_id, horarioAtual + i*offset, arq));
	}
	job_id++;
}

void checaEscalonador(){
	int horarioAtual = obterHorarioAtual();
	std::vector<Processo>::iterator it = proc_scheduled.begin();
	for( ; it != proc_scheduled.end(); ){
		if(it->GetRuntime() <= horarioAtual){
			it->SetStatus(Pstatus::DORMINDO);
			proc_running.push(*it);
			it = proc_scheduled.erase(it);
		} else it++;
	}
}

void executaEscalonador(){
	signal(SIGTERM, shutdown);

	log << "job\tarq_exec\thhmm";
	job_id = 0;
	key_t msgkey = 0x14002713;
	criarFila(msgkey);

	struct mensagem msg;
	int msgsize = sizeof(struct mensagem) - sizeof(long);
	while(true){

		if(msgrcv(msgqid, &msg, msgsize, -4, IPC_NOWAIT) != -1){
			switch(msg.tipo){
				case 1:
					raise(SIGTERM);
					break;
				case 2:
					listaProcessos();
					break;
				case 3:
					removeProcesso(msg.offset);
					break;
				case 4:
					adicionaEscalonador(msg.arq, msg.offset, msg.times);
					break;
			}	
		}

		checaEscalonador();
		rodaProcessos();
		sleep(1);

	}
}

// TODO: Ver a necessidade de adicionar semáfora para partes críticas
// TODO: Ver melhor como se encaixa o fat tree
// TODO: Ver melhor questão das estruturas de dados, se há outras pra adicionar
int main(){
	executaEscalonador();
	return 0;
}
