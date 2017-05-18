#include "processo.h"
#include "log.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

Processo::Processo(int id, int tempo, std::string a){
	status = Pstatus::AGENDADO;
	this->id = id;
	runtime = tempo;
	arq = a.c_str();	
}

Processo::~Processo(){ }

void Processo::Rodar(){
	status = Pstatus::RODANDO;

	pid_t pid;
	if((pid = fork()) < 0){
		perror("Erro ao executar fork(): ");
	} else if(pid == 0){
		//execl(arq.c_str(), arq.c_str(), (char*) NULL);
		execv(arq.c_str(), NULL);
	} else{
		int s;
		wait(&s);
		int hora = (runtime / (60*60));
		int minuto = (runtime - (hora*60*60)/60);
		int segundo = (runtime - hora*60*60 - minuto*60);

		std::string aux = "";
		aux.append("\n" + id);
		aux.append("\t" + arq + "\t");
		aux.append("" + hora);
		aux.append(":" + minuto);
		aux.append(":" + segundo);
		aux.append("\n");
		Log::appendText(aux);
		status = Pstatus::MORTO;
	}
}

void Processo::SetStatus(Pstatus::tipo s){
	status = s;
}

int Processo::GetId(){
	return id;
}

bool Processo::ConfirmaId(int id){
	return (this->id == id);
}

std::string Processo::GetArq(){
	return arq;
}

int Processo::GetRuntime(){
	return runtime;
}
