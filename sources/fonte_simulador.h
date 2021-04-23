#include <iostream>
#include <string>
#include <vector>


#include "util.h"


using namespace std;


void format_cout_instruction(int program_counter, int acc, bool is_output=false, int output=0);

class ProgramSimulation{
public:
    int PC = 0;
    int ACC = 0;
    vector<int> program = {};
    vector<int> outputs = {};

    ProgramSimulation(int p_c, int acumulator, string s){
        PC = p_c;
        ACC = acumulator;
        program = split_string_integer(s, ' ');
    }

    void add(int len){
        // O valor somado é o próximo elemento do PC
        int operando = program[PC+1];
        ACC += program[operando];

        // Atualiza o program counter
        PC += len;

        // Imprime instrução na tela
        format_cout_instruction(PC, ACC);

    }
    void sub(int len){

        // O valor subtraído é o próximo elemento do PC
        int operando = program[PC+1];
        ACC -= program[operando];
        
        // Atualiza o program counter
        PC += len;

        // Imprime instrução na tela
        format_cout_instruction(PC, ACC);
    }
    void mul(int len){
        // O valor multiplicado é próximo elemento do PC
        int operando = program[PC+1];
        ACC *= program[operando];
        
        // Atualiza o program counter
        PC += len;

        // Imprime instrução na tela
        format_cout_instruction(PC, ACC);
    }
    void div(int len){
        // O valor dvidido é próximo elemento do PC
        int operando = program[PC+1];
        ACC = int(ACC/program[operando]);
        
        // Atualiza o program counter
        PC += len;

        // Imprime instrução na tela
        format_cout_instruction(PC, ACC);
    }



    void jmp(int len){
        // Pula pro valor do argumento
        PC = program[PC+1];

        // Imprime instrução na tela
        format_cout_instruction(PC, ACC);
    }
    void jmpn(int len){
        // Verifica condicional, se sim pula
        if(ACC < 0){
            PC = program[PC+1];
        }else{
            PC += len;
        }

        // Imprime instrução na tela
        format_cout_instruction(PC, ACC);
    }
    void jmpp(int len){
        // Verifica condicional, se sim pula
        if(ACC > 0){
            PC = program[PC+1];
        }else{
            PC += len;
        }

        // Imprime instrução na tela
        format_cout_instruction(PC, ACC);
    }
    void jmpz(int len){
        // Verifica condicional, se sim pula
        if(ACC == 0){
            PC = program[PC+1];
        }else{
            PC += len;
        }

        // Imprime instrução na tela
        format_cout_instruction(PC, ACC);
    }




    void copy(int len){
        // Copia o endereço do primeiro operando pro segundo
        int first_argument = program[PC+1];
        int second_argument = program[PC+2];
        program[second_argument] = program[first_argument];

        // Atualiza o program counter
        PC += len;

        // Imprime instrução na tela
        format_cout_instruction(PC, ACC);
    }

    void load(int len){
        // Carrega um endereço da memória
        int argument = program[PC+1];
        ACC = program[argument];

        // Atualiza o program counter
        PC += len;

        // Imprime instrução na tela
        format_cout_instruction(PC, ACC);
    }

    void store(int len){
        // Carrega um endereço da memória
        int operando = program[PC+1];
        program[operando] = ACC;

        // Atualiza o program counter
        PC += len;

        // Imprime instrução na tela
        format_cout_instruction(PC, ACC);
    }

    void input(int len){
        // Recebe um entrada da entrada padrão
        int operando = program[PC+1];
        cout << "INPUT: ";
        cin >> program[operando];

        // Atualiza o program counter
        PC += len;

        // Imprime instrução na tela
        format_cout_instruction(PC, ACC);
    }   

    void output(int len){
        // Obtem o operando
        int operando = program[PC+1];

        // Salva no vetor de outputs
        outputs.push_back(program[operando]);

        // Atualiza o program counter
        PC += len;
        
        // Imprime instrução na tela
        format_cout_instruction(PC, ACC, true, program[operando]);
    }
    void stop(int len){
        // Soma 1 ao contador de programa
        PC += len;
        
        // Imprime instrução na tela
        format_cout_instruction(PC, ACC);
    }   

};
