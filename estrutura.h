#ifndef ESTRUTURA_H
#define ESTRUTURA_H

#define N 200

struct mensagem {
	long tipo; // tipo da mensagem, para o escalonador ser usado = 4
	int job; // ?
	char arq[N]; // nome do arquivo
	int offset; // segundos da postergacao
};

struct job {
	long tipo;
	int job;
};

#endif
