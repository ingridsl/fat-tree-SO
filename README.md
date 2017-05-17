# fat-tree-SO
Execução Multi-Processo Postergada Utilizando FAT TREE (Trabalho SO)

escalonador:
	g++ escalonador.cpp processo.cpp log.cpp -o escalonador
	./escalonador&

	- Irá mostrar o id e o pid
	- Para ver todos abertos: ps -l
	- Para remover algum: kill [pid]
	
shutdown:
	g++ shutdown.cpp -o shutdown
	./shutdown