#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "processo.h"

#include <sys/types.h>
#include <vector>
#include <queue>
#include <sstream>

struct mensagem {
	long tipo;
	char arq[200];
	int offset;
	int times;
};


int job_id, msgqid;
std::vector<Processo> proc_scheduled;
std::queue<Processo> proc_running;
std::ostringstream log;

int obterHorarioAtual();
void criarFila(key_t msgkey);

void shutdown(int sig);

void listaProcessos();
void rodaProcessos();
void removeProcesso(int id);

void adicionaEscalonador(char* arq, int offset, int times);
void checaEscalonador();
void executaEscalonador();

#endif