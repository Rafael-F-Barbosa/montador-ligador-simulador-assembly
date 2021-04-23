#include <iostream>

#include "./sources/util.h"
#include "./sources/fonte_simulador.h"

using namespace std;

#define ADD 1
#define SUB 2
#define MUL 3
#define DIV 4
#define JMP 5
#define JMPN 6
#define JMPP 7
#define JMPZ 8
#define COPY 9
#define LOAD 10
#define STORE 11
#define INPUT 12
#define OUTPUT 13
#define STOP 14


int main( int argc, char *argv[ ] )
{
    int qtd_argumentos = argc;

    if(qtd_argumentos == 1){
        cout << "É necessário passar o nome do arquivo a ser montado." << endl;
    }else if(qtd_argumentos == 2){

        string assembler_name = argv[0];
        string program_name = argv[1];

        // Cria um objeto que lê arquivos.
        ifstream input_file;

        // Declara vetor para armazenar todas as linhas
        vector <string> lines = {};

        // Declara string para armazenar linhas
        string line;

        // Abre o arquivo.
        input_file.open(program_name);

        if (input_file.is_open())
        {
            while (getline(input_file, line))
            {
                lines.push_back(line);
            }
            input_file.close();
        }
        else
        {
            cout << "Não foi possível abrir o arquivo" << endl;
        }

        // Validação da formatação do arquivo objeto.
        if(lines.size() != 1){
            cout << "O arquivo objeto deve ter apenas uma linha" << endl;
        }else{
            // As tabelas de instrução e de diretivas são padrões
            vector<Instruction *> instruction_table = create_instruction_table();

            ProgramSimulation *simulation = new ProgramSimulation(0, 0, lines[0]); 

            // Percorre programa
            while(simulation->program[simulation->PC] != STOP){

                // Obtem opcode observado
                int opcode = simulation->program[simulation->PC];

                // Procura instrução
                Instruction *inst = get_instruction_by_opcode(instruction_table, opcode);

                // Se a instrução for encontrada, ela é executada.
                if(inst != NULL){
                    // blue_cout(inst->name);
                    
                    // Realiza uma ação para cada opcode
                    switch(inst->opcode){
                        case ADD: simulation->add(inst->length);
                        break;
                        case SUB: simulation->sub(inst->length);
                        break;
                        case MUL: simulation->mul(inst->length);
                        break;
                        case DIV: simulation->div(inst->length);
                        break;
                        case JMP: simulation->jmp(inst->length);
                        break;
                        case JMPN: simulation->jmpn(inst->length);
                        break;
                        case JMPP: simulation->jmpp(inst->length);
                        break;
                        case JMPZ: simulation->jmpz(inst->length);
                        break;
                        case COPY: simulation->copy(inst->length);
                        break;
                        case LOAD: simulation->load(inst->length);
                        break;
                        case STORE: simulation->store(inst->length);
                        break;
                        case INPUT: simulation->input(inst->length);
                        break;
                        case OUTPUT: simulation->output(inst->length);
                        break;
                    }
                }else{
                    cout << "Essa Instrução não existe!!" << endl;
                    break;
                }
            }
            // Adiciona a impressão do STOP
            simulation->stop(1);

            // Muda terminação
            string output_name = split_string(argv[1], '.')[0];

            // Salva saída em arquivo
            ofstream outputs_file;
            outputs_file.open(output_name+".out");
            for(auto it:simulation->outputs){
                outputs_file << it << endl;
            }
            outputs_file.close();

        }
    }
    else{
        cout << "Foram passados mais argumentos que o necessário." << endl;
    }

    // Salva os outputs em um arquivo


    return 0;
}