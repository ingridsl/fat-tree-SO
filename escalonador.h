#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "estrutura.h"

#include <sys/types.h>
#include <vector>
#include <queue>

int job_id, msgqid_recebimento, msgqid_envio;// 15 ou 16
int n_pid = 0; // coloquei só pra auxiliar enquanto não tem o fat tree
int pid_filho;

std::vector<struct exec> execucoes_pendentes;
std::queue<struct exec> proc_running;

int obterHorarioAtual();

void shutdown(int sig);

void listaProcessos();
void rodaProcessos();
void removeProcesso(int id);

void adicionaEscalonador(int job, char* arq, int offset);
void checaEscalonador();
void executaEscalonador();
void criarGerentes();

#endif
