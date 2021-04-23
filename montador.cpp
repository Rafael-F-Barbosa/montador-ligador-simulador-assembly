#include <iostream>
#include <map>

#include "./sources/util.h"
#include "./sources/pre_processamento.h"
#include "./sources/primeira_passagem.h"
#include "./sources/segunda_passagem.h"

using namespace std;


int main( int argc, char *argv[ ] )
{
    int arguments_amount = argc;

    if(arguments_amount == 1){
        cout << "É necessário passar o nome do arquivo a ser montado." << endl;
    }else if(arguments_amount == 2){
        // Extrai os argumentos importantes
        string assembler_name = argv[0];
        string compiled_program = argv[1];
        
        // Obtem o programa pré-processado
        Program *p = pre_processing(compiled_program);
        

        // Salva código pre processado no em um arquivo de texto
        // ofstream object_result0;
        // object_result0.open("pre.txt");
        // for(auto it:p->lines){
        //     object_result0 << it->text << endl;
        // }
        // object_result0.close();



        // Obtem a tabela de simbolos do programa
        vector<Symbol *> symbol_table = first_pass(p); 
        // Passa a tabela de símbolos e o programa como argumentos;
        vector<string> object_code = second_pass(p, symbol_table);

        // Muda terminação
        string object_name = split_string(argv[1], '.')[0];
        

        // Caso o programa tenha não tenha gerado erros salva ele em um arquivo .obj
        if(p->program_errors.size() > 0){
            print_program_errors(p->program_errors);
        }else{
            cout << "A compilação terminou sem erros. O código objeto foi gerado." << endl;
            // Salva código objeto no em um arquivo de texto
            ofstream object_result;
            object_result.open(object_name+".obj");
            for(auto it:object_code){
                object_result << it << " ";
            }
            object_result.close();
        }
        
        
    }
    else{
        cout << "Foram passados mais argumentos que o necessário." << endl;
    }



    return 0;
}