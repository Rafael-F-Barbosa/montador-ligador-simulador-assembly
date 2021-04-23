#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "util.h"

using namespace std;


// Realiza o processamento de um token do tipo label
void label_processing(string label, int program_counter, int line_number, string text, vector<Symbol*> *symbol_table, vector<Instruction*> instruction_table,
vector<Directive* > directive_table, Program *program){
    
    // Elimina espaço no final do label, se houver
    label = split_string(label, ' ')[0];

    // Se o label for redefinido, adiciona um erro
    if(get_symbol_by_name(*symbol_table, label)){
        program->program_errors.push_back(new ErrorMessage(line_number, text, "Erro semântico: símbolo redefinido."));
    }

    // Se o label for inválido, adiciona um erro
    else if(validate_label(label).size()>1){
        program->program_errors.push_back(new ErrorMessage(line_number, text, "Erro Léxico: "+ validate_label(label)));
    }

    // Se o label for uma palavra reservada, adiciona um erro
    else if(is_reserved_word(instruction_table, directive_table, label)){
        program->program_errors.push_back(new ErrorMessage(line_number, text, "Erro sintático: Palavra reservada."));
    }

    // Senão, adciona símbolo a tabela
    else{
        symbol_table->push_back(new Symbol(program_counter, label));
    }

}

// Realiza o processamento de um token do tipo instrução ou diretiva
void instruction_processing(Instruction *inst,Directive *directive, int *program_counter, int line_number, string text, Program *program){
    
    // Se for instrução atualiza PC
    if(inst != NULL){
        *program_counter+=inst->length;
    }
    // Se for diretiva atualiza PC
    else if(directive != NULL){
        *program_counter+=directive->len;
    }
    // Senão, gera mensagem de erro: instrução que não existe.
    else{
        program->program_errors.push_back(new ErrorMessage(line_number, text, "Erro sintático: Essa instrução ou diretiva não existe."));
    }
}

vector<Symbol*> first_pass(Program *program)
{
    // As tabelas de instrução e de diretivas são padrões
    vector<Instruction *> instruction_table = create_instruction_table();
    vector<Directive *> directive_table = create_directive_table();
    
    // Cria uma tabela para armazenar os símbolos
    vector<Symbol *> symbol_table;

    // Inicializa o contador de programa
    int program_counter = 0;

    for(auto it: program->lines){

        // Pula section data e section text
        if(it->line_number == program->data_position || it->line_number==program->text_position){
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
                label_processing(label, program_counter, it->line_number, it->text, &symbol_table, instruction_table, directive_table, program);

            // Tem símbolo e um comando
            }else{
                // Obtem instrução
                string instruction = line_vector[1];
                
                // Processa label
                label_processing(label, program_counter, it->line_number, it->text, &symbol_table, instruction_table, directive_table, program);

                // Processa instrução

                // Separa instrução por espaços
                vector<string> instructions_elements = split_string(instruction, ' ');

                // Obtem instrução e diretiva.
                Instruction *inst = get_instruction_by_name(instruction_table, instructions_elements[0]);
                Directive *directive = get_directive_by_name(directive_table, instructions_elements[0]);
                
                // Realiza o processamento da istrução ou diretiva.
                instruction_processing(inst, directive, &program_counter, it->line_number, it->text, program);
            }
        
        // Tem um comando
        }else{
            // Separa instrução por espaços
            vector<string> line_vector = split_string(it->text, ' ');

            // Obtem instrução e diretiva.
            Instruction *inst = get_instruction_by_name(instruction_table, line_vector[0]);
            Directive *directive = get_directive_by_name(directive_table, line_vector[0]);

            // Realiza o processamento da istrução ou diretiva. 
            instruction_processing(inst, directive, &program_counter, it->line_number, it->text, program);
        }
    }


    // Imprime tabela de símbolos
    // cout << "TABELA DE SIMBOLOS" << endl;
    // for(auto it:symbol_table)
    //     cout << it->label << ": " << it->position << endl;

    return symbol_table;
}
