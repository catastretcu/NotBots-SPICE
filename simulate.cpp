#include "Components.hpp"
#include "Node.hpp"
#include "Analysis.hpp"
#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <utility>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

int main();
{
    Nodes circuit;
    Analysis command;
    
    read_input(circuit, command);
    
    if(command.get_type() == ".op")
    {
        node_voltages(circuit);
        //output voltages;
        circuit.print_voltages();
    }
    
    if(command.get_type() == ".tran")
    {
        //do transient analysis using discrete time
    }
}
