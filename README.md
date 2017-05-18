# fat-tree-SO
Execução Multi-Processo Postergada Utilizando FAT TREE (Trabalho SO)

escalonador:

	g++ escalonador.cpp processo.cpp log.cpp -o escalonador

	./escalonador&

	- Irá mostrar o id e o pid
	- Para ver todos abertos: ps -l
	- Para remover algum: kill [pid]
executa postergado:

	g++ executa_postergado.cpp -o executa_postergado

	./executa_postergado 5 hello_world

	- Depois de escalonador ter sido executado com sucesso
	
shutdown:
	g++ shutdown.cpp -o shutdown
	./shutdown
