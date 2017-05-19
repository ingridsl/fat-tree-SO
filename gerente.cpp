#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <sys/msg.h>
#include <sys/wait.h>

#include <unistd.h>
#include <time.h>

int main (int argc, char *argv[]){
	int nivel;
	nivel = atoi(argv[1]);
	if(argc > 2)
	printf("%s, Nivel %d Pid dos filhos: %s %s\n", argv[0],nivel, argv[2],argv[3]);

	while(1){

	}
	return 0;
}