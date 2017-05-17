#ifndef PROCESSO_H
#define PROCESSO_H

#include <time.h>
#include <string>

namespace Pstatus {
	enum tipo { RODANDO, AGENDADO, DORMINDO, MORTO };
}

class Processo {
public:
	Processo(int id, time_t tempo, std::string a);
	std::string Rodar();
	void SetStatus(Pstatus::tipo s);
	int GetId();
	bool ConfirmaId(int id);
	std::string GetArq();
	time_t GetRuntime();
private:
	Pstatus::tipo status;
	std::string arq;
	int id;
	time_t runtime;
};

#endif