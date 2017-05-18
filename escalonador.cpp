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

void criarFila(key_t msgkey){
	// Cria a fila de mensagens 0x1B6, 0666 = 0x29A
	if((msgqid = msgget(msgkey, IPC_CREAT | 0x1B6)) < 0){
		perror("Erro ao executar msgget: ");
		exit(1);
    }
}

void shutdown(int sig){
	std::cout << "\nServidor deligando...\n\nProcessos que não serão executados: ";
	listaProcessos();
	std::cout << "\nProcessos que foram executados: \n";
	Log::printText();

	msgctl(msgqid, IPC_RMID, NULL);

	// Pode ter que dar kill nos processos e wait
	for (int i = 0; i < n_pid /*16*/; i++)
       kill(pid_filho[i], SIGKILL);
	int status;
	while(wait(&status) != -1);

	exit(0); //exit(1);
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
		pid_filho[n_pid++] = pid;
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

void adicionaEscalonador(int job, char* arq, int offset, int times){
	std::cout << "\njob_id = " << job_id << ": " << arq << " com offset de " << 
		offset << " segundo(s) e executando " << times << " vez(es)" << std::endl;
	int horarioAtual = obterHorarioAtual();
	for(int i = 1; i <= times; ++i){
		proc_scheduled.push_back(Processo(job_id, horarioAtual + i*offset, arq));
	}
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

void executaEscalonador(){
	signal(SIGTERM, shutdown);

	Log::appendText(std::string("job\tarq_exec\t\thh:mm:ss"));
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
					adicionaEscalonador(msg.job, msg.arq, msg.offset, msg.times);
					break;
			}	
		}

		checaEscalonador();
		rodaProcessos();
		//sleep(1);

	}
}

// TODO: Ver a necessidade de adicionar semáfora para partes críticas
// TODO: Ver melhor como se encaixa o fat tree
// TODO: Ver melhor questão das estruturas de dados, se há outras pra adicionar
int main(){
	executaEscalonador();
	return 0; //exit(0);
}
