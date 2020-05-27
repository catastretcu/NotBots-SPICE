#include "Components.hpp"
#include "Node.hpp"
#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>

using namespace std;

//creates a specific component object from a line of input
Component create_component(string &line)
{
    stringstream ss(line), check_type(line);
    string word;
    
    getline(check_type, word, ' ');
    
    switch (word[0])
    {
        case 'R':
            Resistor res(ss);
            return res;
        case 'C':
            Capacitor cap(ss);
            return cap;
        case 'L':
            Inductor ind(ss);
            return ind;
            
        case 'V':
            return choose_source(line, ss);
        case 'I':
            return choose_source(line, ss);
    }
}

//this function is kinda redundant, can be modified and moved to Nodes.hpp or modified for input directly
Nodes create_nodes(string &line)
{
    stringstream ss(line);
    string first_node, second_node;
    
    getline(ss, first_node, ' ');
    getline(ss, first_node, ' ');
    getline(ss, second_node, ' ');
    
    Nodes N;
    N.add_branch(make_pair(first_node, second_node), create_component(line));
    
    return N;
}
