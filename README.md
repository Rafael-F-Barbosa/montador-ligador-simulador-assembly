# Montador-Ligador-Simulador assembly

Esse trabalho é um montador, ligador e simulador de um código assembly inventado realizado para a disciplina de Software Básico-UnB no segundo semestre de 2020.

## Ambiente de Desenvolvimento
O código foi desenvolvido em c++ e compilado com g++ versão 7.5.0(Ubuntu 7.5.0-3ubuntu1~18.04) em um Linux Mint 19.3 Tricia, mas deve ser possível utilizá-lo com qualquer compilador com suporte o padrão c++11. O código foi testado no linux bem como os exemplos desenolvidos para este também, no entanto, foram desenvolvidas estratégias para evitar problemas com códigos desenvolvidos no windows(Suporte a CRLF e LF).

## Linguagem assembly inventada

O montador suporta o seguinte conjunto de instruções que é utilizado na disciplina:

![Tabela assembly inventado](/assets/assembly_inventado.jpg)


Além dessas, para o funcionamento de módulos são incluídas as diretivas EXTERN(declarar variáveis de declaradas externamente), PUBLIC(tornar variáveis acessíveis externamente) e BEGIN/END que demarcam início e fim de um módulo.

## Funcionamento

O montador é responsável por gerar um ou mais programas a partir de um código de texto (formato .asm). Gerando um arquivo .o com cabeçalho, seção de dados, de texto e informação de realocação. Usa-se os arquivos .o com entrada para o ligador, este é responsável por juntar os programa e gerar um único programa .obj que é lido pelo simulador. Este recebe um programa objeto e simula execução do programa fazendo a contagem e salvando possíveis outputs em um arquivo .out.

![Exemplo de funcionamento](/assets/1.gif)


## Compilação

O código pode ser compilado utilizando o makefile disponível no diretório principal da aplicação. Assim basta digitar make que o simuador, ligador e o montador serão compilados e ligados. 

```
make
```

Caso o usuário não tenha suporte a make o **montador** pode ser compilado com o comando:

```
g++ -std=c++11 -c ./sources/pre_processamento.cpp ./sources/primeira_passagem.cpp ./sources/segunda_passagem.cpp ./sources/util.cpp
```

Em seguida a compilação e ligação é feita com:

```
g++ -std=c++11 montador.cpp  pre_processamento.o primeira_passagem.o segunda_passagem.o util.o -o montador
```

E o **ligador** com os comandos:
```
g++ -std=c++11 -c ./sources/module.cpp ./sources/util.cpp
```

Em seguida a compilação e ligação é feita com:
```
g++ -std=c++11 ligador.cpp  module.o util.o -o ligador
```

Por fim, o **simulador** com os comandos:
```
g++ -std=c++11 -c ./sources/fonte_simulador.cpp ./sources/util.cpp
```
Em seguida a compilação e ligação é feita com:
```
g++ -std=c++11 simulador.cpp  fonte_simulador.o util.o -o simulador
```
## Execução
Para executar o montador é necessário passar um programa de texto com a linguagem de assembly inventada. Exemplo:
```
./montador exemplo.asm
```
Para executar o ligador é necessário passar um arquivo de texto com o código objeto gerado pelo montador. Exemplo:
```
./ligador moduloA.o moduloB.o
```
Para executar o simulador é necessário passar um programa texto com um código objeto gerado pelo ligador. Exemplo:


```
./simulador exemplo.obj
```


