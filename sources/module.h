#ifndef MODULE
#define MODULE

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace std;


// Definição tipo linha
class Line
{
public:
    int line_number;
    string text;

    Line(int n, string l)
    {
        line_number = n;
        text = l;
    }
};

// Definição tipo mensagem de erro
class ErrorMessage{
public:
    int line_number;
    string error_line;
    string error_message;

    ErrorMessage(int l_n, string e_l, string e_m)
    {
        line_number = l_n;
        error_line = e_l;
        error_message = e_m;
    }
};

// Definição tipo Instrução
class Instruction
{
public:
    int opcode;
    int length;
    string name;

    Instruction(int op, int l, string n)
    {
        opcode = op;
        length = l;
        name = n;
    }
};

// Definição tipo diretiva
class Directive{
public:
    int len;
    string name;
    Directive(int l, string n){
        len = l;
        name = n;
    }
};


// Definição tipo símbolo
class Symbol
{
public:
    int position;
    string label;
    bool is_extern;

    Symbol(int p, string l, bool is_ext)
    {
        position = p;
        label = l;
        is_extern = is_ext;
    }
};

// Definição tipo Definição
class Definition{
public:
    int value;
    string label;

    Definition(int v, string l){
        value = v;
        label = l;
    }
};

// Definição Uso
class Use{
public:
    int value;
    string label;
    Use(int v, string l){
        value = v;
        label = l;
    }
};


// Definição tipo módulo
class Module
{
public:
    // Linhas do programa
    vector<Line *> lines = {};

    // Tabelas importantes
    vector<Symbol *> symbols_table = {};
    vector<Use *> uses_table = {};
    vector<Definition *> definitions_table = {};
    vector<int> bits_map = {};

    // Codigo obeto do programa
    vector<string> object_code = {} ;

    // Controle de Begin e End
    bool is_this_module_the_only = true;
    bool contain_begin = false;
    bool contain_end   = false;

    // Atributos para cabeçalho
    string module_name;
    int module_len;

    // Posições de inícios section text e data
    int data_position;
    int text_position;
    
    // Vetor de mensagens de erro
    vector< ErrorMessage *> program_errors;

    // Inicialização do módulo
    Module(vector<Line *> v, int d_p, int t_p, string m_n)
    {
        lines.assign(v.begin(), v.end());
        data_position = d_p;
        text_position = t_p;
        module_name = m_n;
    }
};

// Definição tipo Programa
class Program{
public:
    vector<Module *> modules = {};
    vector<int> correction_factor = {0};
    vector<Definition *> global_definitions_table = {};

    Program(vector<Module *> m){
        modules.assign(m.begin(), m.end());
    }
    Definition* get_definition_by_name(string name){
        int tam_v = global_definitions_table.size();
        for(int i=0; i<tam_v; i++){
            if(global_definitions_table[i]->label == name){
                return global_definitions_table[i];
            }
        }
        return NULL;

    }
};

#endif