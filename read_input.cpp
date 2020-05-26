#include "Components.hpp"
#include "Node.hpp"
#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>

using namespace std;

Component create_component(string &line)
{
    stringstream ss(line);
    string word;
    
    getline(ss, word, ' ');
    
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
            
        //special cases for function as input
        case 'V':
            break;
        case 'I':
            break;
    }
}

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
