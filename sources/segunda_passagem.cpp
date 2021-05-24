#include <iostream>
using namespace std;

#include "util.h"
#include "module.h"

// Realiza o processamento das instruções
void instruction_processing(Line *line, Instruction *inst, int *program_counter, vector<string> line_analysed, 
vector<Symbol *> symbols_table, vector<string> *object_code, Module *_module){
    
    // Tratamento de instruções de tamanho 3
    if(inst->length == 3){

        // Quantidade de argumentos inválida(separado por espaços)
        if(line_analysed.size() != 2){
            _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro sintático: Quantidade de argumentos incorreta."));
        }
        // Senão: a quantidade é certa(separado por espaços)
        else{
            // Separa por vírgula
            vector<string> arguments = split_string(line_analysed[1], ',');

            // Valida caso seja menor ou maior
            if(arguments.size() < 2){
                _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro sintático: Quantidade de argumentos menor que esperada."));
            }else if(arguments.size() > 2){
                _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro sintático: Quantidade de argumentos maior que esperada."));
            }

            // Senão a quantidade de argumentos é correta
            else{
                bool first_argument_contain_lexical_errors = false;
                bool second_argument_contain_lexical_errors = false;

                if(validate_label(arguments[0]).size()>1){
                    _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro Léxico: No primeiro argumento. "+ validate_label(arguments[0])));
                    first_argument_contain_lexical_errors = true;
                }
                if(validate_label(arguments[1]).size()>1){
                    _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro Léxico: No segunda argumento. "+ validate_label(arguments[1])));
                    second_argument_contain_lexical_errors = true;
                }       

                
                // Obtem os dois argumentos na tabela de simbolos
                Symbol *first_argument = get_symbol_by_name(symbols_table, arguments[0]);
                Symbol *second_argument = get_symbol_by_name(symbols_table, arguments[1]);

                // Adiciona opcode da instrução
                object_code->push_back(to_string(inst->opcode)+"a");

                // Se o argumento for encontrado adiciona o seu valor ao codigo objeto
                if(first_argument != NULL){
                    
                    // Caso seja externa a diretiva é adicionada na tabela de uso
                    if(first_argument->is_extern){
                        // Obtém nome e posição do uso do label
                        Use *new_use = new Use(*program_counter + 1,first_argument->label) ;
                        _module->uses_table.push_back(new_use);
                    }


                    // Adiciona a posição do argumento
                    object_code->push_back(to_string(first_argument->position)+"r");
                    // Atualiza o PC
                    // *program_counter +=inst->length; -> Só atualiza o PC no segundo argumento
                }
                // Senão gera erro de variável não declarada
                else{
                    if(!first_argument_contain_lexical_errors){
                        _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro semântico: Primeiro argumento não declarado."));
                    }
                }

                // Se o argumento for encontrado adiciona o seu valor ao codigo objeto
                if(second_argument != NULL){

                    // Caso seja externa a diretiva é adicionada na tabela de uso
                    if(second_argument->is_extern){
                        // Obtém nome e posição do uso do label
                        Use *new_use = new Use(*program_counter + 2,second_argument->label) ;
                        _module->uses_table.push_back(new_use);
                    }

                    // Adiciona a posição do argumento
                    object_code->push_back(to_string(second_argument->position)+"r");
                    // Atualiza o PC
                    *program_counter +=inst->length; // -> Só atualiza o PC no segundo argumento
                }
                // Senão gera erro de variável não declarada
                else{
                    if(!second_argument_contain_lexical_errors){
                        _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro semântico: Segundo argumento não declarado."));
                    }
                }
                
            }
        }
    }

    // Tratamento de instruções de tamanho 2
    else if(inst -> length == 2){
        if(line_analysed.size() != 2){
            _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro sintático: Quantidade de argumentos incorreta."));
        }
        else{
            vector<string> arguments = split_string(line_analysed[1], ',');
            if(arguments.size() > 1){
                _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro sintático: Quantidade de argumentos maior que esperada."));
            }else if(arguments.size() < 1){
                _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro sintático: Quantidade de argumentos menor que esperada."));
            }
            else{
                if(validate_label(arguments[0]).size()>1){
                    _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro Léxico: "+ validate_label(arguments[0])));
                }else{

                    // Obtem o argumento na tabela de símbolos
                    Symbol *argument = get_symbol_by_name(symbols_table, arguments[0]);
                    
                    // Se o argumento for encontrado adiciona o seu valor ao codigo objeto
                    if(argument != NULL){

                        // Caso seja externa a diretiva é adicionada na tabela de uso
                        if(argument->is_extern){
                            // Obtém nome e posição do uso do label
                            Use *new_use = new Use(*program_counter + 1,argument->label) ;
                            _module->uses_table.push_back(new_use);
                        }

                        // Adiciona opcode da instrução
                        object_code->push_back(to_string(inst->opcode)+"a");
                        // Adiciona a posição do argumento
                        object_code->push_back(to_string(argument->position)+"r");
                        // Atualiza o PC
                        *program_counter +=inst->length;

                    }
                    // Senão gera erro de variável não declarada
                    else{
                        _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro semântico: Argumento não declarado."));
                    }
                }
            }
        }
    }

    // Tratamento de instruções de tamanho 1
    else if(inst -> length == 1){
        if(line_analysed.size() != 1){
            _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro sintático: Quantidade de argumentos maior que esperada."));
        }
        object_code->push_back(to_string(inst->opcode)+"a");
        *program_counter +=inst->length;
    }
}

void directive_processing(Line* line, Directive *directive, int *program_counter, vector<string> line_analysed, 
vector<string> *object_code, Module *_module){
    // Tratar casos como mais de um argumento pros dois
    // Tratar const de formato inválido

    // Caso a diretiva seja CONST
    if(directive->name == "SPACE"){
        
        if(line_analysed.size() != 1){
            // Erro quantidade de argumentos inválida para diretiva CONST
            _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro sintático: SPACE não recebe argumentos."));
        }
        else{
            object_code->push_back("0a");
            program_counter+=directive->len;
        }
    }

    // Caso a diretiva seja CONST
    else if(directive ->name == "CONST"){
        // Erro quantidade de argumentos inválida para diretiva CONST
        if(line_analysed.size() < 2){
            _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro sintático: CONST recebe pelo menos 1 argumento."));
        }
        // Erro quantidade de argumentos inválida para diretiva CONST
        else if(line_analysed.size() > 2){
            _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro sintático: CONST no máximo 1 argumento."));
        }
        else{
            // Verifica se o argumento é válido
            if(is_valid_number(line_analysed[1])){
                object_code->push_back(line_analysed[1]+"a");
                program_counter+=directive->len;
            }
            // Senão gera erro
            else{
                _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro sintático: CONST só aceita inteiros."));
            }
        }
    }
    else if(directive->name == "PUBLIC"){
        if(line_analysed.size() != 2){
            _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro sintático: PUBLIC aceita 1 argumento."));
        }else{
            // Divide argumentos de public por vírgula
            vector<string> arguments = split_string(line_analysed[1], ',');
            if(arguments.size() != 1){
                _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro sintático: PUBLIC aceita 1 argumento."));
            }else{
                // Obtem o argumento na tabela de símbolos
                Symbol *argument = get_symbol_by_name(_module->symbols_table, arguments[0]);

                // Se o argumento tiver sido declarado coloca ele na tabela de definições
                if(argument != NULL){
                    Definition *def = new Definition(argument->position, argument->label);
                    _module->definitions_table.push_back(def);
                }
                // Senão gera erro
                else{
                    _module->program_errors.push_back(new ErrorMessage(line->line_number, line->text, "Erro semântico: Argumento public não declarado."));
                }
            }
        }
    }

}

// Realiza segunda passagem
vector<string> second_pass(Module *_module){
  // As tabelas de instrução e de diretivas são padrões
    vector<Instruction *> instruction_table = create_instruction_table();
    vector<Directive *> directive_table = create_directive_table();

  // Cria vetor pra armazenar o código objeto
    
    vector<string> object_code = {};

    int program_counter = 0;

    for(auto it: _module->lines){

        // Pula section data e section text
        if(it->line_number == _module->data_position || it->line_number==_module->text_position){
            continue;
        }

        if(contain_label(it->text)){
            // Separa o label do resto pro ':'
            vector<string> line_vector = split_string(it->text, ':');

            string label = line_vector[0];

            // Só tem símbolo - linha ignorada
            if(line_vector.size() == 1){
                continue;

            // Tem símbolo e um comando
            }else{
                // Ignora o símbolo e processa o comando
                
                // Extrai a instrução
                string instruction = line_vector[1];
                // Separa instrução por espaços
                vector<string> line_analysed = split_string(instruction, ' ');
                
                // Procura na tabela de instruções e de diretivas
                Instruction *inst = get_instruction_by_name(instruction_table, line_analysed[0]);
                Directive *directive = get_directive_by_name(directive_table, line_analysed[0]);

                
                // Se for instrução
                if(inst != NULL){
                    instruction_processing(it, inst, &program_counter, line_analysed, _module->symbols_table, &object_code, _module);
                }

                // Se for diretiva
                else if(directive != NULL){
                    directive_processing(it, directive, &program_counter, line_analysed, &object_code, _module);
                }

                // Caso não seja nem diretiva nem instrução foi tratada na primeira passagem
            }

        // Contém apenas instrução ou diretia
        }else{
            // Separa instrução por espaços
            vector<string> line_analysed = split_string(it->text, ' ');

            Instruction *inst = get_instruction_by_name(instruction_table, line_analysed[0]);
            Directive *directive = get_directive_by_name(directive_table, line_analysed[0]);

            // Se for instrução
            if(inst != NULL){
                instruction_processing(it, inst, &program_counter, line_analysed, _module->symbols_table, &object_code,  _module);
            }
            // Se for diretiva
            else if(directive != NULL){
                directive_processing(it, directive, &program_counter, line_analysed, &object_code, _module);
            }

                // Caso não seja nem diretiva nem instrução foi tratada na primeira passagem
        }
    }
    return object_code;

}
