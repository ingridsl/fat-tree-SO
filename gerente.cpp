#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <sys/msg.h>
#include <sys/wait.h>
#include <signal.h>

#include <unistd.h>
#include <time.h>

int nivel;
int pid_filho_1, pid_filho_2;

void shutdown(int sig){
	int status1, status2;

	if(nivel != 4){
		kill(pid_filho_1, SIGUSR1);
		kill(pid_filho_2, SIGUSR1);
		wait(&status1);
		wait(&status2);
	}

	exit(0); //exit(1);
}

int main (int argc, char *argv[]){
	signal(SIGUSR1, shutdown);

	nivel = atoi(argv[1]);
	if(argc > 2){
		pid_filho_1 = atoi(argv[2]);
		pid_filho_2 = atoi(argv[3]);
	}

	while(1){

	}
	return 0;
}