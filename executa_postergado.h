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
#endif
