#ifndef EXECUTA_POSTERGADO_H
#define EXECUTA_POSTERGADO_H

//#include "processo.h"
#include "estrutura.h"

#include <sys/types.h>
#include <vector>
#include <queue>

int job_id, msgqid, msgqidjob;
void checaQuantidade(int quantidade);
int numOk(const char * seg);
int arquivoOK(char * arqExe);
int verificaFila(key_t msgkey);
int encontraUltimoJob();
void executaPostergado(int seg, char arq_executavel[N]);

void executaPostergado();
//void shutdown(int sig);

//void listaProcessos();
//void rodaProcessos();
//void removeProcesso(int id);

//void adicionaEscalonador(char* arq, int offset, int times);
//void checaEscalonador();
//void executaEscalonador();

#endif
