#ifndef GERENTE_H
#define GERENTE_H

#include "estrutura.h"
#include "executa_postergado.h"

#define OCUPADO 1
#define LIVRE 0

/**/
int identificador;
/**/
int pid_filho_1, pid_filho_2;
/**/
int msgqid_up, msgqid_down;
/**/
int estado;

/**/
const char* obterNomeArq(char *arq);

/**/
int obterHorarioAtual();

/**/
void shutdown(int sig);

/**/
void trabalha();

#endif