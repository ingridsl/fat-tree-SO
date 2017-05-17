#include "processo.h"
#include "log.h"

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

Processo::~Processo(){ }

void Processo::Rodar(){
	status = Pstatus::RODANDO;

	pid_t pid;
	if((pid = fork()) < 0){
		perror("Erro ao executar fork(): ");
	} else if(pid == 0){
		execv(arq.c_str(), NULL);
	} else{
		int status;
		wait(&status);
		std::string aux = "";
		aux.append("\n" + id);
		aux.append("\t" + arq + "\t\t\t");
		aux.append("" + (int)(runtime / 60));
		aux.append(":" + (int)(runtime % 60));
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

time_t Processo::GetRuntime(){
	return runtime;
}