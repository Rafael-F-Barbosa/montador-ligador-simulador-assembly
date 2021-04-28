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

    if(arguments_amount == 1){
        cout << "É necessário passar o nome do arquivo a ser montado." << endl;
    }else if(arguments_amount == 2){
        // Extrai os argumentos importantes
        string assembler_name = argv[0];
        string compiled_program = argv[1];
        
        // Obtem o programa pré-processado
        Module *p = pre_processing(compiled_program);
        

        // Salva código pre processado no em um arquivo de texto
        // ofstream object_result0;
        // object_result0.open("pre.txt");
        // for(auto it:p->lines){
        //     object_result0 << it->text << endl;
        // }
        // object_result0.close();



        // Obtem a tabela de simbolos do programa
        p->symbols_table = first_pass(p); 

        // Passa a tabela de símbolos e o programa como argumentos;
        vector<string> object_code = second_pass(p);

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
    else if(arguments_amount == 3 || arguments_amount == 4){

        cout << "Tem módulos." << endl;

        Program *program = new Program({});
        
        for(int i = 1; i < arguments_amount; i++){

            cout << "\nMODULO:" << argv[i] << endl;
            
            Module *m = pre_processing(argv[i]);
            
            //  Informação de que esse módulo não é o único.
            m->is_this_module_the_only = false;

            m->symbols_table = first_pass(m);

            // Imprime tabela de símbolos
            red_cout("TABELA DE SIMBOLOS");
            for(auto it:m->symbols_table)
                cout << it->label << ": " << it->position << "-- ext: "<< it->is_extern << endl;
            
            vector<string> object_code = second_pass(m);

            // Imprime tabela de símbolos
            blue_cout("TABELA DE DEFINIÇÕES");
            for(auto it:m->definitions_table)
                cout << it->label << ": " << it->value << endl;

            green_cout("TABELA DE USO");
            for(auto it:m->uses_table)
                cout << it->label << ": " << it->value << endl;

            // Imprime erros no programa
            print_program_errors(m->program_errors);

            cout << endl;
            program->modules.push_back(m);
        }


    }
    else{
        cout << "Foram passados mais argumentos que o necessário." << endl;
    }



    return 0;
}