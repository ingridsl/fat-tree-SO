fat_tree: escalonador executa_postergado gerente shutdown


escalonador: escalonador.cpp processo.cpp log.cpp
	g++ escalonador.cpp processo.cpp log.cpp -o escalonador

executa_postergado: executa_postergado.cpp
	g++ executa_postergado.cpp -o executa_postergado

gerente: gerente.cpp
	g++ gerente.cpp -o gerente

shutdown: shutdown.cpp
	g++ shutdown.cpp -o shutdown

clean: 
	rm -f escalonador executa_postergado gerente shutdown *.o