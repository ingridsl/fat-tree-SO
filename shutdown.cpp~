#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <signal.h>

int main(){
	key_t msgkey = 0x14002713;
	int msgqid;
	if((msgqid = msgget(msgkey, 0x1B6)) < 0){
		perror("Erro ao executar msgget: ");
		exit(1);
	}

	// envia mensagem do tipo 1, que ativa flag no escalonador
	struct mensagem {
		long tipo;
		char dummy;
	};
	struct mensagem msg = { 1, ' ' };
	int msg_tam = sizeof(struct mensagem) - sizeof(long);

    if(msgsnd(msgqid, &msg, msg_tam, 2) < 0){
    	perror("Erro ao executar msgsnd: ");
    }

    // Podemos ver para implementar com kill
    //kill(msgqid, SIGTERM);
	
	return 0; //exit(0);
}