#include <iostream>
#include <map>

#include "./sources/util.h"
#include "./sources/pre_processamento.h"
#include "./sources/primeira_passagem.h"
#include "./sources/segunda_passagem.h"
#include "./sources/module.h"

using namespace std;


int main( int argc, char *argv[ ] )
{
    int arguments_amount = argc;
    // Cria programa geral
    Program *program = new Program({});

    if(arguments_amount == 1){
        cout << "É necessário passar o nome do arquivo a ser montado." << endl;
    }else if(arguments_amount == 2){
        // Extrai o nome do módulo
        string compiled_program = argv[1];

        // Obtem o programa pré-processado
        Module *_module = pre_processing(compiled_program);

        // Obtem a tabela de simbolos do programa
        _module->symbols_table = first_pass(_module); 

        // Passa a tabela de símbolos e o programa como argumentos;
        _module->object_code = second_pass(_module);

        // Salva tamanho do módulo
        _module->module_len = _module->object_code.size();

        for(int i = 0; i < _module->object_code.size(); i++){
            int last_index = _module->object_code[i].size()- 1;
            if(_module->object_code[i][last_index] == 'a'){
                _module->bits_map.push_back(0);
            }else if(_module->object_code[i][last_index] == 'r'){
                _module->bits_map.push_back(1);
            }
            // Apaga absoluto e relativo do código objeto
            _module->object_code[i].pop_back();
        }

        // Salva o módulo no programa
        program->modules.push_back(_module);
    
    }
    else if(arguments_amount == 3 || arguments_amount == 4){
        for(int i = 1; i < arguments_amount; i++){

            cout << "\nMODULO:" << argv[i] << endl;
            
            Module *m = pre_processing(argv[i]);
            
            //  Informação de que esse módulo não é o único.
            m->is_this_module_the_only = false;

            m->symbols_table = first_pass(m);            
            
            m->object_code = second_pass(m);

            m->module_len = m->object_code.size();

            for(int i = 0; i < m->object_code.size(); i++){
                int last_index = m->object_code[i].size()- 1;
                if(m->object_code[i][last_index] == 'a'){
                    m->bits_map.push_back(0);
                }else if(m->object_code[i][last_index] == 'r'){
                    m->bits_map.push_back(1);
                }
                // Apaga absoluto e relativo do código objeto
                m->object_code[i].pop_back();
            }

            // Salva o módulo no programa
            program->modules.push_back(m);

            // Imprime tabela de símbolos
            red_cout("TABELA DE SIMBOLOS");
            for(auto it:m->symbols_table)
                cout << it->label << ": " << it->position << "-- ext: "<< it->is_extern << endl;

            // Imprime tabela de símbolos
            blue_cout("TABELA DE DEFINIÇÕES");
            for(auto it:m->definitions_table)
                cout << it->label << ": " << it->value << endl;

            green_cout("TABELA DE USO");
            for(auto it:m->uses_table)
                cout << it->label << ": " << it->value << endl;
        }

    }
    else{
        cout << "Foram passados mais argumentos que o necessário." << endl;
    }


    // Imprime erros e salva código montado
    ofstream object_file;
    for(auto it: program->modules){
        // Imprime erros no programa
        if(it->program_errors.size() != 0){
            cout << "Erro(s) no módulo: " << it->module_name << endl;
            print_program_errors(it->program_errors);
        }

        // Abre arquivo de texto
        object_file.open(it->module_name+".o");
        
        // Salva nome e tamanho do arquivo
        object_file << "N," << it->module_name << endl;
        object_file << "L," << it->module_len << endl;
        
        // Salva mapa de bits - A FAZER
        // Salva tabela de definições
        object_file << "R,";
        for(auto bit: it->bits_map){
            object_file << bit << " ";
        }
        object_file << endl;

        // Salva tabela de definições
        for(auto def: it->definitions_table){
            object_file << "D," << def->label << " "<< def->value << endl;
        }

        // Salva tabela de uso
        for(auto use: it->uses_table){
            object_file << "U," << use->label << " "<< use->value << endl;
        }

        // Salva arquivo objeto
        object_file << "T,";
        for(auto ob_code: it->object_code){
            object_file << ob_code << " ";
        }
        object_file << endl;

        object_file.close();
    }



    return 0;
}