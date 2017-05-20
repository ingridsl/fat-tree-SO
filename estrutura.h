#ifndef ESTRUTURA_H
#define ESTRUTURA_H

#define N 200

struct mensagem {
	long tipo; // tipo da mensagem, para o escalonador ser usado = 49
	int job; // ?
	char arq[N]; // nome do arquivo
	int delay; // segundos da postergacao
};

struct job {
	long tipo;
	int job;
};

struct exec{
	long tipo;
	int job;
	char arq[N];
	int tempo;
};

#endif
