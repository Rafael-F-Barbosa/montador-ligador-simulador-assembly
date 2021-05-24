#include <iostream>


using namespace std;

// Impressão de cada instrução no simulador
void format_cout_instruction(int program_counter, int acc, bool is_output=false, int output=0){
    cout << "PC <- " + to_string(program_counter) << endl;
    cout << "ACC <- " + to_string(acc) << endl;
    if(is_output){
        cout << "OUTPUT: " << output << endl;
    }
    cout << endl;
}