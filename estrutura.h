#ifndef ESTRUTURA_H
#define ESTRUTURA_H

#define N 200
#define RANGE_REQUISICOES -50
#define ADICIONA_JOB 49
#define SHUTDOWN 50
#define CONFIRMACAO_EXECUCAO 51
#define N_NOS 15

#define ID_1 1
#define ID_2 2
#define ID_3 3
#define ID_4 4
#define ID_5 5
#define ID_6 6
#define ID_7 7
#define ID_8 8
#define ID_9 9
#define ID_10 10
#define ID_11 11
#define ID_12 12
#define ID_13 13
#define ID_14 14
#define ID_15 15

/**/
struct mensagem {
	long tipo; // tipo da mensagem, para o escalonador ser usado = 49
	int job; // ?
	char arq[N]; // nome do arquivo
	int delay; // segundos da postergacao
};

/**/
struct job {
	long tipo;
	int job;
};

/**/
struct exec{
	long tipo;
	int job;
	char arq[N];
	int tempo; //delay
	int tempo_solicitacao;
	int tempo_inicio;
	int tempo_termino;
};

#endif
