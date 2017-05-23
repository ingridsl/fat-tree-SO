#ifndef GERENTE_H
#define GERENTE_H

#include "estrutura.h"
#include "executa_postergado.h"

#define OCUPADO 1
#define LIVRE 0

/* variavel global usado para calcular o identificador do pai ou do filho*/
int identificador;

/* variaveis globais para armazenar o pid dos filhos criados para cada nó de forma a montar a estrutura da tree*/
int pid_filho_1, pid_filho_2;

/* variaveis globais que servem como ID das filas de mensagens que vão do sentindo down->up e up->down respectivamente */
int msgqid_up, msgqid_down;

/* variável global que define o estado do gerente para controle de acesso*/
int estado;

/*cria estruturas de dados do tipo struct de formas globais. as estruturas das mesmas estão definidas em estrutura.h*/
struct exec msg, msgfilho;

/** 
* obtem horário atual em segundos
*/
int obterHorarioAtual();

/** 
* função de shutdown para os nós da arvore com recepção de sinais de término
*/
void shutdown(int sig);

/** 
* funcao principal do gerente.cpp que define a lógica de funcionamento da fat tree quanto ao envio de mensagens e seus tipos
*/
void trabalha();

#endif
