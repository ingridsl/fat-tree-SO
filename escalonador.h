#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "processo.h"

#include <sys/types.h>
#include <vector>
#include <queue>

int job_id, msgqid;
pid_t pid_filho[16]; // 15 ou 16
int n_pid = 0; // coloquei só pra auxiliar enquanto não tem o fat tree
std::vector<Processo> proc_scheduled;
std::queue<Processo> proc_running;

int obterHorarioAtual();
void criarFila(key_t msgkey);

void shutdown(int sig);

void listaProcessos();
void rodaProcessos();
void removeProcesso(int id);

void adicionaEscalonador(int job, char* arq, int offset, int times);
void checaEscalonador();
void executaEscalonador();

#endif
