#include "Components.hpp"
#include "Node.hpp"
#include "Analysis.hpp"
#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <utility>

using namespace std;

//helper function for function map_circuit -> without transistor support
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
pair<Nodes, Analysis> read_input(Nodes &circuit, Analysis &command)
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
    
    return make_pair(circuit, command);
}
