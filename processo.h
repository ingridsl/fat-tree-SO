#ifndef PROCESSO_H
#define PROCESSO_H

#include <sys/types.h>
#include <string>

namespace Pstatus {
	enum tipo { RODANDO, AGENDADO, DORMINDO, MORTO };
}

class Processo {
public:
	Processo(int id, int tempo, std::string a);
	~Processo();
	pid_t Rodar();
	void SetStatus(Pstatus::tipo s);
	int GetId();
	std::string GetArq();
	int GetRuntime();
private:
	Pstatus::tipo status;
	std::string arq;
	int id;
	int runtime;
};

#endif
