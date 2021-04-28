#ifndef MODULE
#define MODULE

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "util.h"

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


// Classe simbolo
class Symbol
{
public:
    int position;
    string label;
    bool is_label_extern;

    Symbol(int p, string l)
    {
        position = p;
        label = l;
    }
};


class Definition{
public:
    int value;
    string label;

    Definition(int v, string l){
        value = v;
        label = l;
    }
};

class Use{
public:
    int value;
    string label;
    Use(int v, string l){
        value = v;
        label = l;
    }
};

class Module
{
public:
    vector<Line *> lines;
    
    vector<Symbol *> symbols_table;
    vector<Symbol *> uses_table;

    int data_position;
    int text_position;
    
    int module_len;

    vector< ErrorMessage *> program_errors;

    Module(vector<Line *> v, int d_p, int t_p)
    {
        lines.assign(v.begin(), v.end());
        data_position = d_p;
        text_position = t_p;
    }
};

class Program{
public:
    vector<Module *> modules = {};

    Program(vector<Module *> m){
        modules.assign(m.begin(), m.end());
    }

};

#endif