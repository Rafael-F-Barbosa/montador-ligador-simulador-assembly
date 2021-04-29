all: montador simulador ligador

# Principal 
montador: montador.o pre_processamento.o primeira_passagem.o segunda_passagem.o util.o module.o
		@ g++ montador.o pre_processamento.o primeira_passagem.o segunda_passagem.o util.o module.o -o montador -std=c++11

simulador: simulador.o fonte_simulador.o util.o
		@ g++ simulador.o util.o fonte_simulador.o -o simulador -std=c++11

ligador: ligador.o util.o module.o
		@ g++ ligador.o util.o module.o -o ligador -std=c++11

# Segundo principal
montador.o: montador.cpp
		@ g++ -c montador.cpp -std=c++11

simulador.o: simulador.cpp
		@ g++ -c simulador.cpp -std=c++11

lgiador.o: ligador.cpp
		@ g++ -c ligador.cpp -std=c++11

# Auxiliares
pre_processamento.o: ./sources/pre_processamento.cpp ./sources/pre_processamento.h ./sources/util.h ./sources/module.h
		@ g++ -c ./sources/pre_processamento.cpp -std=c++11

primeira_passagem.o: ./sources/primeira_passagem.cpp ./sources/primeira_passagem.h ./sources/util.h ./sources/module.h
		@ g++ -c ./sources/primeira_passagem.cpp -std=c++11

segunda_passagem.o: ./sources/segunda_passagem.cpp ./sources/segunda_passagem.h ./sources/util.h ./sources/module.h
		@ g++ -c ./sources/segunda_passagem.cpp -std=c++11

fonte_simulador.o: ./sources/fonte_simulador.cpp ./sources/fonte_simulador.h ./sources/util.h 
		@ g++ -c ./sources/fonte_simulador.cpp -std=c++11

util.o: ./sources/util.cpp ./sources/util.h 
		@ g++ -c ./sources/util.cpp -std=c++11

module.o: ./sources/module.h ./sources/util.cpp ./sources/util.h
		@ g++ -c ./sources/module.cpp -std=c++11
clean:
		rm *.o *.obj *.out *.txt montador simulador ligador
