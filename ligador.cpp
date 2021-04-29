#include <iostream>
#include <map>

#include "./sources/util.h"
#include "./sources/module.h"

using namespace std;


int main( int argc, char *argv[ ] )
{
    int arguments_amount = argc;

    if(arguments_amount == 1){
        cout << "É necessário passar o nome do arquivo a ser ligado." << endl;
    }
    // Caso apenas um programa precise ser ligado
    else if(arguments_amount == 2){
        // Extrai o nome do módulo
        string file_name = argv[1];

        // Cria um objeto que lê arquivos.
        ifstream input_file;
        // Abre o arquivo.
        input_file.open(file_name);

        if (input_file.is_open())
        {
            // Cria objeto para escrever em arquivo
            ofstream output_file;
            // Abre arquivo de texto
            output_file.open(split_string(file_name, '.')[0]+".obj");

            // Declara string para armazenar cada linha do ponteiro.
            string line;

            // Percorre arquivo a ser ligado, encontra seção de texto e escreve ela em um arquivo.
            while (getline(input_file, line))
            {
                if(line[0] == 'T'){
                    output_file << split_string(line, ',')[1] << endl;
                }
            }
            input_file.close();
            output_file.close();
        }
        else
        {
            cout << "Não foi possível abrir o arquivo" << endl;
        }

        
    
    }
    // Caso mais de um programa precise ser ligado
    else if(arguments_amount == 3 || arguments_amount == 4){
        Program *p = new Program({});
        for(int i = 1; i < arguments_amount; i++){
            // Extrai o nome do módulo
            string file_name = argv[i];

            cout << "parsing module: " << file_name << endl; 

            // Cria um objeto que lê arquivos.
            ifstream input_file;
            // Abre o arquivo.
            input_file.open(file_name);

            if (input_file.is_open())
            {
                // Declara string para armazenar cada linha do ponteiro.
                string line;

                Module *m = new Module({}, 0, 0, file_name);
                // Percorre arquivo a ser ligado, encontra seção de texto e escreve ela em um arquivo.
                while (getline(input_file, line))
                {
                    // Obtém nome do módulo
                    if(line[0] == 'N'){
                        m->module_name = split_string(line,',')[1];
                        cout << m->module_name << endl << endl;
                    }
                    
                    // Obtém tamanho do módulo
                    if(line[0] == 'L'){
                        m->module_len = stoi(split_string(line,',')[1]);
                        cout << m->module_len << endl;
                    }
                    // Obtém mapa de bits do módulo
                    else if(line[0] == 'R'){
                        string aux = split_string(line,',')[1];
                        m->bits_map = split_string_integer(aux, ' ');
                    }
                    // Obtém tabela de definições
                    else if(line[0] == 'D'){
                        string aux = split_string(line,',')[1];
                        string label = split_string(aux, ' ')[0];
                        int pc = stoi(split_string(aux, ' ')[1]);
                        Definition *def = new Definition(pc, label);
                        m->definitions_table.push_back(def);
                    }
                    // Obtém tabela de uso
                    else if(line[0] == 'U'){
                        string aux = split_string(line,',')[1];
                        string label = split_string(aux, ' ')[0];
                        int pc = stoi(split_string(aux, ' ')[1]);
                        Use *new_use = new Use(pc, label);
                        m->uses_table.push_back(new_use);
                    }
                    // Obtém código objeto do módulo
                    else if(line[0] == 'T'){
                        string aux = split_string(line,',')[1];
                        m->object_code = split_string(aux, ' ');
                    }

                }

                p->modules.push_back(m);

                blue_cout("TABELA DE DEFINIÇÕES");
                for(auto it:m->definitions_table)
                    cout << it->label << ": " << it->value << endl;

                green_cout("TABELA DE USO");
                for(auto it:m->uses_table)
                    cout << it->label << ": " << it->value << endl;
                
                red_cout("CÓDIGO OBJETO");
                for(auto it:m->object_code)
                    cout << it << " ";
                cout << endl;

                cout << "MAPA DE BITS" << endl;
                for(auto it:m->bits_map)
                    cout << it << " ";
                cout << endl;

                input_file.close();
            }
            else
            {
                cout << "Não foi possível abrir o arquivo" << endl;
            }
        }


        // Define o fator de correção de cada módulo
        for(int i=0; i < p->modules.size() - 1; i++){
            int f_c = p->correction_factor[p->correction_factor.size()-1] + p->modules[i]->module_len;
            p->correction_factor.push_back(f_c);
        }
        cout << "Fatores de correção : " << endl;
        cout << p->correction_factor[0] << " " << p->correction_factor[1] << endl;

        // Cria tabela de definições global
        int i = 0;
        for(auto module: p->modules){
            for(auto def: module->definitions_table){
                int corrected_pc = p->correction_factor[i] + def->value;
                p->global_definitions_table.push_back(new Definition(corrected_pc,def->label));
            }
            i++;
        }
        blue_cout("TABELA DE DEFINIÇÕES GLOBAL");
        for(auto it:p->global_definitions_table){
            cout << it->label << " " << it->value << endl;
        }

        red_cout("Endereços antes");
        for(auto module:p->modules){
            for(auto obj: module->object_code){
                cout << obj << " ";
            }
            cout << endl;
        } 

        // Corrige endereços relativos
        for(int i = 0; i < p->modules.size(); i++){
            for(int j = 0; j < p->modules[i]->object_code.size(); j++){
                if(p->modules[i]->bits_map[j] == 1){
                    int corrected_adress = stoi(p->modules[i]->object_code[j]) + p->correction_factor[i];
                    p->modules[i]->object_code[j] = to_string(corrected_adress);
                }
            }
        } 
        red_cout("Endereços depois");
        for(auto module:p->modules){
            for(auto obj: module->object_code){
                cout << obj << " ";
            }
            cout << endl;
        }

        // Corrige referências cruzadas
        for(int i = 0; i < p->modules.size(); i++){
            for(int j = 0; j < p->modules[i]->uses_table.size(); j++){
                Use *use_case = p->modules[i]->uses_table[j];
                Definition *def = p->get_definition_by_name(use_case->label);
                if(def != NULL ){
                    p->modules[i]->object_code[use_case->value] = to_string(def->value);
                }else{
                    p->modules[i]->program_errors.push_back(new ErrorMessage(0,p->modules[i]->uses_table[j]->label,"Tu tens que definir todos os símbolos, irmão!"));
                }
            }
            print_program_errors(p->modules[i]->program_errors);
        }

        green_cout("Endereços Finais");

        for(auto module:p->modules){
            for(auto obj: module->object_code){
                cout << obj << " ";
            }
            cout << endl;
        }

        vector<string> final_object_code = {};
        // Alinhando os códigos
        for(int i = 0; i < p->modules.size(); i++){
            for(auto code:p->modules[i]->object_code){
                final_object_code.push_back(code);
            }
        }


        // Salvando código em arquivo
        ofstream output_file;
        output_file.open(split_string(p->modules[0]->module_name, '.')[0]+".obj");
        for(auto mod:p->modules){

        }
            for(auto code:final_object_code){
                output_file << code << " ";
            }
        


        


    }
    else{
        cout << "Foram passados mais argumentos que o necessário." << endl;
    }

    return 0;
}