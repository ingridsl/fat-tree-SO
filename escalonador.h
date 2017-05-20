#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "estrutura.h"

#include <sys/types.h>
#include <vector>
#include <queue>

int job_id, msgqid_recebimento, msgqid_envio;// 15 ou 16
int pid_filho;

std::vector<struct exec> execucoes_pendentes;
std::queue<struct exec> proc_running;

int obterHorarioAtual();

void shutdown(int sig);

void adicionaExecucaoPostergada(struct mensagem recebido);
void checaEscalonador();
void executaEscalonador();
void criarGerentes();

#endif
