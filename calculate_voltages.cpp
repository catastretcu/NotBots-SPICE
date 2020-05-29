#include "Components.hpp"
#include "Node.hpp"
#include "Analysis.hpp"
#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <utility>



vector<double> node_voltages(Nodes &circuit, Analysis &command);
{
    //compute node voltages;
    if(command.type == ".op")
    {
        vector<double> voltages;
        voltages.resize(circuit.Size);
        //do inverse matrix and multiply with current vector to find voltage vector
    }
    if(command.type == ".tran")
    {
        //do transient analysis using discrete time
    }
}
