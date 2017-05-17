#include "processo.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

Processo::Processo(int id, time_t tempo, std::string a){
	status = Pstatus::AGENDADO;
	this->id = id;
	runtime = tempo;
	arq = a.c_str();	
}

std::string Processo::Rodar(){
	std::string log = "";
	status = Pstatus::RODANDO;

	pid_t pid;
	if((pid = fork()) < 0){
		perror("Erro ao executar fork(): ");
	} else if(pid == 0){
		execv(arq.c_str(), NULL);
	} else{
		int status;
		wait(&status);
		log.append("\n");
		log.append("" + id);
		log.append("\t");
		log.append(arq);
		log.append("\t\t\t");
		log.append("" + (int)(runtime / 60));
		log.append(":");
		log.append("" + (int)(runtime % 60));
		log.append("\n");
		status = Pstatus::MORTO;
	}
	return log;
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

time_t Processo::GetRuntime(){
	return runtime;
}