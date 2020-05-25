#include "Components.hpp"
#include "Node.hpp"
#include <iostream>
#include <cstring>
#include <sstream>

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
