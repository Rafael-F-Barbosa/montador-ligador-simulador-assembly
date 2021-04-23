#ifndef UTIL
#define UTIL

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;


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


class Program
{
public:
    vector<Line *> lines;
    int data_position;
    int text_position;
    vector< ErrorMessage *> program_errors;

    Program(vector<Line *> v, int d_p, int t_p)
    {
        lines.assign(v.begin(), v.end());
        data_position = d_p;
        text_position = t_p;
    }
};

// Transforma string em maiúscula
string to_upper_case(string s);

// Transforma um string em um vetor
vector<char> transform_string_in_vector(string s);

// Transforma vetor em string
string transform_vector_in_string(vector<char> v);

// Dividir string em substring com base em um separador
vector<string> split_string(string s, char separador);

// Dividir string em substring com base em um separador para inteiros
vector<int> split_string_integer(string s, char separador);

// Imprime o programa na linha de comando
void print_program(vector<Line *> program);

// Imprime os erros do programa na linha de comando
void print_program_errors(vector<ErrorMessage *> program_errors);

// Encontra a posiçao de uma linha
int find_line_index(vector<Line *> program_lines, string searched_text);

// Classe instrucao
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

vector<Instruction *> create_instruction_table();

// Obtem instrucao por opcode.
Instruction *get_instruction_by_opcode(vector<Instruction *> v, int op);

// Obtem instrucao por nome.
Instruction *get_instruction_by_name(vector<Instruction *> v, string name);

// Verifica se uma string é uma instrucao
bool is_instruction(vector<Instruction *> v, string text);

// Verfica se uma string é um rótulo
bool is_label(string text);

// Classe simbolo
class Symbol
{
public:
    int position;
    string label;
    Symbol(int p, string l)
    {
        position = p;
        label = l;
    }
};

// Obtém simbolo por nome
Symbol *get_symbol_by_name(vector<Symbol *> v, string name);

// Classe diretiva
class Directive{
public:
    int len;
    string name;
    Directive(int l, string n){
        len = l;
        name = n;
    }
};

vector<Directive*> create_directive_table();

Directive* get_directive_by_name(vector<Directive*> v, string name);

bool is_directive(vector<Directive*> v, string s);


string validate_label(string s);

bool is_reserved_word(vector<Instruction *> instructions, vector<Directive *> directives, string s);

bool contain_label(string line);


void red_cout(string s);

void blue_cout(string s);

bool is_valid_number(string s);

#endif