#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "util.h"
#include "module.h"

using namespace std;

Module* pre_processing(string program_name)
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

    
    // Retorona novo módulo criado
    return new Module(sorted_lines, data_position, text_position, split_string(program_name, '.')[0]);
}
