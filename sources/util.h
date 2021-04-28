#ifndef UTIL
#define UTIL

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "module.h"

using namespace std;

// Remove os espaços em brancos de uma string
string remove_white_spaces(string s);

// Apaga linhas em branco de um programa
vector<Line*> clean_program_lines(vector<Line*> program_lines);

// Apaga os comentários de um programa
string remove_comments(string s);

// Coloca data section depois da section text se existir
vector<Line*> put_data_section_in_the_end(vector<Line*> program_lines);

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

// Cria tabela de isntruções
vector<Instruction *> create_instruction_table();

// Obtem instrucao por opcode.
Instruction *get_instruction_by_opcode(vector<Instruction *> v, int op);

// Obtem instrucao por nome.
Instruction *get_instruction_by_name(vector<Instruction *> v, string name);

// Verifica se uma string é uma instrucao
bool is_instruction(vector<Instruction *> v, string text);

// Verfica se uma string é um rótulo
bool is_label(string text);

// Obtém simbolo por nome
Symbol *get_symbol_by_name(vector<Symbol *> v, string name);

// Cria tabela de diretivas
vector<Directive*> create_directive_table();

// Procura diretiva na tabela por nome
Directive* get_directive_by_name(vector<Directive*> v, string name);

// Verifica se é diretiva
bool is_directive(vector<Directive*> v, string s);

// Valida label
string validate_label(string s);

// Verifica se label é palavra reservada
bool is_reserved_word(vector<Instruction *> instructions, vector<Directive *> directives, string s);

// Verifica se contém label em uma linha por ":"
bool contain_label(string line);

// Imprime em vermelho
void red_cout(string s);

// Imprime em azul
void blue_cout(string s);

// Imprime em verde
void green_cout(string s);

// Verifica se número é válido
bool is_valid_number(string s);

#endif