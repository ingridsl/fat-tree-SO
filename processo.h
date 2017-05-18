#ifndef PROCESSO_H
#define PROCESSO_H

#include <string>

namespace Pstatus {
	enum tipo { RODANDO, AGENDADO, DORMINDO, MORTO };
}

class Processo {
public:
	Processo(int id, int tempo, std::string a);
	~Processo();
	void Rodar();
	void SetStatus(Pstatus::tipo s);
	int GetId();
	bool ConfirmaId(int id);
	std::string GetArq();
	int GetRuntime();
private:
	Pstatus::tipo status;
	std::string arq;
	int id;
	int runtime;
};

#endif
