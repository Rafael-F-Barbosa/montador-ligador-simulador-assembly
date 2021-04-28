#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "util.h"
#include "module.h"

using namespace std;


// Realiza o processamento de um token do tipo label
void label_processing(string label, int program_counter, int line_number, string text, vector<Symbol*> *symbol_table, vector<Instruction*> instruction_table,
vector<Directive* > directive_table, Module *_module){
    
    // Elimina espaço no final do label, se houver
    label = split_string(label, ' ')[0];

    // Se o label for redefinido, adiciona um erro
    if(get_symbol_by_name(*symbol_table, label)){
        _module->program_errors.push_back(new ErrorMessage(line_number, text, "Erro semântico: símbolo redefinido."));
    }

    // Se o label for inválido, adiciona um erro
    else if(validate_label(label).size()>1){
        _module->program_errors.push_back(new ErrorMessage(line_number, text, "Erro Léxico: "+ validate_label(label)));
    }

    // Se o label for uma palavra reservada, adiciona um erro
    else if(is_reserved_word(instruction_table, directive_table, label)){
        _module->program_errors.push_back(new ErrorMessage(line_number, text, "Erro sintático: Palavra reservada."));
    }

    // Senão, adciona símbolo a tabela
    else{
        symbol_table->push_back(new Symbol(program_counter, label, false));
    }
}

// Realiza o processamento de um token do tipo instrução ou diretiva
void instruction_processing(Instruction *inst,Directive *directive, vector<Symbol*> *symbols_table, int *program_counter, int line_number, string text, Module *_module){
    // Se for instrução atualiza PC
    if(inst != NULL){
        *program_counter+=inst->length;
    }
    // Se for diretiva atualiza PC
    else if(directive != NULL){
        *program_counter+=directive->len;
        if(directive->name == "EXTERN"){
            // Acessa o último elemento na tabela de símbolos
            int last_element_index = (*symbols_table).size() - 1;
            // Se esse index for negativo, a tabela de símbolos está vazia
            if(last_element_index < 0){
                red_cout("ERRO: Extern sem label");
            }
            // Senão adiciona flag de símbolo externo
            else{
                (*symbols_table)[last_element_index]->is_extern = true; 
            }
        }else if(directive->name == "BEGIN"){
            if(_module->is_this_module_the_only){
                _module->program_errors.push_back(new ErrorMessage(line_number, text, "Erro XXXXXX: Uso de BEGIN em programa único."));
            }else{
                _module->contain_begin = true;
            }
        }else if(directive->name == "END"){
            if(_module->is_this_module_the_only){
                _module->program_errors.push_back(new ErrorMessage(line_number, text, "Erro XXXXXX: Uso de END em programa único."));
            }else{
                _module->contain_end = true;
            }
        }
    }
    // Senão, gera mensagem de erro: instrução que não existe.
    else{
        _module->program_errors.push_back(new ErrorMessage(line_number, text, "Erro sintático: Essa instrução ou diretiva não existe."));
    }
}

vector<Symbol*> first_pass(Module *module)
{
    // As tabelas de instrução e de diretivas são padrões
    vector<Instruction *> instruction_table = create_instruction_table();
    vector<Directive *> directive_table = create_directive_table();
    
    // Cria uma tabela para armazenar os símbolos
    vector<Symbol *> symbol_table;

    // Inicializa o contador de programa
    int program_counter = 0;

    for(auto it: module->lines){

        // Pula section data e section text
        if(it->line_number == module->data_position || it->line_number==module->text_position){
            cout << it->text << "---------------------"<<  it->line_number << endl; 
            continue;
        }

        if(contain_label(it->text)){
            // Separa o label do resto pro ':'
            vector<string> line_vector = split_string(it->text, ':');

            // Obtem o elemento que corresponde ao label
            string label = line_vector[0];

            // Só tem símbolo!!
            if(line_vector.size() == 1){
                // Processa label
                label_processing(label, program_counter, it->line_number, it->text, &symbol_table, instruction_table, directive_table, module);

            // Tem símbolo e um comando
            }else{
                // Obtem instrução
                string instruction = line_vector[1];
                
                // Processa label
                label_processing(label, program_counter, it->line_number, it->text, &symbol_table, instruction_table, directive_table, module);

                // Processa instrução

                // Separa instrução por espaços
                vector<string> instructions_elements = split_string(instruction, ' ');

                // Obtem instrução e diretiva.
                Instruction *inst = get_instruction_by_name(instruction_table, instructions_elements[0]);
                Directive *directive = get_directive_by_name(directive_table, instructions_elements[0]);
                
                // Realiza o processamento da istrução ou diretiva.
                instruction_processing(inst, directive, &symbol_table, &program_counter, it->line_number, it->text, module);
            }
        
        // Tem um comando
        }else{
            // Separa instrução por espaços
            vector<string> line_vector = split_string(it->text, ' ');

            // Obtem instrução e diretiva.
            Instruction *inst = get_instruction_by_name(instruction_table, line_vector[0]);
            Directive *directive = get_directive_by_name(directive_table, line_vector[0]);

            // Realiza o processamento da istrução ou diretiva. 
            instruction_processing(inst, directive,&symbol_table , &program_counter, it->line_number, it->text, module);
        }
    }
    
    if(!module->is_this_module_the_only){
        if(!module->contain_begin){
            module->program_errors.push_back(new ErrorMessage(0, "", "Erro XXXXX: Módulo sem BEGIN."));
        }
        if(!module->contain_end){
            module->program_errors.push_back(new ErrorMessage(0, "", "Erro XXXXX: Módulo sem END."));
        }
    }


    return symbol_table;
}
