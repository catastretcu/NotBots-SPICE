//redundant for now: only testing with simulate.cpp


/*
#include "Components.hpp"
#include "Node.hpp"
#include "Analysis.hpp"
#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <utility>

using namespace std;
*/

#include <iostream>
#include <cstring>
#include <sstream>
#include <utility>
#include <cassert>

using namespace std;

//helper function for function read_input -> without transistor support
pair<string, string> read_nodes(string &linern)
{
    stringstream ssrn(linern);
    string first_node, second_node;
    
    getline(ssrn, first_node, ' ');
    
    getline(ssrn, first_node, ' ');
    getline(ssrn, second_node, ' ');
    
    return make_pair(first_node, second_node);
}

//function used for direct input: returns a map of the circuit and the analysis type.
void read_input(Nodes *circuit, Analysis *command)
{
    string lineri;
    
    while(getline(cin, lineri))
    {
        if(lineri[0] != '*' && lineri[0] != '.')
            circuit->add_branch(read_nodes(lineri), create_component(lineri));
        else
        {
            //save analysis type data
            if(lineri[0] == '.' && lineri != ".end")
                command = choose_analysis(lineri);
        }
    }
}
