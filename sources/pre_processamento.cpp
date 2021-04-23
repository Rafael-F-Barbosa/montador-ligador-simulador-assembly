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

Program* pre_processing(string program_name)
{
    // Cria um objeto que lê arquivos.
    ifstream input_file;

    // Declara string para armazenar cada linha do ponteiro.
    string line;

    // Abre o arquivo.
    input_file.open(program_name);

    // Criação de um container dec chave-valor com o map.

    vector<Line*> program_lines;

    if (input_file.is_open())
    {
        // Insere o programa no conteiner map
        int i = 1; // Linhas do programa original(Começa em 1 mesmo)
        while (getline(input_file, line))
        {
            Line *l = new Line(i, to_upper_case(remove_comments(line)));
            program_lines.push_back(l);
            i++;
        }
        input_file.close();
    }
    else
    {
        cout << "Não foi possível abrir o arquivo" << endl;
    }

    // Retira espaços desnecessários entre as linhas
    vector<Line*> cleaned_lines = clean_program_lines(program_lines);

    // Se SECTION DATA estiver no início, colocar no final
    vector<Line*> sorted_lines = put_data_section_in_the_end(cleaned_lines);

    // Get the data and text position
    int data_position = find_line_index(sorted_lines, "SECTION DATA");
    int text_position = find_line_index(sorted_lines, "SECTION TEXT");

    // Cria objeto chamado do tipo Program
    Program *program = new Program(sorted_lines, data_position, text_position);

    return program;
}
