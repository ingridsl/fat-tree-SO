# fat-tree-SO
Execução Multi-Processo Postergada Utilizando FAT TREE (Trabalho SO)

Compilação
----------
Basta navegar até o diretório da **fat-tree-so** no terminal e executar o comando:
```
make
```
Isso irá gerar os executáveis automaticamente

Também pode ser feita de forma manual, basta navegar até o diretório da **fat-tree-so** no terminal e executar os comandos:
```
g++ escalonador.cpp processo.cpp log.cpp -o escalonador
g++ executa_postergado.cpp -o executa_postergado
g++ gerente.cpp -o gerente
g++ shutdown.cpp -o shutdown
```

Execução
--------
**1. escalonador:**
```
./escalonador&
```
  - Irá mostrar o id e o pid
  - Para ver todos abertos: ```ps -l```
  - Para remover algum: ```kill <pid>```

**2. executa postergado:**
```
./executa_postergado <seg> <arq_executável>
```
  - Executar após o escalonador ter sido executado com sucesso
  - Irá enviar o job ao escalonador com os dados passados pelo usuário

**3. shutdown:**
```
./shutdown
```
