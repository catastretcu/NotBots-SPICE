#include "Components.hpp"
#include "Node.hpp"
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

//creates a specific component object from a line of input
Component create_component(string &line)
{
    stringstream ss(line);
    
    switch (line[0])
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
            
        //add diode and transistor support;
    }
}

//function used for direct input
Nodes map_circuit()
{
    Nodes circuit;
    string line;
    
    while(getline(cin, line))
        circuit.add_branch(read_nodes(line), create_component(line));
    
    return circuit;
}
