#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "util.h"

using namespace std;

string remove_white_spaces(string s)
{
    // Coloca a string em um vetor;
    vector<char> v = transform_string_in_vector(s);

    // Remove espaços em branco.
    vector<char> cleaned_vector = {};
    int v_size = v.size();

    for (int x = 0; x < v_size; x++)
    {
        if (x != v_size - 1)
        {
            if (!(v[x] == ' ' && v[x] == v[x + 1]))
            {
                cleaned_vector.push_back(v[x]);
            }
        }
        else
        {
            if (v[x] != ' ')
            {
                cleaned_vector.push_back(v[x]);
            }
        }
    }

    if (cleaned_vector.size() != 0)
    {
        if (cleaned_vector[0] == ' ')
        {
            cleaned_vector.erase(cleaned_vector.begin());
        }
    }

    return transform_vector_in_string(cleaned_vector);
}

vector<Line*> clean_program_lines(vector<Line*> program_lines)
{

    // Limpa epaços em branco
    vector<Line*> removed_white_spaces_vector;

    for (auto it : program_lines)
    {
        Line *l = new Line(it->line_number, remove_white_spaces(it->text));
        removed_white_spaces_vector.push_back(l);
    }

    vector<Line*> removed_empty_lines_vector;

    for (auto it : removed_white_spaces_vector)
    {
        if (it->text.size() != 0)
        {
            Line *l = new Line(it->line_number, it->text);
            removed_empty_lines_vector.push_back(l);
        }
    }

    return removed_empty_lines_vector;
}

string remove_comments(string s){
    // Quando o ; está no início o split não funcionava
    if(s[0] == ';'){
        return "";
    }
    vector<string> splitted_string = split_string(s, ';');
    if(splitted_string.size()==0){
        splitted_string.clear();
        return "";
    }
    return splitted_string[0];
}

vector<Line*> put_data_section_in_the_end(vector<Line*> program_lines){

    int data_position = find_line_index(program_lines, "SECTION DATA");
    int text_position = find_line_index(program_lines, "SECTION TEXT");
    
    // Se a data está no final, já é o formato que eu desejo.
    if(data_position > text_position){
        return program_lines;
    }


    vector<Line*> new_program = {};

    for(auto it:program_lines){
        if(it->line_number < text_position){
            continue;
        }
        else {
            Line *l = new Line(it->line_number, it->text); 
            new_program.push_back(l);
        }
    }
    for(auto it:program_lines){
        if(it->line_number >= data_position && it->line_number < text_position){
            Line *l = new Line(it->line_number, it->text); 
            new_program.push_back(l);
        }
    }

    return new_program;
}

string to_upper_case(string s){
    int tam_s = s.size();
    string upper_case = "";
    int i;
    for(i = 0; i < tam_s; i++){
        upper_case.push_back(toupper(s[i]));
    }
    return upper_case;
}

vector<char> transform_string_in_vector(string s)
{
    vector<char> v = {};
    int tam = s.size();
    for (int i = 0; i < tam; i++)
    {
        v.push_back(s[i]);
    }
    return v;
}


string transform_vector_in_string(vector<char> v)
{
    string s;
    int tam = v.size();
    for (int i = 0; i < tam; i++)
    {
        s.push_back(v[i]);
    }
    return s;
}


vector<string> split_string(string s, char separador){
    vector<string> splitted_string;

    int len_s = s.size();
    string aux = "";
    int i = 0;
    for(i = 0; i < len_s; i++){
        if(s[i] != separador){
        aux += s[i];
        }else{
            if(aux != ""){
                splitted_string.push_back(aux);
                aux="";
            }
        }
    }
    if(aux.size()!=0)
    {
        splitted_string.push_back(aux);
    }
    return splitted_string;
}

vector<int> split_string_integer(string s, char separador){

    vector<int> splitted_string;

    int len_s = s.size();
    string aux = "";
    int i = 0;
    for(i = 0; i < len_s; i++){
        if(s[i] != separador){
            aux += s[i];
        }else{
            if(aux != ""){
                splitted_string.push_back(stoi(aux));
                aux="";
            }
        }
    }
    if(aux.size()!=0)
    {
        splitted_string.push_back(stoi(aux));
    }
    return splitted_string;
}

void print_program(vector<Line*> program)
{
    for (auto it : program)
    {
        cout << it->line_number << " - " << it->text << " - " << it->text.size() << endl;
        // cout << it->texto << endl;
    }
}
void print_program_errors(vector<ErrorMessage *> program_errors){
    for (auto it : program_errors)
    {
        red_cout(it->error_message);
        cout << "Na linha " << it->line_number << ": " << it->error_line << endl << endl;
    }
}

int find_line_index(vector<Line*> program_lines, string searched_text){
    for(auto it:program_lines){
        if(it->text == searched_text){
            return it->line_number;
        }
    }
    return 0;
}


// Retorna uma tabela com as instruções
vector<Instruction *> create_instruction_table()
{

    vector<Instruction *> instructions_table;

    instructions_table.push_back(new Instruction(1, 2, "ADD"));
    instructions_table.push_back(new Instruction(2, 2, "SUB"));
    instructions_table.push_back(new Instruction(3, 2, "MUL"));
    instructions_table.push_back(new Instruction(4, 2, "DIV"));
    instructions_table.push_back(new Instruction(5, 2, "JMP"));
    instructions_table.push_back(new Instruction(6, 2, "JMPN"));
    instructions_table.push_back(new Instruction(7, 2, "JMPP"));
    instructions_table.push_back(new Instruction(8, 2, "JMPZ"));
    instructions_table.push_back(new Instruction(9, 3, "COPY"));
    instructions_table.push_back(new Instruction(10, 2, "LOAD"));
    instructions_table.push_back(new Instruction(11, 2, "STORE"));
    instructions_table.push_back(new Instruction(12, 2, "INPUT"));
    instructions_table.push_back(new Instruction(13, 2, "OUTPUT"));
    instructions_table.push_back(new Instruction(14, 1, "STOP"));

    return instructions_table;
}

Instruction *get_instruction_by_opcode(vector<Instruction *> v, int op)
{
    int tam_v = v.size();
    for(int i=0; i<tam_v; i++){
        if(v[i]->opcode == op){
            return v[i];
        }
    }

    return NULL;
}

Instruction *get_instruction_by_name(vector<Instruction *> v, string name)
{
    int tam_v = v.size();
    for(int i=0; i<tam_v; i++){
        if(v[i]->name == name){
            return v[i];
        }
    }
    return NULL;
}

bool is_instruction(vector<Instruction *> v, string text)
{
    Instruction *inst = get_instruction_by_name(v, text);
    if(inst == NULL){
        return false;
    }
    return true;
}

bool is_label(string text){
    int tam_t = text.size();
    if(text[tam_t-1] == ':'){
        return true;
    }
    return false;
}

Symbol* get_symbol_by_name(vector<Symbol*> v, string name){
    int tam_v = v.size();
    for(int i=0; i<tam_v; i++){
        if(v[i]->label == name){
            return v[i];
        }
    }
    return NULL;
}

vector<Directive*> create_directive_table(){
    vector<Directive*> v = {};

    v.push_back(new Directive(1, "SPACE"));
    v.push_back(new Directive(1, "CONST"));

    return v;
};

Directive* get_directive_by_name(vector<Directive*> v, string name){
    int tam_v = v.size();
    for(int i=0; i<tam_v; i++){
        if(v[i]->name == name){
            return v[i];
        }
    }
    return NULL;
}

bool is_directive(vector<Directive*> v, string s){
    Directive *directive = get_directive_by_name(v, s);
    if(directive == NULL){
        return false;
    }
    return true;
}

bool is_letter(char c){
    c = toupper(c);
    if(int(c) >= int('A') && int(c) <= int('Z')){
        return 1;
    }
    return 0;
}
bool is_number(char c){
    if(int(c) >= int('0') && int(c) <= '9'){
        return 1;
    }
    return 0;
}

bool contain_only_letter_number_or_underscore(string s){
    for (auto it:s){
        if(!is_letter(it) && !is_number(it) && !(it=='_')){
            return 0;
        }
    }
    return 1;
}

string validate_label(string s){
    if(s.size() > 50){
        return "Label com tamanho inválido.";
    }
    else if( !contain_only_letter_number_or_underscore(s) ){
        return "Label contém caracteres inválidos.";
    }
    else if(is_number(s[0])){
        return "Label inicia com número.";
    }
    return "";
}

bool is_reserved_word(vector<Instruction *> instructions, vector<Directive *> directives, string s){
    if(is_instruction(instructions, s)){
        return true;
    }else if(is_directive(directives, s)){
        return true;
    }

    return false;
}


bool contain_label(string line){
    if(line.find(':') == -1){
        return false;
    }
    return true;
}

void red_cout(string s){
    cout << "\033[1;31m"<<s<<"\033[0m\n";
}

void blue_cout(string s){
    cout << "\033[1;36m"<<s<<"\033[0m\n";
}

bool is_valid_number(string s){
    int s_zize = s.size();
    
    for(int i = 0; i < s_zize; i++){
        if(!is_number(s[i])){
            if(i == 0 && s[i] == '-'){
                continue;
            }else{
                return false;
            }
        }
    }
    return true;
}

