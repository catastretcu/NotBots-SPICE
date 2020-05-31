#include "Components.hpp"
#include "Node.hpp"
#include "Analysis.hpp"
#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <utility>

using namespace std;

//helper function for function read_input -> without transistor support
pair<string, string> read_nodes(string &line)
{
    stringstream ss(line);
    string first_node, second_node;
    
    getline(ss, first_node, ' ');
    
    getline(ss, first_node, ' ');
    getline(ss, second_node, ' ');
    
    return make_pair(first_node, second_node);
}

//function used for direct input: returns a map of the circuit and the analysis type.
void read_input(Nodes &circuit, Analysis &command)
{
    string line;
    
    while(getline(cin, line))
    {
        if(line[0] != '*' && line[0] != '.')
            circuit.add_branch(read_nodes(line), create_component(line));
        else
        {
            //save analysis type data
            if(line[0] == '.' && line != ".end")
                command = choose_analysis(line);
        }
    }
}
